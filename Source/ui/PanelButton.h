#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"

namespace junky
{
/** Boton de panel: una tecla cuadrada con un piloto (LED) opcional, o una flecha. */
class PanelButton : public juce::Button
{
public:
    enum class Style { key, arrowUp, arrowDown };

    explicit PanelButton (Style s = Style::key, juce::Colour led = theme::ledRed, bool showLed = true)
        : juce::Button ({}), style (s), ledColour (led), hasLed (showLed)
    {
    }

    void paintButton (juce::Graphics& g, bool isMouseOver, bool isButtonDown) override
    {
        auto bounds = getLocalBounds().toFloat().reduced (2.0f);
        const bool lit = getToggleState();
        const float corner = bounds.getWidth() * 0.08f;

        // Sombra
        g.setColour (juce::Colours::black.withAlpha (0.4f));
        g.fillRoundedRectangle (bounds.translated (0.0f, 3.0f), corner);

        // Cuerpo de la tecla
        const auto base = (style == Style::key && lit) ? juce::Colour (0xff3b3b39) : theme::cream;
        auto face = base.brighter (isMouseOver ? 0.08f : 0.0f);
        if (isButtonDown)
            face = face.darker (0.2f);

        juce::ColourGradient gradient (face.brighter (0.15f), bounds.getX(), bounds.getY(),
                                       face.darker (0.2f), bounds.getX(), bounds.getBottom(), false);
        g.setGradientFill (gradient);
        g.fillRoundedRectangle (bounds, corner);

        g.setColour (juce::Colours::black.withAlpha (0.55f));
        g.drawRoundedRectangle (bounds, corner, 1.5f);

        if (style == Style::key)
        {
            if (hasLed)
            {
                auto led = juce::Rectangle<float> (bounds.getWidth() * 0.5f, bounds.getHeight() * 0.10f)
                               .withCentre ({ bounds.getCentreX(), bounds.getY() + bounds.getHeight() * 0.25f });

                if (lit)
                {
                    g.setColour (ledColour.withAlpha (0.35f));
                    g.fillRoundedRectangle (led.expanded (3.0f), 3.0f);
                }

                g.setColour (lit ? ledColour : ledColour.darker (0.8f).withAlpha (0.7f));
                g.fillRoundedRectangle (led, 2.0f);
            }
        }
        else
        {
            juce::Path arrow;
            const auto c = bounds.getCentre();
            const float s = bounds.getWidth() * 0.22f;

            if (style == Style::arrowUp)
                arrow.addTriangle (c.x, c.y - s, c.x - s, c.y + s * 0.7f, c.x + s, c.y + s * 0.7f);
            else
                arrow.addTriangle (c.x, c.y + s, c.x - s, c.y - s * 0.7f, c.x + s, c.y - s * 0.7f);

            g.setColour (juce::Colour (0xffbfc3bd));
            g.fillPath (arrow);
        }
    }

private:
    Style style;
    juce::Colour ledColour;
    bool hasLed;
};
} // namespace junky
