#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>
#include <juce_core/juce_core.h>

namespace
{
constexpr double bassBoostDb = 9.0;
} // namespace

JunkyNoizeAudioProcessor::JunkyNoizeAudioProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "JunkyNoize", junky::params::createLayout())
{
    inputParam = apvts.getRawParameterValue (junky::params::input);
    outputParam = apvts.getRawParameterValue (junky::params::output);
    mixParam = apvts.getRawParameterValue (junky::params::mix);
    filterParam = apvts.getRawParameterValue (junky::params::filter);
    bassBoostParam = apvts.getRawParameterValue (junky::params::bassBoost);
    bassFreqParam = apvts.getRawParameterValue (junky::params::bassFreq);
    monoParam = apvts.getRawParameterValue (junky::params::mono);
    driveParam = apvts.getRawParameterValue (junky::params::drive);
    bitsParam = apvts.getRawParameterValue (junky::params::bits);
    rateParam = apvts.getRawParameterValue (junky::params::rate);
    aliasParam = apvts.getRawParameterValue (junky::params::alias);
    noiseParam = apvts.getRawParameterValue (junky::params::noise);
    drunkParam = apvts.getRawParameterValue (junky::params::drunk);
    characterParam = apvts.getRawParameterValue (junky::params::character);
}

void JunkyNoizeAudioProcessor::prepareToPlay (double sampleRate, int)
{
    currentSampleRate = sampleRate;

    inputGain.reset (sampleRate, 0.03);
    outputGain.reset (sampleRate, 0.03);
    mixAmount.reset (sampleRate, 0.03);
    driveAmount.reset (sampleRate, 0.03);
    noiseGain.reset (sampleRate, 0.03);
    drunkAmount.reset (sampleRate, 0.1);

    inputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (inputParam->load()));
    outputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (outputParam->load()));
    mixAmount.setCurrentAndTargetValue (mixParam->load());
    driveAmount.setCurrentAndTargetValue (driveParam->load());
    const float initialNoise = noiseParam->load();
    noiseGain.setCurrentAndTargetValue (initialNoise < 0.0005f ? 0.0f
        : juce::Decibels::decibelsToGain (-66.0f + 42.0f * initialNoise));
    drunkAmount.setCurrentAndTargetValue (drunkParam->load());

    lowPass.reset();
    highShelf.reset();
    bassShelf.reset();
    aliasLowPass.reset();
    machineLowPass.reset();
    machineLowShelf.reset();
    machineHighShelf.reset();
    rateReducer.reset();
    noiseSource.reset();
    drunkDelay.prepare (sampleRate);
    lastFilterValue = 999.0f;
    lastBassHz = -1.0f;
    lastRateValue = -1.0f;
    lastAliasValue = -1.0f;
    rateReducerOn = false;
    lastMachine = -2;
    lastCharacter = -1.0f;
}

void JunkyNoizeAudioProcessor::updateMachineFilters (float character)
{
    const int machine = currentMachine.load();
    if (machine == lastMachine && character == lastCharacter) return;
    machineLowPass.reset(); machineLowShelf.reset(); machineHighShelf.reset();
    if (machine >= 0 && machine < (int) junky::machineProfiles.size())
    {
        const auto& profile = junky::machineProfiles[(size_t) machine];
        const double cutoff = 20000.0 * std::pow ((double) profile.lpHz / 20000.0, character);
        if (cutoff < 19000.0) machineLowPass.setLowPass (currentSampleRate, cutoff);
        if (std::abs (profile.lowShelfDb * character) >= .05f)
            machineLowShelf.setLowShelf (currentSampleRate, 120.0, profile.lowShelfDb * character);
        if (std::abs (profile.highShelfDb * character) >= .05f)
            machineHighShelf.setHighShelf (currentSampleRate, 6000.0, profile.highShelfDb * character);
    }
    lastMachine = machine; lastCharacter = character;
}

bool JunkyNoizeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& mainIn = layouts.getMainInputChannelSet();
    const auto& mainOut = layouts.getMainOutputChannelSet();

    if (mainIn != mainOut)
        return false;

    return mainOut == juce::AudioChannelSet::mono() || mainOut == juce::AudioChannelSet::stereo();
}

void JunkyNoizeAudioProcessor::updateFilters (float filterValue, bool bassBoostOn, float bassHz)
{
    if (filterValue != lastFilterValue)
    {
        const bool wasLowPassOn = lowPassOn;
        const bool wasHighShelfOn = highShelfOn;
        lowPassOn = filterValue < -0.001f;
        highShelfOn = filterValue > 0.001f;

        if (lowPassOn)
            lowPass.setLowPass (currentSampleRate, 20000.0 * std::pow (800.0 / 20000.0, (double) -filterValue));
        if (highShelfOn)
            highShelf.setHighShelf (currentSampleRate, 4000.0, 9.0 * (double) filterValue);
        if (wasLowPassOn != lowPassOn) lowPass.reset();
        if (wasHighShelfOn != highShelfOn) highShelf.reset();
        lastFilterValue = filterValue;
    }

    if (bassBoostOn != bassOn || bassHz != lastBassHz)
    {
        const bool wasBassOn = bassOn;
        bassOn = bassBoostOn;
        if (bassOn)
            bassShelf.setLowShelf (currentSampleRate, bassHz, bassBoostDb);
        if (wasBassOn != bassOn) bassShelf.reset();
        lastBassHz = bassHz;
    }
}

void JunkyNoizeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const int numChannels = juce::jmin (buffer.getNumChannels(), 2);
    const int numSamples = buffer.getNumSamples();

    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear (ch, 0, numSamples);

    if (numChannels == 0)
        return;

    inputGain.setTargetValue (juce::Decibels::decibelsToGain (inputParam->load()));
    outputGain.setTargetValue (juce::Decibels::decibelsToGain (outputParam->load()));
    mixAmount.setTargetValue (mixParam->load());
    driveAmount.setTargetValue (driveParam->load());

    const float rateValue = rateParam->load();
    const float aliasValue = aliasParam->load();
    const float noiseValue = noiseParam->load();
    const float characterValue = characterParam->load();
    const int bitIndex = juce::jlimit (0, 3, static_cast<int> (std::lround (bitsParam->load())));
    constexpr int bitDepths[4] { 16, 12, 8, 4 };
    const int bitDepth = bitDepths[bitIndex];
    noiseGain.setTargetValue (noiseValue < 0.0005f ? 0.0f
        : juce::Decibels::decibelsToGain (-66.0f + 42.0f * noiseValue));
    drunkAmount.setTargetValue (drunkParam->load());

    const bool rateActive = rateValue >= 0.001f;
    const double rateHz = 48000.0 * std::pow (4000.0 / 48000.0, (double) rateValue);
    const double ratio = ! rateActive || rateHz >= currentSampleRate * 0.999
        ? 1.0 : rateHz / currentSampleRate;
    if (rateValue != lastRateValue || aliasValue != lastAliasValue)
    {
        const bool nextRateReducerOn = rateActive && ratio < 0.999;
        if (nextRateReducerOn && ! rateReducerOn)
            rateReducer.reset();
        rateReducerOn = nextRateReducerOn;
        const bool wasAliasLowPassOn = aliasLowPassOn;
        aliasLowPassOn = rateReducerOn;
        if (aliasLowPassOn)
        {
            const double fcClean = 0.40 * rateHz;
            const double fcGritty = juce::jmin (0.45 * currentSampleRate, 20000.0);
            const double fcAlias = fcClean * std::pow (juce::jmax (fcGritty, fcClean) / fcClean,
                                                       (double) aliasValue);
            aliasLowPass.setLowPass (currentSampleRate, fcAlias);
        }
        if (wasAliasLowPassOn != aliasLowPassOn)
            aliasLowPass.reset();
        lastRateValue = rateValue;
        lastAliasValue = aliasValue;
    }

    updateFilters (filterParam->load(), bassBoostParam->load() > 0.5f, bassFreqParam->load());
    updateMachineFilters (characterValue);

    const bool monoOn = monoParam->load() > 0.5f && numChannels == 2;

    float* data[2] = { buffer.getWritePointer (0),
                       numChannels > 1 ? buffer.getWritePointer (1) : nullptr };

    float inPeak = 0.0f;
    float outPeak = 0.0f;

    for (int i = 0; i < numSamples; ++i)
    {
        const float inGain = inputGain.getNextValue();
        const float outGain = outputGain.getNextValue();
        const float mixValue = mixAmount.getNextValue();
        const float driveValue = driveAmount.getNextValue();
        const float noiseLevel = noiseGain.getNextValue();
        const float drunkValue = drunkAmount.getNextValue();

        float dry[2] = { 0.0f, 0.0f };
        float wet[2] = { 0.0f, 0.0f };
        float mixed[2] = { 0.0f, 0.0f };

        for (int ch = 0; ch < numChannels; ++ch)
            dry[ch] = data[ch][i] * inGain;

        if (monoOn)
        {
            const float m = 0.5f * (dry[0] + dry[1]);
            dry[0] = m;
            dry[1] = m;
        }

        for (int ch = 0; ch < numChannels; ++ch)
        {
            inPeak = juce::jmax (inPeak, std::abs (dry[ch]));
            wet[ch] = junky::saturate (dry[ch], driveValue);
            if (aliasLowPassOn)
                wet[ch] = aliasLowPass.process (ch, wet[ch]);
        }

        rateReducer.process (wet, numChannels, ratio);

        for (int ch = 0; ch < numChannels; ++ch)
        {
            const int machine = currentMachine.load();
            const bool companded = machine >= 0 && machine < (int) junky::machineProfiles.size()
                && junky::machineProfiles[(size_t) machine].companded;
            wet[ch] = junky::quantise (wet[ch], bitDepth, companded);
            wet[ch] += noiseSource.process (ch, 1.0f, noiseLevel);
            if (machine >= 0 && machine < (int) junky::machineProfiles.size())
            {
                const auto& profile = junky::machineProfiles[(size_t) machine];
                const double cutoff = 20000.0 * std::pow ((double) profile.lpHz / 20000.0, characterValue);
                if (cutoff < 19000.0) wet[ch] = machineLowPass.process (ch, wet[ch]);
                if (std::abs (profile.lowShelfDb * characterValue) >= .05f) wet[ch] = machineLowShelf.process (ch, wet[ch]);
                if (std::abs (profile.highShelfDb * characterValue) >= .05f) wet[ch] = machineHighShelf.process (ch, wet[ch]);
            }

            if (lowPassOn)
                wet[ch] = lowPass.process (ch, wet[ch]);

            if (highShelfOn)
                wet[ch] = highShelf.process (ch, wet[ch]);

            if (bassOn)
                wet[ch] = bassShelf.process (ch, wet[ch]);

            mixed[ch] = dry[ch] + mixValue * (wet[ch] - dry[ch]);
        }

        drunkDelay.process (mixed, numChannels, drunkValue);
        for (int ch = 0; ch < numChannels; ++ch)
        {
            const float out = mixed[ch] * outGain;
            data[ch][i] = out;
            outPeak = juce::jmax (outPeak, std::abs (out));
        }
    }

    inputPeak.store (juce::jmax (inputPeak.load(), inPeak));
    outputPeak.store (juce::jmax (outputPeak.load(), outPeak));
}

juce::AudioProcessorEditor* JunkyNoizeAudioProcessor::createEditor()
{
    return new JunkyNoizeAudioProcessorEditor (*this);
}

void JunkyNoizeAudioProcessor::setParameterPlain (const juce::String& parameterId, float plainValue)
{
    if (auto* parameter = apvts.getParameter (parameterId))
    {
        parameter->beginChangeGesture();
        parameter->setValueNotifyingHost (parameter->convertTo0to1 (plainValue));
        parameter->endChangeGesture();
    }
}


namespace
{
juce::File userPresetDirectory()
{
    return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
        .getChildFile ("Junky Noize").getChildFile ("Presets");
}
}

juce::StringArray JunkyNoizeAudioProcessor::getPresetNames() const
{
    juce::StringArray names;
    for (const auto& preset : junky::factoryPresets) names.add (preset.name);
    const juce::ScopedLock lock (userPresetLock);
    names.addArray (userPresetNames);
    return names;
}

int JunkyNoizeAudioProcessor::getLcdSelectionIndex() const
{
    const juce::ScopedLock lock (userPresetLock);
    if (selectedUserPreset.isNotEmpty())
    {
        const int pos = userPresetNames.indexOf (selectedUserPreset, true);
        if (pos >= 0) return 51 + pos;
    }
    return juce::jlimit (0, 50, currentPreset.load());
}

bool JunkyNoizeAudioProcessor::refreshUserPresets()
{
    juce::StringArray next;
    const auto dir = userPresetDirectory();
    if (dir.isDirectory())
    {
        juce::Array<juce::File> files;
        dir.findChildFiles (files, juce::File::findFiles, false, "*.xml");
        for (const auto& file : files) next.add (file.getFileNameWithoutExtension());
    }
    next.sort (true);
    const juce::ScopedLock lock (userPresetLock);
    if (next == userPresetNames) return false;
    userPresetNames = next;
    if (selectedUserPreset.isNotEmpty() && userPresetNames.indexOf (selectedUserPreset, true) < 0)
    {
        selectedUserPreset.clear();
        currentPreset.store (0);
    }
    return true;
}

void JunkyNoizeAudioProcessor::selectPreset (int index)
{
    if (index < 51)
    {
        index = juce::jlimit (0, 50, index);
        { const juce::ScopedLock lock (userPresetLock); selectedUserPreset.clear(); }
        currentPreset.store (index);
        const auto& preset = junky::factoryPresets[(size_t) index];
        currentMachine.store (preset.machine);
        auto plain = [this] (const juce::String& id, float value) { setParameterPlain (id, value); };
        plain (junky::params::drive, preset.drive);
        plain (junky::params::bits, (float) preset.bitsIndex);
        plain (junky::params::rate, preset.rate);
        plain (junky::params::alias, preset.alias);
        plain (junky::params::noise, preset.noise);
        plain (junky::params::drunk, preset.drunk);
        plain (junky::params::character, preset.character);
        if (preset.kind == junky::Preset::Kind::init)
        { plain (junky::params::input, 0); plain (junky::params::output, 0); }
        plain (junky::params::filter, preset.filter);
        plain (junky::params::mix, preset.mix);
        plain (junky::params::bassBoost, preset.bassBoost ? 1.0f : 0.0f);
        plain (junky::params::bassFreq, preset.bassFreq);
        plain (junky::params::mono, preset.mono ? 1.0f : 0.0f);
        return;
    }

    juce::String name;
    {
        const juce::ScopedLock lock (userPresetLock);
        const int pos = index - 51;
        if (! juce::isPositiveAndBelow (pos, userPresetNames.size())) return;
        name = userPresetNames[pos];
    }
    const auto file = userPresetDirectory().getChildFile (name + ".xml");
    auto xml = file.existsAsFile() ? juce::parseXML (file) : nullptr;
    if (xml == nullptr || ! xml->hasTagName (apvts.state.getType())) { refreshUserPresets(); return; }
    auto tree = juce::ValueTree::fromXml (*xml);
    if (! tree.isValid()) { refreshUserPresets(); return; }
    currentMachine.store (juce::jlimit (-1, 38, (int) tree.getProperty ("machine", -1)));
    apvts.replaceState (tree);
    currentPreset.store (0);
    { const juce::ScopedLock lock (userPresetLock); selectedUserPreset = name; }
}

bool JunkyNoizeAudioProcessor::saveUserPreset (const juce::String& name, bool replaceExisting)
{
    auto dir = userPresetDirectory();
    if (! dir.createDirectory().wasOk()) return false;
    auto target = dir.getChildFile (name + ".xml");
    juce::String oldName;
    {
        const juce::ScopedLock lock (userPresetLock);
        for (const auto& existing : userPresetNames)
            if (existing.equalsIgnoreCase (name)) { oldName = existing; break; }
    }
    if (oldName.isNotEmpty() && ! replaceExisting) return false;
    auto tree = apvts.copyState();
    tree.setProperty ("machine", juce::jlimit (-1, 38, currentMachine.load()), nullptr);
    auto xml = tree.createXml();
    if (xml == nullptr || ! xml->writeTo (target)) return false;
    if (oldName.isNotEmpty() && oldName != name)
        dir.getChildFile (oldName + ".xml").deleteFile();
    refreshUserPresets();
    currentPreset.store (0);
    { const juce::ScopedLock lock (userPresetLock); selectedUserPreset = name; }
    return true;
}

void JunkyNoizeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    state.setProperty ("preset", currentPreset.load(), nullptr);
    { const juce::ScopedLock lock (userPresetLock); state.setProperty ("userPreset", selectedUserPreset, nullptr); }
    state.setProperty ("machine", currentMachine.load(), nullptr);
    state.setProperty ("editorWidth", editorWidth.load(), nullptr);
    state.setProperty ("editorHeight", editorHeight.load(), nullptr);
    if (auto xml = state.createXml()) copyXmlToBinary (*xml, destData);
}

void JunkyNoizeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
        {
            auto tree = juce::ValueTree::fromXml (*xml);
            const int restoredPreset = juce::jlimit (0, 50, (int) tree.getProperty ("preset", 0));
            const auto restoredUser = tree.getProperty ("userPreset", juce::String()).toString();
            editorWidth.store (juce::jlimit (0, 100000, (int) tree.getProperty ("editorWidth", 0)));
            editorHeight.store (juce::jlimit (0, 100000, (int) tree.getProperty ("editorHeight", 0)));
            apvts.replaceState (tree);
            currentMachine.store (juce::jlimit (-1, 38, (int) tree.getProperty ("machine", -1)));
            currentPreset.store (restoredPreset);
            refreshUserPresets();
            { const juce::ScopedLock lock (userPresetLock); selectedUserPreset.clear(); }
            if (restoredUser.isNotEmpty())
            {
                const auto file = userPresetDirectory().getChildFile (restoredUser + ".xml");
                if (file.existsAsFile())
                {
                    const juce::ScopedLock lock (userPresetLock);
                    if (userPresetNames.indexOf (restoredUser, true) >= 0)
                        selectedUserPreset = restoredUser;
                }
            }
        }
}

// Esta funcion la llama el DAW para crear el plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JunkyNoizeAudioProcessor();
}
