#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>
#include <juce_core/juce_core.h>

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
            bassShelf.setLowShelf (currentSampleRate, bassHz, 6.0);
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

void JunkyNoizeAudioProcessor::selectPreset (int index)
{
    index = juce::jlimit (0, (int) junky::factoryPresets.size() - 1, index);
    currentPreset.store (index);

    const auto& preset = junky::factoryPresets[(size_t) index];

    if (preset.kind == junky::Preset::Kind::custom)
    {
        auto dir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
            .getChildFile ("Junky Noize").getChildFile ("Presets");
        auto file = dir.getChildFile ("custom" + juce::String (index - 50) + ".xml");
        if (file.existsAsFile())
            if (auto xml = juce::parseXML (file))
                if (xml->hasTagName (apvts.state.getType()))
                {
                    auto tree = juce::ValueTree::fromXml (*xml);
                    currentMachine.store (juce::jlimit (-1, 38, (int) tree.getProperty ("machine", -1)));
                    apvts.replaceState (tree);
                }
        return;
    }

    currentMachine.store (preset.machine);
    auto plain = [this] (const juce::String& id, float value) { setParameterPlain (id, value); };
    float drive = preset.drive, rate = preset.rate, alias = preset.alias, noise = preset.noise;
    int bits = preset.bitsIndex;
    if (index >= 1 && index <= 39)
    {
        static constexpr int bitValues[] = {1,1,0,0,0,0,0,1,1,0,0,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,2,1,0,0,1,2,0};
        static constexpr float rateValues[] = {14,12,12,12,15,15,18,16,21,15,12,31,31,29,12,18,12,10,7,25,24,27,24,14,21,21,18,18,25,25,12,33,24,21,20,15,21,35,23};
        static constexpr float driveValues[] = {.48f,.42f,.30f,.28f,.38f,.35f,.42f,.55f,.60f,.32f,.26f,.58f,.62f,.50f,.28f,.40f,.28f,.22f,.20f,.40f,.42f,.48f,.44f,.36f,.40f,.42f,.36f,.38f,.48f,.52f,.26f,.58f,.52f,.50f,.38f,.40f,.45f,.50f,.38f};
        static constexpr float aliasValues[] = {55,45,30,28,35,35,30,65,60,25,20,80,85,70,20,40,28,18,15,55,40,50,45,35,42,40,35,35,50,52,22,75,75,55,40,38,60,80,60};
        static constexpr float noiseValues[] = {18,14,10,8,12,10,12,30,24,10,7,28,26,35,8,14,10,6,5,30,35,22,18,14,18,16,14,14,22,22,8,30,40,20,14,14,28,35,20};
        const auto m = (size_t) (index - 1);
        bits = bitValues[m]; rate = rateValues[m] / 100.0f; drive = driveValues[m];
        alias = aliasValues[m] / 100.0f; noise = noiseValues[m] / 100.0f;
    }
    plain (junky::params::drive, drive);
    plain (junky::params::bits, (float) bits);
    plain (junky::params::rate, rate);
    plain (junky::params::alias, alias);
    plain (junky::params::noise, noise);
    plain (junky::params::drunk, preset.drunk);
    plain (junky::params::character, preset.character);
    if (preset.kind == junky::Preset::Kind::init)
    { plain (junky::params::input, 0); plain (junky::params::output, 0); }

    setParameterPlain (junky::params::filter, preset.filter);
    setParameterPlain (junky::params::mix, preset.mix);
    setParameterPlain (junky::params::bassBoost, preset.bassBoost ? 1.0f : 0.0f);
    setParameterPlain (junky::params::bassFreq, preset.bassFreq);
    setParameterPlain (junky::params::mono, preset.mono ? 1.0f : 0.0f);
}

int JunkyNoizeAudioProcessor::saveCustomPreset()
{
    auto dir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
        .getChildFile ("Junky Noize").getChildFile ("Presets");
    dir.createDirectory();
    int slot = currentPreset.load() >= 51 && currentPreset.load() <= 54 ? currentPreset.load() - 51 : -1;
    if (slot < 0)
        for (int i = 0; i < 4; ++i)
            if (! dir.getChildFile ("custom" + juce::String (i + 1) + ".xml").existsAsFile()) { slot = i; break; }
    if (slot < 0) return -1;
    auto tree = apvts.copyState(); tree.setProperty ("machine", currentMachine.load(), nullptr);
    auto xml = tree.createXml();
    return xml != nullptr && xml->writeTo (dir.getChildFile ("custom" + juce::String (slot + 1) + ".xml")) ? slot : -1;
}

void JunkyNoizeAudioProcessor::toggleAB()
{
    const int current = activeSlot.load();
    const int other = 1 - current;

    // Guardamos lo que suena ahora en el slot actual y cargamos el otro.
    slotState[current] = apvts.copyState();
    slotState[current].setProperty ("machine", currentMachine.load(), nullptr);
    activeSlot.store (other);

    // La primera vez, B empieza siendo una copia de A.
    if (slotState[other].isValid())
    {
        currentMachine.store (juce::jlimit (-1, 38, (int) slotState[other].getProperty ("machine", -1)));
        apvts.replaceState (slotState[other].createCopy());
    }
}

void JunkyNoizeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    state.setProperty ("preset", currentPreset.load(), nullptr);
    state.setProperty ("machine", currentMachine.load(), nullptr);

    if (auto xml = state.createXml())
        copyXmlToBinary (*xml, destData);
}

void JunkyNoizeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
    {
        if (xml->hasTagName (apvts.state.getType()))
        {
            auto tree = juce::ValueTree::fromXml (*xml);
            currentPreset.store (juce::jlimit (0, (int) junky::factoryPresets.size() - 1,
                                               (int) tree.getProperty ("preset", 0)));
            apvts.replaceState (tree);
            currentMachine.store (juce::jlimit (-1, 38, (int) tree.getProperty ("machine", -1)));
        }
    }
}

// Esta funcion la llama el DAW para crear el plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JunkyNoizeAudioProcessor();
}
