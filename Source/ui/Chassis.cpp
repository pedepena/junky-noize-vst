#include "Chassis.h"
#include "Theme.h"
#include "BinaryData.h"

#include <cmath>

namespace junky
{
namespace
{
void drawCentred (juce::Graphics& g, const juce::String& text, juce::Point<float> centre, float height)
{
    const float width = (float) text.length() * height * 6.0f / 7.0f;
    theme::drawBitmapText (g, text, { centre.x - width * 0.5f, centre.y - height * 0.5f, width, height }, height);
}

juce::Point<float> polarPoint (juce::Point<float> centre, float degrees, float radius)
{
    const float angle = juce::MathConstants<float>::pi * degrees / 180.0f;
    return { centre.x + std::sin (angle) * radius, centre.y - std::cos (angle) * radius };
}
} // namespace

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

void paintLabels (juce::Graphics& g, bool)
{
    g.setColour (theme::ink);
    drawCentred (g, "FILTER", { 285.0f, 425.0f }, 14.0f);
    drawCentred (g, "DARK", { 214.0f, 606.0f }, 13.0f);
    drawCentred (g, "BRIGHT", { 340.0f, 606.0f }, 13.0f);
    drawCentred (g, "MIX", { 543.0f, 632.0f }, 26.0f);
    drawCentred (g, "0%", { 421.0f, 607.0f }, 13.0f);
    drawCentred (g, "100%", { 672.0f, 607.0f }, 13.0f);
    drawCentred (g, "DRUNK", { 814.0f, 425.0f }, 14.0f);
    drawCentred (g, "OFF", { 748.0f, 606.0f }, 13.0f);
    drawCentred (g, "MAX", { 866.0f, 606.0f }, 13.0f);
    drawCentred (g, "MONO", { 147.0f, 753.0f }, 15.0f);
    drawCentred (g, "STEREO", { 305.0f, 753.0f }, 15.0f);
    drawCentred (g, "BASS BOOST", { 535.0f, 735.0f }, 15.0f);
    drawCentred (g, "FREQ", { 697.0f, 733.0f }, 15.0f);
    g.drawLine (465.0f, 754.0f, 745.0f, 754.0f, 1.0f);
    drawCentred (g, "INPUT", { 874.0f, 711.0f }, 13.0f);
    drawCentred (g, "OUTPUT", { 978.0f, 711.0f }, 13.0f);

    g.drawLine (79.0f, 998.0f, 777.0f, 998.0f, 1.0f);
    theme::drawBitmapText (g, "COLOR", { 101.0f, 970.0f, 100.0f, 16.0f }, 16.0f);

    constexpr float knobX[] = { 118.0f, 236.0f, 354.0f, 472.0f, 590.0f, 708.0f };
    constexpr const char* knobNames[] = { "DRIVE", "BITS", "RATE", "ALIAS", "NOISE", "CHARACTER" };
    for (int i = 0; i < 6; ++i)
        drawCentred (g, knobNames[i], { knobX[i], 1105.0f }, 12.0f);

    for (int i : { 0, 2, 4, 5 })
    {
        const juce::Point<float> centre (knobX[i], 1052.0f);
        drawCentred (g, "0%", polarPoint (centre, 225.0f, 52.0f), 9.0f);
        drawCentred (g, "100%", polarPoint (centre, 135.0f, 52.0f), 9.0f);
    }
    drawCentred (g, "SMOOTH", polarPoint ({ knobX[3], 1052.0f }, 225.0f, 58.0f), 7.0f);
    drawCentred (g, "GRITTY", polarPoint ({ knobX[3], 1052.0f }, 135.0f, 58.0f), 7.0f);

    constexpr const char* bitLabels[] = { "16", "12", "8", "4" };
    constexpr float bitAngles[] = { 225.0f, 315.0f, 45.0f, 135.0f };
    for (int i = 0; i < 4; ++i)
        drawCentred (g, bitLabels[i], polarPoint ({ knobX[1], 1052.0f }, bitAngles[i], 54.0f), 9.0f);

    drawCentred (g, "IN", { 873.0f, 1118.0f }, 14.0f);
    drawCentred (g, "OUT", { 980.0f, 1118.0f }, 14.0f);

    const juce::Point<float> freqCentre (680.0f, 841.0f);
    const float start = juce::MathConstants<float>::pi * 1.25f;
    const float end = juce::MathConstants<float>::pi * 2.75f;
    juce::NormalisableRange<float> freqRange (20.0f, 500.0f, 1.0f);
    freqRange.setSkewForCentre (100.0f);
    const std::pair<float, const char*> marks[] = { { 20.0f, "20" }, { 50.0f, "50" }, { 100.0f, "100" },
                                                    { 200.0f, "200" }, { 500.0f, "500" } };
    for (auto [hz, label] : marks)
    {
        const float angle = start + freqRange.convertTo0to1 (hz) * (end - start);
        const float radius = hz == 100.0f ? 79.0f : 90.0f;
        drawCentred (g, label, { freqCentre.x + std::sin (angle) * radius,
                                 freqCentre.y - std::cos (angle) * radius }, 18.0f);
    }
}
} // namespace junky
