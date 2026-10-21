#include "PluginProcessor.h"
#include "PluginEditor.h"

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
}

void JunkyNoizeAudioProcessor::prepareToPlay (double sampleRate, int)
{
    currentSampleRate = sampleRate;

    inputGain.reset (sampleRate, 0.03);
    outputGain.reset (sampleRate, 0.03);
    mixAmount.reset (sampleRate, 0.03);

    inputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (inputParam->load()));
    outputGain.setCurrentAndTargetValue (juce::Decibels::decibelsToGain (outputParam->load()));
    mixAmount.setCurrentAndTargetValue (mixParam->load());

    lowPass.reset();
    highShelf.reset();
    bassShelf.reset();
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
    // Filtro tipo "tono": hacia la izquierda oscurece (paso bajo), hacia la derecha abrillanta (shelf de agudos).
    lowPassOn = filterValue < -0.001f;
    highShelfOn = filterValue > 0.001f;

    if (lowPassOn)
        lowPass.setLowPass (currentSampleRate, 20000.0 * std::pow (800.0 / 20000.0, (double) -filterValue));

    if (highShelfOn)
        highShelf.setHighShelf (currentSampleRate, 4000.0, 9.0 * (double) filterValue);

    bassOn = bassBoostOn;

    if (bassOn)
        bassShelf.setLowShelf (currentSampleRate, bassHz, 6.0);
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

        float dry[2] = { 0.0f, 0.0f };

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

            // Aqui es donde, en la fase 2, entrara el motor de las maquinas de sampleo.
            float wet = dry[ch];

            if (lowPassOn)
                wet = lowPass.process (ch, wet);

            if (highShelfOn)
                wet = highShelf.process (ch, wet);

            if (bassOn)
                wet = bassShelf.process (ch, wet);

            const float out = (dry[ch] + mixValue * (wet - dry[ch])) * outGain;
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
