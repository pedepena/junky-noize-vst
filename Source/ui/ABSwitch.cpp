#include "ABSwitch.h"
#include "Theme.h"

namespace junky
{
void ABSwitch::paint (juce::Graphics& g)
{
    auto area = getLocalBounds().toFloat();
    g.setColour (juce::Colour (0xff202120));
    g.fillRoundedRectangle (area, 5.0f);
    for (int i = 0; i < 2; ++i)
    {
        const float x = 5.0f + (float) i * 70.0f;
        const bool active = (i == 1) == activeB;
        juce::Rectangle<float> key (x, 5.0f, 67.0f, 40.0f);
        g.setColour (active ? theme::cream.brighter (0.1f) : theme::cream.darker (0.08f));
        g.fillRoundedRectangle (key, 4.0f);
        g.setColour (theme::ink);
        g.drawRoundedRectangle (key, 4.0f, 1.0f);
        theme::drawBitmapText (g, i == 0 ? "A" : "B", { x + 28.0f, 7.0f, 12.0f, 17.0f }, 12.0f);
        const auto led = juce::Rectangle<float> (11.0f, 11.0f).withCentre ({ x + 33.5f, 36.0f });
        g.setColour (active ? theme::ledRed : juce::Colour (0xff3a0f0c));
        g.fillEllipse (led);
        g.setColour (juce::Colours::black.withAlpha (0.65f));
        g.drawEllipse (led, 1.0f);
    }
}

void ABSwitch::mouseDown (const juce::MouseEvent& e)
{
    const int slot = e.x < getWidth() / 2 ? 0 : 1;
    if ((slot == 1) != activeB && onSelect != nullptr)
        onSelect (slot);
}
} // namespace junky
