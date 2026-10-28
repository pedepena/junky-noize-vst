#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace junky
{
/** Botón que presenta una imagen del panel y se comprime ligeramente al pulsarlo. */
class AssetButton : public juce::Button
{
public:
    explicit AssetButton() : juce::Button ({}) {}

    void setImageData (const void* data, int size)
    {
        image = juce::ImageCache::getFromMemory (data, size);
        repaint();
    }

    void paintButton (juce::Graphics& g, bool, bool isButtonDown) override
    {
        if (! image.isValid()) return;
        auto bounds = getLocalBounds().toFloat();
        if (isButtonDown) bounds = bounds.withSizeKeepingCentre (bounds.getWidth() * 0.96f, bounds.getHeight() * 0.96f);
        g.setOpacity (1.0f);
        g.drawImageWithin (image, juce::roundToInt (bounds.getX()), juce::roundToInt (bounds.getY()),
                           juce::roundToInt (bounds.getWidth()), juce::roundToInt (bounds.getHeight()),
                           juce::RectanglePlacement::centred);
    }

private:
    juce::Image image;
};
} // namespace junky
