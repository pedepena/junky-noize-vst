#pragma once

#include <memory>
#include <juce_audio_processors/juce_audio_processors.h>

#include "PluginProcessor.h"
#include "ui/KnobLookAndFeel.h"
#include "ui/LcdDisplay.h"
#include "ui/LevelMeter.h"
#include "ui/PanelButton.h"

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

    juce::Slider filterKnob, mixKnob, bassFreqKnob, inputKnob, outputKnob;

    junky::PanelButton abButton { junky::PanelButton::Style::key, junky::theme::ledRed, false };
    junky::PanelButton bassButton { junky::PanelButton::Style::key, junky::theme::ledRed, true };
    junky::PanelButton monoButton { junky::PanelButton::Style::key, junky::theme::ledRed, true };
    junky::PanelButton stereoButton { junky::PanelButton::Style::key, junky::theme::ledGreen, true };
    junky::PanelButton presetUpButton { junky::PanelButton::Style::arrowUp };
    junky::PanelButton presetDownButton { junky::PanelButton::Style::arrowDown };

    junky::LcdDisplay lcd;
    junky::LevelMeter inputMeter, outputMeter;

    float inputDisplayDb = -100.0f;
    float outputDisplayDb = -100.0f;
    bool displayedSlotB = false;

    juce::Image background;

    // Las conexiones con los parametros van al final: se destruyen antes que los controles.
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> filterAttachment, mixAttachment, bassFreqAttachment,
        inputAttachment, outputAttachment;
    std::unique_ptr<juce::ButtonParameterAttachment> bassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JunkyNoizeAudioProcessorEditor)
};
