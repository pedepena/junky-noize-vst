#pragma once

#include <cmath>
#include <juce_gui_basics/juce_gui_basics.h>
#include "Theme.h"
#include "WearTexture.h"

namespace junky
{
/** Boton de panel con LED bicolor, o tecla de flecha. */
class PanelButton : public juce::Button
{
public:
    enum class Style { key, agedKey, arrowUp, arrowDown, folder };

    explicit PanelButton (Style s = Style::key,
                          juce::Colour off = theme::ledRed,
                          juce::Colour on = theme::ledGreen,
                          bool showLed = true, int fixedWearSeed = 4321)
        : juce::Button ({}), style (s), ledOffColour (off), ledOnColour (on), hasLed (showLed), wearSeed (fixedWearSeed)
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

            {
                juce::Graphics::ScopedSaveState wearState (g);
                g.reduceClipRegion (faceBounds.toNearestInt());
                wear::draw (g, faceBounds, wear::Surface::key, wearSeed);
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
            {
                juce::Graphics::ScopedSaveState wearState (g);
                g.reduceClipRegion (bounds.toNearestInt());
                wear::draw (g, bounds, wear::Surface::key, wearSeed);
            }

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
        else if (style == Style::folder)
        {
            juce::ColourGradient gradient (face.brighter (0.15f), bounds.getX(), bounds.getY(),
                                           face.darker (0.2f), bounds.getX(), bounds.getBottom(), false);
            g.setGradientFill (gradient);
            g.fillRoundedRectangle (bounds, corner);
            g.setColour (juce::Colours::black.withAlpha (0.55f));
            g.drawRoundedRectangle (bounds, corner, 1.5f);
            {
                juce::Graphics::ScopedSaveState wearState (g);
                g.reduceClipRegion (bounds.toNearestInt());
                wear::draw (g, bounds, wear::Surface::button, wearSeed);
            }
            const float x = bounds.getCentreX(), y = bounds.getCentreY();
            const float w = bounds.getWidth() * 0.46f, h = bounds.getHeight() * 0.43f;
            juce::Path folder;
            folder.startNewSubPath (x - w * 0.5f, y - h * 0.30f);
            folder.lineTo (x - w * 0.18f, y - h * 0.30f);
            folder.lineTo (x - w * 0.05f, y - h * 0.12f);
            folder.lineTo (x + w * 0.5f, y - h * 0.12f);
            folder.lineTo (x + w * 0.5f, y + h * 0.42f);
            folder.lineTo (x - w * 0.5f, y + h * 0.42f);
            folder.closeSubPath();
            g.setColour (theme::ink);
            g.fillPath (folder);
            g.setColour (theme::ink.darker (0.25f));
            g.strokePath (folder, juce::PathStrokeType (1.2f));
        }
        else
        {
            juce::Path arrow;
            const float sink = isButtonDown ? 1.5f : 0.0f;
            const auto c = bounds.getCentre().translated (0.0f, sink);
            const float size = bounds.getWidth() * 0.22f;

            if (style == Style::arrowUp)
                arrow.addTriangle (c.x, c.y - size, c.x - size, c.y + size * 0.7f, c.x + size, c.y + size * 0.7f);
            else
                arrow.addTriangle (c.x, c.y + size, c.x - size, c.y - size * 0.7f, c.x + size, c.y - size * 0.7f);

            // Las flechas deben seguir claras; no volver a dibujarlas en negro.
            const auto arrowColour = theme::presetArrow.brighter (isMouseOver ? 0.07f : 0.0f);
            g.setColour (arrowColour.withAlpha (isMouseOver ? 0.98f : 0.88f));
            g.fillPath (arrow);

            // La textura se crea una sola vez y se recorta dentro de cada flecha.
            {
                juce::Graphics::ScopedSaveState wearState (g);
                g.reduceClipRegion (arrow);
                wear::draw (g, bounds, wear::Surface::arrow, wearSeed);
            }
        }

        if (isKey && label.isNotEmpty())
            theme::drawPanelText (g, label, bounds.getCentreX(), bounds.getCentreY(), bounds.getHeight() * 0.58f);
    }

private:
    Style style;
    juce::Colour ledOffColour, ledOnColour;
    bool hasLed;
    int wearSeed;
    juce::String label;
};
} // namespace junky
