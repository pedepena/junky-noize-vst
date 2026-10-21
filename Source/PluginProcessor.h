#pragma once

#include <atomic>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Parameters.h"
#include "Presets.h"
#include "dsp/Biquad.h"

class JunkyNoizeAudioProcessor : public juce::AudioProcessor
{
public:
    JunkyNoizeAudioProcessor();
    ~JunkyNoizeAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Junky Noize"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // ---- Funciones para la interfaz ----------------------------------------
    int getCurrentPreset() const { return currentPreset.load(); }
    void selectPreset (int index);

    /** Cambia entre los ajustes A y B (para comparar dos sonidos). */
    void toggleAB();
    bool isSlotB() const { return activeSlot.load() == 1; }

    /** Cambia un parametro avisando al DAW (para que se pueda automatizar). */
    void setParameterPlain (const juce::String& parameterId, float plainValue);

    /** Devuelve el pico desde la ultima llamada (valor lineal, 1.0 = 0 dBFS). */
    float takeInputPeak() { return inputPeak.exchange (0.0f); }
    float takeOutputPeak() { return outputPeak.exchange (0.0f); }

    juce::AudioProcessorValueTreeState apvts;

private:
    void updateFilters (float filterValue, bool bassOn, float bassHz);

    std::atomic<float>* inputParam = nullptr;
    std::atomic<float>* outputParam = nullptr;
    std::atomic<float>* mixParam = nullptr;
    std::atomic<float>* filterParam = nullptr;
    std::atomic<float>* bassBoostParam = nullptr;
    std::atomic<float>* bassFreqParam = nullptr;
    std::atomic<float>* monoParam = nullptr;

    juce::SmoothedValue<float> inputGain, outputGain, mixAmount;

    junky::Biquad lowPass, highShelf, bassShelf;
    bool lowPassOn = false, highShelfOn = false, bassOn = false;
    double currentSampleRate = 44100.0;

    std::atomic<int> currentPreset { 0 };
    std::atomic<int> activeSlot { 0 };
    juce::ValueTree slotState[2];

    std::atomic<float> inputPeak { 0.0f };
    std::atomic<float> outputPeak { 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JunkyNoizeAudioProcessor)
};
