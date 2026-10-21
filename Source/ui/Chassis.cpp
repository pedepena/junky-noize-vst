#include "Chassis.h"
#include "Theme.h"
#include "BinaryData.h"

namespace junky
{
void paintChassis (juce::Graphics& g)
{
    const auto image = juce::ImageCache::getFromMemory (BinaryData::JunkyNoizeReference_jpg,
                                                        BinaryData::JunkyNoizeReference_jpgSize);

    if (image.isValid())
        g.drawImage (image, 0, 0, juce::roundToInt (theme::designWidth), juce::roundToInt (theme::designHeight),
                     0, 0, image.getWidth(), image.getHeight());
    else
        g.fillAll (theme::metalMid);
}

void paintLabels (juce::Graphics& g, bool isSlotB)
{
    using namespace theme;

    // La escala viene grabada en la imagen de fondo; este disco oculta solo la escala
    // de frecuencia original de la referencia para sustituirla por 20-500 Hz.
    {
        juce::Graphics::ScopedSaveState state (g);
        g.reduceClipRegion (juce::Rectangle<int> (410, 710, 392, 225));
        juce::ColourGradient plate (juce::Colour (0xffc0bfb7), 680.0f, 738.0f,
                                    juce::Colour (0xff9b9c96), 680.0f, 938.0f, false);
        g.setGradientFill (plate);
        g.fillEllipse (576.0f, 736.0f, 208.0f, 208.0f);
        g.setColour (ink.withAlpha (0.32f));
        g.drawEllipse (576.0f, 736.0f, 208.0f, 208.0f, 2.0f);
    }

    // Valores impresos del knob de frecuencia: 20, 50, 100, 200 y 500 Hz.
    {
        const juce::Point<float> centre (680.0f, 840.0f);
        const float startAngle = juce::MathConstants<float>::pi * 1.25f;
        const float endAngle = juce::MathConstants<float>::pi * 2.75f;
        juce::NormalisableRange<float> range (20.0f, 500.0f, 1.0f);
        range.setSkewForCentre (100.0f);

        const std::pair<float, const char*> marks[] = { { 20.0f, "20" }, { 50.0f, "50" }, { 100.0f, "100" },
                                                        { 200.0f, "200" }, { 500.0f, "500" } };
        g.setColour (ink);

        for (auto [hz, text] : marks)
        {
            const float angle = startAngle + range.convertTo0to1 (hz) * (endAngle - startAngle);
            const juce::Rectangle<float> textBounds (70.0f, 28.0f);
            theme::drawBitmapText (g, text, textBounds.withCentre ({ centre.x + std::sin (angle) * 98.0f, centre.y - std::cos (angle) * 98.0f }), 18.0f);
        }
    }

    // Solo los pilotos A/B cambian con el estado; el resto de la rotulacion es arte fijo.
    const juce::Point<float> ledA (840.0f, 432.0f), ledB (975.0f, 432.0f);
    auto drawLed = [&g] (juce::Point<float> point, bool on)
    {
        g.setColour (juce::Colour (0xff77756d));
        g.fillEllipse (point.x - 16.0f, point.y - 16.0f, 32.0f, 32.0f);
        g.setColour (on ? theme::ledRed : juce::Colour (0xff3a0f0c));
        g.fillEllipse (point.x - 12.0f, point.y - 12.0f, 24.0f, 24.0f);
        g.setColour (juce::Colours::black.withAlpha (0.65f));
        g.drawEllipse (point.x - 12.0f, point.y - 12.0f, 24.0f, 24.0f, 2.0f);
    };

    drawLed (ledA, ! isSlotB);
    drawLed (ledB, isSlotB);
}
} // namespace junky
