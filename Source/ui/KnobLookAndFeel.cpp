#include "KnobLookAndFeel.h"
#include "Theme.h"
#include "../Layout.h"
#include "BinaryData.h"

namespace
{
juce::Image createTrimmedKnobImage (const char* data, int dataSize)
{
    const auto source = juce::ImageCache::getFromMemory (data, dataSize);
    if (! source.isValid())
        return {};

    int left = source.getWidth();
    int top = source.getHeight();
    int right = -1;
    int bottom = -1;

    for (int y = 0; y < source.getHeight(); ++y)
        for (int x = 0; x < source.getWidth(); ++x)
            if (source.getPixelAt (x, y).getAlpha() > 1)
            {
                left = juce::jmin (left, x);
                top = juce::jmin (top, y);
                right = juce::jmax (right, x);
                bottom = juce::jmax (bottom, y);
            }

    if (right < left || bottom < top)
        return source;

    const int side = juce::jmax (right - left + 1, bottom - top + 1);
    const int cropX = juce::jlimit (0, source.getWidth() - side,
                                    juce::roundToInt (((float) left + (float) right + 1.0f) * 0.5f
                                                      - (float) side * 0.5f));
    const int cropY = juce::jlimit (0, source.getHeight() - side,
                                    juce::roundToInt (((float) top + (float) bottom + 1.0f) * 0.5f
                                                      - (float) side * 0.5f));
    return source.getClippedImage ({ cropX, cropY, side, side });
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

    const juce::Image* image = nullptr;
    if (relativeWidth >= (layout::largeKnobSize + layout::mediumKnobSize) * 0.5f / layout::designWidth)
    {
        static const auto trimmedLarge = createTrimmedKnobImage (BinaryData::knob_large_png,
                                                                  BinaryData::knob_large_pngSize);
        image = &trimmedLarge;
    }
    else if (relativeWidth >= (layout::mediumKnobSize + layout::smallKnobSize) * 0.5f / layout::designWidth)
    {
        static const auto trimmedMedium = createTrimmedKnobImage (BinaryData::knob_medium_png,
                                                                   BinaryData::knob_medium_pngSize);
        image = &trimmedMedium;
    }
    else
    {
        static const auto trimmedSmall = createTrimmedKnobImage (BinaryData::knob_small_png,
                                                                  BinaryData::knob_small_pngSize);
        image = &trimmedSmall;
    }

    if (image == nullptr || ! image->isValid())
        return;

    const auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height);
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    juce::Graphics::ScopedSaveState state (g);
    g.addTransform (juce::AffineTransform::rotation (angle,
                                                     bounds.getCentreX(), bounds.getCentreY()));
    g.drawImageWithin (*image, x, y, width, height, juce::RectanglePlacement::centred);
}
} // namespace junky
