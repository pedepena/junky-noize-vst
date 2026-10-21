#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"

namespace junky
{
/** Medidor de nivel vertical con segmentos de colores y la escala 0, -6, -12, -18, -24 dB. */
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
        auto bounds = getLocalBounds().toFloat();
        auto barArea = bounds.removeFromLeft (bounds.getWidth() * 0.38f);

        g.setColour (juce::Colour (0xff0e0e0e));
        g.fillRoundedRectangle (barArea, 3.0f);

        const int numSegments = 10;
        const float topDb = 0.0f;
        const float bottomDb = -30.0f;
        const float segHeight = (barArea.getHeight() - 6.0f) / (float) numSegments;

        for (int i = 0; i < numSegments; ++i)
        {
            // i = 0 es el segmento de abajo
            const float thresholdDb = bottomDb + (float) i * ((topDb - bottomDb) / (float) numSegments);
            const bool on = levelDb >= thresholdDb;

            juce::Colour colour = i >= 9 ? juce::Colour (0xffe03a2a)
                                : i >= 8 ? juce::Colour (0xffe8802a)
                                : i >= 6 ? juce::Colour (0xffe2c52c)
                                         : juce::Colour (0xff43c24a);

            auto seg = juce::Rectangle<float> (barArea.getX() + 3.0f,
                                               barArea.getBottom() - 3.0f - (float) (i + 1) * segHeight + 1.5f,
                                               barArea.getWidth() - 6.0f, segHeight - 3.0f);

            g.setColour (on ? colour : colour.darker (1.4f).withAlpha (0.55f));
            g.fillRect (seg);
        }

    }

private:
    float levelDb = -100.0f;
};
} // namespace junky
