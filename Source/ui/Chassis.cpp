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
    theme::drawPanelText (g, text, centre.x, centre.y, height);
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

void paintLabels (juce::Graphics& g)
{
    drawCentred (g, "FILTER", { 277.0f, 428.0f }, 26.0f);
    drawCentred (g, "DRUNK", { 808.0f, 428.0f }, 26.0f);
    drawCentred (g, "DARK", { 196.0f, 624.0f }, 20.0f);
    drawCentred (g, "OFF", { 735.0f, 624.0f }, 20.0f);
    drawCentred (g, "BRIGHT", { 358.0f, 624.0f }, 20.0f);
    drawCentred (g, "MAX", { 881.0f, 624.0f }, 20.0f);
    drawCentred (g, "MIX", { 540.0f, 652.0f }, 40.0f);
    drawCentred (g, "0%", { 420.0f, 626.0f }, 20.0f);
    drawCentred (g, "100%", { 660.0f, 626.0f }, 20.0f);
    drawCentred (g, "MONO", { 147.0f, 753.0f }, 24.0f);
    drawCentred (g, "STEREO", { 305.0f, 753.0f }, 24.0f);
    drawCentred (g, "BASS BOOST", { 535.0f, 737.0f }, 24.0f);
    drawCentred (g, "FREQ", { 680.0f, 737.0f }, 24.0f);
    g.drawLine (465.0f, 754.0f, 745.0f, 754.0f, 1.0f);
    drawCentred (g, "INPUT", { 884.0f, 708.0f }, 19.0f);
    drawCentred (g, "OUTPUT", { 986.0f, 708.0f }, 19.0f);

    g.drawLine (79.0f, 998.0f, 777.0f, 998.0f, 1.0f);
    drawCentred (g, "COLOR", { 146.0f, 978.0f }, 24.0f);

    constexpr float knobX[] = { 138.0f, 252.0f, 366.0f, 480.0f, 594.0f, 708.0f };
    constexpr const char* knobNames[] = { "DRIVE", "BITS", "RATE", "ALIAS", "NOISE", "CHARACTER" };
    for (int i = 0; i < 6; ++i)
        drawCentred (g, knobNames[i], { knobX[i], 1018.0f }, 18.0f);

    for (int i : { 0, 2, 4, 5 })
    {
        drawCentred (g, "0%", { knobX[i] - 34.0f, 1106.0f }, 17.0f);
        drawCentred (g, "100%", { knobX[i] + 34.0f, 1106.0f }, 17.0f);
    }

    drawCentred (g, "16", { knobX[1] - 36.0f, 1106.0f }, 17.0f);
    drawCentred (g, "4", { knobX[1] + 36.0f, 1106.0f }, 17.0f);
    drawCentred (g, "12", { knobX[1] - 38.0f, 1036.0f }, 17.0f);
    drawCentred (g, "8", { knobX[1] + 38.0f, 1036.0f }, 17.0f);
    drawCentred (g, "SMOOTH", { knobX[3] - 30.0f, 1110.0f }, 12.0f);
    drawCentred (g, "GRITTY", { knobX[3] + 30.0f, 1110.0f }, 12.0f);

    drawCentred (g, "IN", { 873.0f, 1116.0f }, 22.0f);
    drawCentred (g, "OUT", { 980.0f, 1116.0f }, 22.0f);

    const juce::Point<float> freqCentre (680.0f, 852.0f);
    const float start = juce::MathConstants<float>::pi * 1.25f;
    const float end = juce::MathConstants<float>::pi * 2.75f;
    juce::NormalisableRange<float> freqRange (20.0f, 500.0f, 1.0f);
    freqRange.setSkewForCentre (100.0f);
    const std::pair<float, const char*> marks[] = { { 20.0f, "20" }, { 50.0f, "50" }, { 100.0f, "100" },
                                                    { 200.0f, "200" }, { 500.0f, "500" } };
    for (auto [hz, label] : marks)
    {
        const float angle = start + freqRange.convertTo0to1 (hz) * (end - start);
        drawCentred (g, label, { freqCentre.x + std::sin (angle) * 82.0f,
                                 freqCentre.y - std::cos (angle) * 82.0f }, 20.0f);
    }
}
} // namespace junky
