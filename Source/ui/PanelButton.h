#pragma once

#include <cmath>
#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"

namespace junky
{
/** Boton de panel con LED bicolor, o tecla de flecha. */
class PanelButton : public juce::Button
{
public:
    enum class Style { key, agedKey, arrowUp, arrowDown };

    explicit PanelButton (Style s = Style::key,
                          juce::Colour off = theme::ledRed,
                          juce::Colour on = theme::ledGreen,
                          bool showLed = true)
        : juce::Button ({}), style (s), ledOffColour (off), ledOnColour (on), hasLed (showLed)
    {
    }

    void setLabel (const juce::String& text) { label = text; repaint(); }

    void paintButton (juce::Graphics& g, bool isMouseOver, bool isButtonDown) override
    {
        auto bounds = getLocalBounds().toFloat().reduced (2.0f);
        const bool lit = getToggleState();
        const float corner = bounds.getWidth() * 0.08f;
        const bool isKey = style == Style::key || style == Style::agedKey;
        const auto face = isButtonDown ? theme::cream.darker (0.2f)
                                       : theme::cream.brighter (isMouseOver ? 0.08f : 0.0f);

        g.setColour (juce::Colours::black.withAlpha (0.4f));
        g.fillRoundedRectangle (bounds.translated (0.0f, 3.0f), corner);

        if (style == Style::agedKey)
        {
            const auto agedBounds = getLocalBounds().toFloat();
            const float w = agedBounds.getWidth(), h = agedBounds.getHeight();
            const float frameCorner = 0.09f * w;
            g.setColour (juce::Colour (0xff1b1b1a));
            g.fillRoundedRectangle (agedBounds, frameCorner);
            g.setColour (juce::Colour (0xff0a0a0a));
            g.drawRoundedRectangle (agedBounds, frameCorner, 1.5f);
            g.setColour (juce::Colour (0xff3a3a38));
            g.drawRoundedRectangle (agedBounds.reduced (2.0f), juce::jmax (0.0f, frameCorner - 2.0f), 1.0f);

            const auto faceBounds = agedBounds.reduced (0.11f * w, 0.11f * w);
            juce::ColourGradient gradient (face.brighter (0.05f), faceBounds.getX(), faceBounds.getY(),
                                           face.darker (0.25f), faceBounds.getX(), faceBounds.getBottom(), false);
            g.setGradientFill (gradient);
            g.fillRoundedRectangle (faceBounds, 0.07f * w);

            juce::Random wear (4321);
            {
                juce::Graphics::ScopedSaveState wearState (g);
                g.reduceClipRegion (faceBounds.toNearestInt());
                for (int i = 0; i < 16; ++i)
            {
                const float x = faceBounds.getX() + wear.nextFloat() * faceBounds.getWidth();
                const float y = faceBounds.getY() + wear.nextFloat() * faceBounds.getHeight();
                const float length = w * (0.12f + 0.33f * wear.nextFloat());
                const float angle = wear.nextFloat() * juce::MathConstants<float>::twoPi;
                g.setColour (juce::Colour (0xff5b5648).withAlpha (0.30f));
                g.drawLine (x, y, x + std::cos (angle) * length, y + std::sin (angle) * length, 1.0f);
            }
                for (int i = 0; i < 45; ++i)
            {
                const float size = 1.0f + 2.0f * wear.nextFloat();
                const float x = faceBounds.getX() + wear.nextFloat() * (faceBounds.getWidth() - size);
                const float y = faceBounds.getY() + wear.nextFloat() * (faceBounds.getHeight() - size);
                g.setColour (juce::Colour (0xff7a5b3a).withAlpha (0.30f));
                g.fillEllipse (x, y, size, size);
            }
                for (int i = 0; i < 6; ++i)
            {
                    const float along = wear.nextFloat();
                    const float jitter = (wear.nextFloat() - 0.5f) * 0.08f * w;
                    const float size = 3.0f + 3.0f * wear.nextFloat();
                    float x = faceBounds.getX(), y = faceBounds.getY();
                    switch (i)
                {
                    case 0: x += jitter; y += along * 0.08f * h; break;
                    case 1: x = faceBounds.getRight() - size + jitter; y += along * faceBounds.getHeight(); break;
                    case 2: x += along * faceBounds.getWidth(); y += jitter; break;
                    case 3: x += along * faceBounds.getWidth(); y = faceBounds.getBottom() - size + jitter; break;
                    case 4: x += jitter; y = faceBounds.getBottom() - size + along * 0.08f * h; break;
                    default: x = faceBounds.getRight() - size + jitter; y = faceBounds.getBottom() - size + jitter; break;
                }
                    x = juce::jlimit (faceBounds.getX(), faceBounds.getRight() - size, x);
                    y = juce::jlimit (faceBounds.getY(), faceBounds.getBottom() - size, y);
                    g.setColour (juce::Colour (0xff6e4a2a).withAlpha (0.18f));
                    g.fillEllipse (x, y, size, size);
                }
            }

            g.setColour (juce::Colour (0xff8a8571).withAlpha (0.5f));
            g.drawLine (faceBounds.getRight(), faceBounds.getY(), faceBounds.getRight(),
                        faceBounds.getBottom(), 1.5f);
            g.drawLine (faceBounds.getX(), faceBounds.getBottom(), faceBounds.getRight(),
                        faceBounds.getBottom(), 1.5f);

            if (hasLed)
            {
                const auto slot = juce::Rectangle<float> (w * 0.56f, h * 0.14f)
                                      .withCentre ({ agedBounds.getCentreX(), agedBounds.getY() + h * 0.25f });
                g.setColour (juce::Colour (0xff23231f));
                g.fillRoundedRectangle (slot, slot.getHeight() * 0.22f);
                const auto led = juce::Rectangle<float> (w * 0.5f, h * 0.10f)
                                     .withCentre ({ agedBounds.getCentreX(), agedBounds.getY() + h * 0.25f });
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
        }
        else if (isKey)
        {
            juce::ColourGradient gradient (face.brighter (0.15f), bounds.getX(), bounds.getY(),
                                           face.darker (0.2f), bounds.getX(), bounds.getBottom(), false);
            g.setGradientFill (gradient);
            g.fillRoundedRectangle (bounds, corner);
            g.setColour (juce::Colours::black.withAlpha (0.55f));
            g.drawRoundedRectangle (bounds, corner, 1.5f);

            if (hasLed)
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
        }
        else
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

        if (isKey && label.isNotEmpty())
            theme::drawPanelText (g, label, bounds.getCentreX(), bounds.getCentreY(), bounds.getHeight() * 0.58f);
    }

private:
    Style style;
    juce::Colour ledOffColour, ledOnColour;
    bool hasLed;
    juce::String label;
};
} // namespace junky
