#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace junky
{
/** Dibuja los knobs giratorios: tapa negra desgastada, marcas alrededor y aguja clara. */
class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider& slider) override;
};
} // namespace junky
