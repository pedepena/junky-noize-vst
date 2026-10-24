#pragma once

#include <atomic>
#include <juce_audio_processors/juce_audio_processors.h>

#include "Parameters.h"
#include "Presets.h"
#include "Machines.h"
#include "dsp/Biquad.h"
#include "dsp/BitCrusher.h"
#include "dsp/DrunkDelay.h"
#include "dsp/NoiseSource.h"
#include "dsp/RateReducer.h"
#include "dsp/Saturator.h"

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
    int getLcdSelectionIndex() const;
    juce::StringArray getPresetNames() const;
    bool refreshUserPresets();
    void selectPreset (int index);
    bool saveUserPreset (const juce::String& name, bool replaceExisting);
    int getEditorWidth() const { return editorWidth.load(); }
    void setEditorWidth (int width) { editorWidth.store (width); }

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
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* bitsParam = nullptr;
    std::atomic<float>* rateParam = nullptr;
    std::atomic<float>* aliasParam = nullptr;
    std::atomic<float>* noiseParam = nullptr;
    std::atomic<float>* drunkParam = nullptr;
    std::atomic<float>* characterParam = nullptr;

    juce::SmoothedValue<float> inputGain, outputGain, mixAmount;
    juce::SmoothedValue<float> driveAmount, noiseGain, drunkAmount;

    junky::Biquad lowPass, highShelf, bassShelf, aliasLowPass;
    junky::Biquad machineLowPass, machineLowShelf, machineHighShelf;
    junky::RateReducer rateReducer;
    junky::NoiseSource noiseSource;
    junky::DrunkDelay drunkDelay;
    bool lowPassOn = false, highShelfOn = false, bassOn = false;
    bool aliasLowPassOn = false;
    bool rateReducerOn = false;
    double currentSampleRate = 44100.0;
    float lastFilterValue = 999.0f, lastBassHz = -1.0f;
    float lastRateValue = -1.0f, lastAliasValue = -1.0f;

    std::atomic<int> currentPreset { 0 };
    mutable juce::CriticalSection userPresetLock;
    juce::StringArray userPresetNames;
    juce::String selectedUserPreset;
    std::atomic<int> currentMachine { -1 };
    std::atomic<int> editorWidth { 0 };
    void updateMachineFilters (float character);
    int lastMachine = -2;
    float lastCharacter = -1.0f;

    std::atomic<float> inputPeak { 0.0f };
    std::atomic<float> outputPeak { 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JunkyNoizeAudioProcessor)
};
