#include "Phase3Controls.h"
#include "../Layout.h"
#include "BinaryData.h"

namespace junky
{
ImageToggleButton::ImageToggleButton()
    : juce::Button ("Bass Boost"),
      image (juce::ImageCache::getFromMemory (BinaryData::btn_bass_png, BinaryData::btn_bass_pngSize))
{
}

void ImageToggleButton::paintButton (juce::Graphics& g, bool, bool isButtonDown)
{
    const float scale = getWidth() / layout::bassButton.width;
    auto bounds = getLocalBounds().toFloat();
    if (isButtonDown)
        bounds = bounds.reduced (layout::bassButtonPressInset * scale);
    if (image.isValid())
        g.drawImageWithin (image, juce::roundToInt (bounds.getX()), juce::roundToInt (bounds.getY()),
                           juce::roundToInt (bounds.getWidth()), juce::roundToInt (bounds.getHeight()),
                           juce::RectanglePlacement::centred);
}

ImageLed::ImageLed()
    : onImage (juce::ImageCache::getFromMemory (BinaryData::led_on_png, BinaryData::led_on_pngSize)),
      offImage (juce::ImageCache::getFromMemory (BinaryData::led_off_png, BinaryData::led_off_pngSize))
{
}

void ImageLed::setOn (bool shouldBeOn)
{
    if (isOn == shouldBeOn)
        return;
    isOn = shouldBeOn;
    repaint();
}

void ImageLed::paint (juce::Graphics& g)
{
    const auto& image = isOn ? onImage : offImage;
    if (image.isValid())
        g.drawImageWithin (image, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::centred);
}

MonoStereoLever::MonoStereoLever()
    : image (juce::ImageCache::getFromMemory (BinaryData::switch_lever_png,
                                               BinaryData::switch_lever_pngSize)),
      currentCentreX (layout::switchLeverStereoCentreX - layout::switchTrackStartX),
      animationStartX (currentCentreX),
      animationTargetX (currentCentreX)
{
    setMouseCursor (juce::MouseCursor::PointingHandCursor);
}

void MonoStereoLever::setMono (bool shouldBeMono)
{
    if (targetMono == shouldBeMono)
        return;

    targetMono = shouldBeMono;
    animationStartX = currentCentreX;
    animationTargetX = (targetMono ? layout::switchLeverMonoCentreX : layout::switchLeverStereoCentreX)
                       - layout::switchTrackStartX;
    animationElapsedMs = 0;
    startTimerHz (60);
}

void MonoStereoLever::paint (juce::Graphics& g)
{
    if (! image.isValid())
        return;

    const float scale = getWidth() / (layout::switchTrackEndX - layout::switchTrackStartX);
    const float leverWidth = layout::switchLeverWidth * scale;
    const float leverHeight = layout::switchLeverHeight * scale;
    const auto leverBounds = juce::Rectangle<float> (leverWidth, leverHeight)
        .withCentre ({ currentCentreX * scale, getHeight() * 0.5f });
    g.drawImageWithin (image, juce::roundToInt (leverBounds.getX()), juce::roundToInt (leverBounds.getY()),
                       juce::roundToInt (leverBounds.getWidth()), juce::roundToInt (leverBounds.getHeight()),
                       juce::RectanglePlacement::centred);
}

void MonoStereoLever::mouseUp (const juce::MouseEvent&)
{
    setMono (! targetMono);
    if (onToggle)
        onToggle (targetMono);
}

void MonoStereoLever::timerCallback()
{
    animationElapsedMs = juce::jmin (layout::switchAnimationDurationMs, animationElapsedMs + 17);
    const float linearProgress = (float) animationElapsedMs / (float) layout::switchAnimationDurationMs;
    const float smoothProgress = linearProgress * linearProgress * (3.0f - 2.0f * linearProgress);
    currentCentreX = animationStartX + (animationTargetX - animationStartX) * smoothProgress;
    repaint();

    if (animationElapsedMs >= layout::switchAnimationDurationMs)
        stopTimer();
}
} // namespace junky