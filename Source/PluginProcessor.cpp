#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>

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
            wet[ch] = junky::quantise (wet[ch], bitDepth);
            wet[ch] += noiseSource.process (ch, 1.0f, noiseLevel);

            // Los perfiles de maquina y su EQ se incorporan en la fase 2.
            juce::ignoreUnused (characterValue);

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

    if (preset.isCustom)
        return;

    setParameterPlain (junky::params::filter, preset.filter);
    setParameterPlain (junky::params::mix, preset.mix);
    setParameterPlain (junky::params::bassBoost, preset.bassBoost ? 1.0f : 0.0f);
    setParameterPlain (junky::params::bassFreq, preset.bassFreq);
    setParameterPlain (junky::params::mono, preset.mono ? 1.0f : 0.0f);
}

void JunkyNoizeAudioProcessor::toggleAB()
{
    const int current = activeSlot.load();
    const int other = 1 - current;

    // Guardamos lo que suena ahora en el slot actual y cargamos el otro.
    slotState[current] = apvts.copyState();
    activeSlot.store (other);

    // La primera vez, B empieza siendo una copia de A.
    if (slotState[other].isValid())
        apvts.replaceState (slotState[other].createCopy());
}

void JunkyNoizeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    state.setProperty ("preset", currentPreset.load(), nullptr);

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
        }
    }
}

// Esta funcion la llama el DAW para crear el plugin.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JunkyNoizeAudioProcessor();
}
