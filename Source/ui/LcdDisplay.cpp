#include "LcdDisplay.h"
#include "Theme.h"
#include "WearTexture.h"

namespace junky
{
void LcdDisplay::setHeader (const juce::String& text, int durationMs)
{
    header = text; repaint();
    if (durationMs > 0) startTimer (durationMs); else stopTimer();
}
void LcdDisplay::timerCallback() { stopTimer(); header = "PRESET SELECT"; repaint(); }
void LcdDisplay::setItems (const juce::StringArray& newItems)
{
    items = newItems;
    selected = juce::jlimit (0, juce::jmax (0, items.size() - 1), selected);
    ensureVisible();
    repaint();
}

void LcdDisplay::setSelected (int index)
{
    index = juce::jlimit (0, juce::jmax (0, items.size() - 1), index);

    if (index != selected)
    {
        selected = index;
        ensureVisible();
        repaint();
    }
}

void LcdDisplay::ensureVisible()
{
    if (selected < offset)
        offset = selected;
    else if (selected >= offset + visibleRows)
        offset = selected - visibleRows + 1;

    offset = juce::jlimit (0, juce::jmax (0, items.size() - visibleRows), offset);
}

void LcdDisplay::choose (int index)
{
    index = juce::jlimit (0, juce::jmax (0, items.size() - 1), index);
    setSelected (index);

    if (onSelect != nullptr)
        onSelect (index);
}

void LcdDisplay::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    juce::ColourGradient background (theme::lcdBackground.brighter (0.12f), bounds.getCentreX(), bounds.getY(),
                                     theme::lcdBackground.darker (0.15f), bounds.getCentreX(), bounds.getBottom(), false);
    g.setGradientFill (background);
    g.fillRect (bounds);

    g.setColour (theme::lcdText.withAlpha (0.8f));
    g.drawRect (bounds, 2.0f);

    const float headerHeight = bounds.getHeight() / 9.0f;
    const float rowHeight = (bounds.getHeight() - headerHeight) / (float) visibleRows;
    const float fontSize = rowHeight * 0.72f;
    const float padding = bounds.getWidth() * 0.025f;

    // Matriz de pixeles fija: la fuente no depende del sistema operativo.
    g.setColour (theme::lcdText);
    theme::drawBitmapText (g, header,
                           juce::Rectangle<float> (padding * 2.0f, 0.0f,
                                                   bounds.getWidth() - padding * 4.0f, headerHeight), fontSize);
    g.fillRect (padding, headerHeight - 2.0f, bounds.getWidth() - padding * 2.0f, 2.0f);

    const float scrollWidth = bounds.getWidth() * 0.05f;
    const juce::Rectangle<float> track (bounds.getRight() - scrollWidth - padding, headerHeight + 4.0f,
                                        scrollWidth, bounds.getHeight() - headerHeight - 8.0f);
    g.setColour (theme::lcdText.withAlpha (0.25f));
    g.fillRect (track);
    g.setColour (theme::lcdText.withAlpha (0.85f));

    const float total = (float) juce::jmax (items.size(), visibleRows);
    const float thumbHeight = track.getHeight() * (float) visibleRows / total;
    const float thumbY = track.getY() + track.getHeight() * (float) offset / total;
    g.fillRect (track.getX(), thumbY, track.getWidth(), thumbHeight);

    for (int row = 0; row < visibleRows; ++row)
    {
        const int index = offset + row;
        if (index >= items.size())
            break;

        const juce::Rectangle<float> rowBounds (padding, headerHeight + 2.0f + (float) row * rowHeight,
                                                track.getX() - padding * 2.0f, rowHeight);
        const bool isSelected = index == selected;

        if (isSelected)
        {
            g.setColour (theme::lcdText);
            g.fillRect (rowBounds.withTrimmedLeft (rowHeight * 0.9f));

            juce::Path marker;
            const float markerSize = rowHeight * 0.28f;
            marker.addTriangle (rowBounds.getX() + markerSize * 0.5f, rowBounds.getCentreY() - markerSize,
                                rowBounds.getX() + markerSize * 0.5f, rowBounds.getCentreY() + markerSize,
                                rowBounds.getX() + markerSize * 2.2f, rowBounds.getCentreY());
            g.fillPath (marker);
        }

        g.setColour (isSelected ? theme::lcdBackground : theme::lcdText);
        theme::drawBitmapText (g, juce::String (index).paddedLeft ('0', 2) + "  " + items[index],
                               rowBounds.withTrimmedLeft (rowHeight * 1.0f), fontSize);
    }

    // Cristal con polvo, rayas finas y esquinas oscurecidas; el texto sigue nítido.
    {
        juce::Graphics::ScopedSaveState wearState (g);
        g.reduceClipRegion (getLocalBounds());
        wear::draw (g, bounds, wear::Surface::lcd, 7139);
    }
}

void LcdDisplay::mouseDown (const juce::MouseEvent& e)
{
    const float headerHeight = (float) getHeight() / 9.0f;
    const float rowHeight = ((float) getHeight() - headerHeight) / (float) visibleRows;

    if ((float) e.y < headerHeight)
        return;

    const int row = (int) (((float) e.y - headerHeight) / rowHeight);
    const int index = offset + row;

    if (index >= 0 && index < items.size())
        choose (index);
}

void LcdDisplay::mouseWheelMove (const juce::MouseEvent&, const juce::MouseWheelDetails& wheel)
{
    if (wheel.deltaY > 0.0f)
        choose (selected - 1);
    else if (wheel.deltaY < 0.0f)
        choose (selected + 1);
}
} // namespace junky
