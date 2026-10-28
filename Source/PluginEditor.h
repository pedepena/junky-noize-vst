#pragma once

#include <memory>
#include <juce_audio_processors/juce_audio_processors.h>

#include "PluginProcessor.h"
#include "ui/KnobLookAndFeel.h"
#include "ui/LcdDisplay.h"
#include "ui/LevelMeter.h"
#include "ui/AssetButton.h"
#include "ui/Phase3Controls.h"

class JunkyNoizeAudioProcessorEditor : public juce::AudioProcessorEditor,
                                       private juce::Timer
{
public:
    explicit JunkyNoizeAudioProcessorEditor (JunkyNoizeAudioProcessor&);
    ~JunkyNoizeAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void rebuildBackground();
    void setupKnob (juce::Slider& knob);
    juce::Rectangle<int> fromDesign (float x, float y, float w, float h) const;

    JunkyNoizeAudioProcessor& audioProcessor;
    junky::KnobLookAndFeel knobLook;

    juce::Slider filterKnob, mixKnob, bassFreqKnob, inputKnob, outputKnob, drunkKnob;
    juce::Slider driveKnob, bitsKnob, rateKnob, aliasKnob, noiseKnob, characterKnob;

    junky::AssetButton folderButton;
    junky::AssetButton saveButton;
    junky::ImageToggleButton bassButton;
    junky::ImageLed bassLed, monoLed, stereoLed;
    junky::MonoStereoLever monoStereoLever;
    junky::AssetButton presetUpButton;
    junky::AssetButton presetDownButton;

    junky::LcdDisplay lcd;
    junky::LevelMeter inputMeter, outputMeter;

    float inputDisplayDb = -100.0f;
    float outputDisplayDb = -100.0f;
    float inputPeakHoldDb = -100.0f;
    float outputPeakHoldDb = -100.0f;
    int inputPeakHoldTicks = 0;
    int outputPeakHoldTicks = 0;
    int presetRefreshTicks = 0;

    juce::Image background;

    // Las conexiones con los parametros van al final: se destruyen antes que los controles.
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> filterAttachment, mixAttachment, bassFreqAttachment,
        inputAttachment, outputAttachment, drunkAttachment, driveAttachment, bitsAttachment,
        rateAttachment, aliasAttachment, noiseAttachment, characterAttachment;
    std::unique_ptr<juce::ButtonParameterAttachment> bassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JunkyNoizeAudioProcessorEditor)
};
