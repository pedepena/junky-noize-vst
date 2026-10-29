#include "KnobLookAndFeel.h"
#include "Theme.h"
#include "../Layout.h"
#include "BinaryData.h"

namespace
{
juce::Image createKnobImage (const char* data, int dataSize)
{
    return juce::ImageCache::getFromMemory (data, dataSize);
}
} // namespace

namespace junky
{
void KnobLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPosProportional, float rotaryStartAngle,
                                        float rotaryEndAngle, juce::Slider& slider)
{
    const float relativeWidth = slider.getParentWidth() > 0
        ? (float) width / (float) slider.getParentWidth() : 0.0f;

    const juce::Image* base = nullptr;
    const juce::Image* cap = nullptr;
    juce::Point<float> capCentreInBase;
    if (relativeWidth >= (layout::largeKnobSize + layout::mediumKnobSize) * 0.5f / layout::designWidth)
    {
        static const auto largeBase = createKnobImage (BinaryData::knob_large_base_png,
                                                        BinaryData::knob_large_base_pngSize);
        static const auto largeCap = createKnobImage (BinaryData::knob_large_cap_png,
                                                       BinaryData::knob_large_cap_pngSize);
        base = &largeBase;
        cap = &largeCap;
        capCentreInBase = { 487.0f, 473.0f };
    }
    else if (relativeWidth >= (layout::mediumKnobSize + layout::smallKnobSize) * 0.5f / layout::designWidth)
    {
        static const auto mediumBase = createKnobImage (BinaryData::knob_medium_base_png,
                                                         BinaryData::knob_medium_base_pngSize);
        static const auto mediumCap = createKnobImage (BinaryData::knob_medium_cap_png,
                                                        BinaryData::knob_medium_cap_pngSize);
        base = &mediumBase;
        cap = &mediumCap;
        capCentreInBase = { 432.0f, 424.0f };
    }
    else
    {
        static const auto smallBase = createKnobImage (BinaryData::knob_small_base_png,
                                                        BinaryData::knob_small_base_pngSize);
        static const auto smallCap = createKnobImage (BinaryData::knob_small_cap_png,
                                                       BinaryData::knob_small_cap_pngSize);
        base = &smallBase;
        cap = &smallCap;
        capCentreInBase = { 293.0f, 288.0f };
    }

    if (! base->isValid() || ! cap->isValid())
        return;

    const auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height);
    const float diameter = juce::jmin (bounds.getWidth(), bounds.getHeight());
    const float scale = diameter / (float) base->getWidth();
    const float baseWidth = (float) base->getWidth() * scale;
    const float baseHeight = (float) base->getHeight() * scale;
    const float baseX = bounds.getCentreX() - baseWidth * 0.5f;
    const float baseY = bounds.getCentreY() - baseHeight * 0.5f;
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    g.setImageResamplingQuality (juce::Graphics::highResamplingQuality);
    g.drawImage (*base, juce::Rectangle<float> (baseX, baseY, baseWidth, baseHeight),
                 juce::RectanglePlacement::stretchToFit);

    const float capWidth = (float) cap->getWidth() * scale;
    const float capHeight = (float) cap->getHeight() * scale;
    const juce::Point<float> capCentre (baseX + capCentreInBase.x * scale,
                                        baseY + capCentreInBase.y * scale);
    juce::Graphics::ScopedSaveState state (g);
    g.addTransform (juce::AffineTransform::rotation (angle, capCentre.x, capCentre.y));
    g.drawImage (*cap, juce::Rectangle<float> (capCentre.x - capWidth * 0.5f,
                                                capCentre.y - capHeight * 0.5f,
                                                capWidth, capHeight),
                 juce::RectanglePlacement::stretchToFit);
}
} // namespace junky
