#include "KnobLookAndFeel.h"
#include "Theme.h"
#include "BinaryData.h"

namespace junky
{
void KnobLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPosProportional, float rotaryStartAngle,
                                        float rotaryEndAngle, juce::Slider&)
{
    const char* data = BinaryData::knob_small_png;
    int dataSize = BinaryData::knob_small_pngSize;
    if (width >= 170 || height >= 170)
    {
        data = BinaryData::knob_large_png;
        dataSize = BinaryData::knob_large_pngSize;
    }
    else if (width >= 110 || height >= 110)
    {
        data = BinaryData::knob_medium_png;
        dataSize = BinaryData::knob_medium_pngSize;
    }

    auto image = juce::ImageCache::getFromMemory (data, dataSize);
    if (! image.isValid()) return;

    const auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height);
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    // El indicador forma parte del mando extraído; giramos la imagen completa alrededor de su centro.
    juce::Graphics::ScopedSaveState state (g);
    g.addTransform (juce::AffineTransform::rotation (angle,
                                                     bounds.getCentreX(), bounds.getCentreY()));
    g.drawImageWithin (image, x, y, width, height, juce::RectanglePlacement::centred);
}
} // namespace junky
