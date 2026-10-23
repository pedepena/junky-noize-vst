#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"

namespace junky
{
/** Boton de panel con LED bicolor, o tecla de flecha. */
class PanelButton : public juce::Button
{
public:
    enum class Style { key, arrowUp, arrowDown };

    explicit PanelButton (Style s = Style::key,
                          juce::Colour off = theme::ledRed,
                          juce::Colour on = theme::ledGreen,
                          bool showLed = true)
        : juce::Button ({}), style (s), ledOffColour (off), ledOnColour (on), hasLed (showLed)
    {
    }

    void paintButton (juce::Graphics& g, bool isMouseOver, bool isButtonDown) override
    {
        auto bounds = getLocalBounds().toFloat().reduced (2.0f);
        const bool lit = getToggleState();
        const float corner = bounds.getWidth() * 0.08f;

        g.setColour (juce::Colours::black.withAlpha (0.4f));
        g.fillRoundedRectangle (bounds.translated (0.0f, 3.0f), corner);

        // El cuerpo conserva el mismo color: el estado se comunica solo con el LED.
        auto face = theme::cream.brighter (isMouseOver ? 0.08f : 0.0f);
        if (isButtonDown)
            face = face.darker (0.2f);

        juce::ColourGradient gradient (face.brighter (0.15f), bounds.getX(), bounds.getY(),
                                       face.darker (0.2f), bounds.getX(), bounds.getBottom(), false);
        g.setGradientFill (gradient);
        g.fillRoundedRectangle (bounds, corner);
        g.setColour (juce::Colours::black.withAlpha (0.55f));
        g.drawRoundedRectangle (bounds, corner, 1.5f);

        if (style == Style::key && hasLed)
        {
            const auto led = juce::Rectangle<float> (bounds.getWidth() * 0.5f, bounds.getHeight() * 0.10f)
                                 .withCentre ({ bounds.getCentreX(), bounds.getY() + bounds.getHeight() * 0.25f });
            const auto colour = lit ? ledOnColour : ledOffColour;

            if (lit)
            {
                g.setColour (colour.withAlpha (0.35f));
                g.fillRoundedRectangle (led.expanded (3.0f), 3.0f);
            }

            g.setColour (colour.withAlpha (lit ? 1.0f : 0.78f));
            g.fillRoundedRectangle (led, 2.0f);
            g.setColour (juce::Colours::black.withAlpha (0.55f));
            g.drawRoundedRectangle (led, 2.0f, 1.0f);
        }
        else if (style != Style::key)
        {
            juce::Path arrow;
            const auto c = bounds.getCentre();
            const float size = bounds.getWidth() * 0.22f;

            if (style == Style::arrowUp)
                arrow.addTriangle (c.x, c.y - size, c.x - size, c.y + size * 0.7f, c.x + size, c.y + size * 0.7f);
            else
                arrow.addTriangle (c.x, c.y + size, c.x - size, c.y - size * 0.7f, c.x + size, c.y - size * 0.7f);

            g.setColour (theme::ink.withAlpha (0.9f));
            g.fillPath (arrow);
        }
    }

private:
    Style style;
    juce::Colour ledOffColour, ledOnColour;
    bool hasLed;
};
} // namespace junky
