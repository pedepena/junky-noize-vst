#pragma once

#include <cmath>
#include <juce_gui_basics/juce_gui_basics.h>
#include "../Layout.h"

namespace junky
{
/** Dos columnas de segmentos calibradas con la escala impresa del panel. */
class LevelMeter : public juce::Component
{
public:
    void setLevels (float newLevelDb, float newPeakDb)
    {
        if (std::abs (newLevelDb - levelDb) > 0.01f || std::abs (newPeakDb - peakDb) > 0.01f)
        {
            levelDb = newLevelDb;
            peakDb = newPeakDb;
            repaint();
        }
    }

    void paint (juce::Graphics& g) override
    {
        const auto id = getComponentID();
        const auto& meter = id == "output-meter" ? layout::outputMeter : layout::inputMeter;
        const float scale = (float) getWidth() / meter.width;
        const float leftX = (meter.leftColumnCentreX - meter.x - layout::meterSegmentWidth * 0.5f) * scale;
        const float rightX = (meter.rightColumnCentreX - meter.x - layout::meterSegmentWidth * 0.5f) * scale;
        const float width = layout::meterSegmentWidth * scale;
        const float height = layout::meterSegmentHeight * scale;
        const float pitch = layout::meterSegmentPitch * scale;
        const float bottomCentre = (meter.height - layout::meterBottomInset - layout::meterSegmentHeight * 0.5f) * scale;
        const float levelY = (dbToY (levelDb) - meter.y) * scale;
        const float peakY = (dbToY (peakDb) - meter.y) * scale;

        for (int i = 0; i < layout::meterSegmentCount; ++i)
        {
            const float centreY = bottomCentre - (float) i * pitch;
            // Los segmentos apagados no se pintan. El fondo ya aporta su marca tenue.
            if (centreY < levelY - height * 0.5f) continue;
            const auto colour = colourForDb (yToDb (centreY / scale + meter.y));
            const auto segment = juce::Rectangle<float> (0.0f, centreY - height * 0.5f, width, height);
            for (const float x : { leftX, rightX })
            {
                g.setColour (colour.withAlpha (0.20f));
                g.fillRoundedRectangle (segment.translated (x, 0.0f).expanded (1.5f * scale), 2.0f * scale);
                g.setColour (colour);
                g.fillRoundedRectangle (segment.translated (x, 0.0f), 1.5f * scale);
                if (std::abs (centreY - peakY) <= layout::meterSegmentPitch * scale * 0.5f)
                {
                    g.setColour (juce::Colours::white.withAlpha (0.8f));
                    g.fillRect (x, centreY - height * 0.5f, width, juce::jmax (1.0f, scale));
                }
            }
        }
    }

private:
    static float dbToY (float db)
    {
        if (db >= 0.0f) return juce::jmax (layout::meterScaleTopY,
            layout::meterZeroDbY - db * ((layout::meterMinus6DbY - layout::meterZeroDbY) / 6.0f));
        if (db >= -6.0f) return juce::jmap (db, 0.0f, -6.0f, layout::meterZeroDbY, layout::meterMinus6DbY);
        if (db >= -12.0f) return juce::jmap (db, -6.0f, -12.0f, layout::meterMinus6DbY, layout::meterMinus12DbY);
        if (db >= -24.0f) return juce::jmap (db, -12.0f, -24.0f, layout::meterMinus12DbY, layout::meterMinus24DbY);
        if (db >= -48.0f) return juce::jmap (db, -24.0f, -48.0f, layout::meterMinus24DbY, layout::meterMinus48DbY);
        return juce::jmap (juce::jlimit (-60.0f, -48.0f, db), -48.0f, -60.0f,
                           layout::meterMinus48DbY, layout::meterMinus60DbY);
    }

    static float yToDb (float y)
    {
        if (y <= layout::meterZeroDbY)
            return (layout::meterZeroDbY - y) * (6.0f / (layout::meterMinus6DbY - layout::meterZeroDbY));
        if (y <= layout::meterMinus6DbY) return juce::jmap (y, layout::meterZeroDbY, layout::meterMinus6DbY, 0.0f, -6.0f);
        if (y <= layout::meterMinus12DbY) return juce::jmap (y, layout::meterMinus6DbY, layout::meterMinus12DbY, -6.0f, -12.0f);
        if (y <= layout::meterMinus24DbY) return juce::jmap (y, layout::meterMinus12DbY, layout::meterMinus24DbY, -12.0f, -24.0f);
        if (y <= layout::meterMinus48DbY) return juce::jmap (y, layout::meterMinus24DbY, layout::meterMinus48DbY, -24.0f, -48.0f);
        return juce::jmap (y, layout::meterMinus48DbY, layout::meterMinus60DbY, -48.0f, -60.0f);
    }

    static juce::Colour colourForDb (float db)
    {
        if (db > -3.0f) return juce::Colour (0xffe0392b);
        if (db >= -12.0f) return juce::Colour (0xffd6dc2a);
        return juce::Colour (0xff3fd43c);
    }

    float levelDb = -100.0f;
    float peakDb = -100.0f;
};
} // namespace junky
