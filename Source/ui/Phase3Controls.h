#pragma once

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

namespace junky
{
class ImageToggleButton final : public juce::Button
{
public:
    ImageToggleButton();
    void paintButton (juce::Graphics&, bool isMouseOverButton, bool isButtonDown) override;

private:
    juce::Image image;
};

class ImageLed final : public juce::Component
{
public:
    ImageLed();
    void setOn (bool shouldBeOn);
    void paint (juce::Graphics&) override;

private:
    juce::Image onImage;
    juce::Image offImage;
    bool isOn = false;
};

class MonoStereoLever final : public juce::Component,
                              private juce::Timer
{
public:
    MonoStereoLever();
    std::function<void (bool)> onToggle;
    void setMono (bool shouldBeMono);
    void paint (juce::Graphics&) override;
    void mouseUp (const juce::MouseEvent&) override;

private:
    void timerCallback() override;

    juce::Image image;
    bool targetMono = false;
    float currentCentreX = 0.0f;
    float animationStartX = 0.0f;
    float animationTargetX = 0.0f;
    int animationElapsedMs = 0;
};
} // namespace junky