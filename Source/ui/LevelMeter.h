#pragma once

#include <cmath>
#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"

namespace junky
{
/** Medidor vertical de 15 segmentos con escala entre -42 y 0 dB. */
class LevelMeter : public juce::Component
{
public:
    void setLevelDb (float newLevelDb)
    {
        if (std::abs (newLevelDb - levelDb) > 0.05f)
        {
            levelDb = newLevelDb;
            repaint();
        }
    }

    void paint (juce::Graphics& g) override
    {
        const auto bounds = getLocalBounds().toFloat();
        const auto bar = juce::Rectangle<float> (bounds.getX(), bounds.getY(), bounds.getWidth() * 0.38f,
                                                 bounds.getHeight());
        g.setColour (juce::Colour (0xff0e0e0e));
        g.fillRoundedRectangle (bar, 3.0f);
        constexpr int segments = 15;
        const float segmentHeight = (bar.getHeight() - 6.0f) / (float) segments;

        for (int i = 0; i < segments; ++i)
        {
            const float threshold = -42.0f + 3.0f * (float) i;
            const bool on = levelDb >= threshold;
            const auto colour = i >= 14 ? juce::Colour (0xffe03a2a)
                              : i >= 12 ? juce::Colour (0xffe8802a)
                              : i >= 9  ? juce::Colour (0xffe2c52c)
                                        : juce::Colour (0xff43c24a);
            const auto segment = juce::Rectangle<float> (bar.getX() + 3.0f,
                bar.getBottom() - 3.0f - (float) (i + 1) * segmentHeight + 1.5f,
                bar.getWidth() - 6.0f, segmentHeight - 3.0f);
            g.setColour (on ? colour : colour.darker (1.4f).withAlpha (0.55f));
            g.fillRect (segment);

            if (i % 2 == 0)
            {
                const juce::String text = juce::String ((int) threshold);
                const juce::Rectangle<float> label (bar.getRight() + 5.0f,
                    bar.getBottom() - 3.0f - (float) (i + 1) * segmentHeight,
                    bounds.getRight() - bar.getRight() - 5.0f, segmentHeight);
                g.setColour (theme::ink);
                theme::drawBitmapText (g, text, label, 9.0f);
            }
        }
    }

private:
    float levelDb = -100.0f;
};
} // namespace junky
