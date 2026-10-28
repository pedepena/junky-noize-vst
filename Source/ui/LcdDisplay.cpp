#include "LcdDisplay.h"
#include "Theme.h"
#include "../Layout.h"

namespace junky
{
namespace
{
const juce::Colour lcdText (0xff8fe07a);
const juce::Colour lcdSelection (0xff8fe07a);
const juce::Colour lcdSelectedText (0xff10200c);
}

void LcdDisplay::setHeader (const juce::String& text, int durationMs)
{
    header = text;
    repaint();
    if (durationMs > 0) startTimer (durationMs);
    else stopTimer();
}

void LcdDisplay::timerCallback()
{
    stopTimer();
    header.clear();
    repaint();
}

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
    if (selected < offset) offset = selected;
    else if (selected >= offset + visibleRows) offset = selected - visibleRows + 1;
    offset = juce::jlimit (0, juce::jmax (0, items.size() - visibleRows), offset);
}

void LcdDisplay::choose (int index)
{
    index = juce::jlimit (0, juce::jmax (0, items.size() - 1), index);
    setSelected (index);
    if (onSelect != nullptr && ! items.isEmpty()) onSelect (index);
}

void LcdDisplay::selectRelative (int delta)
{
    if (items.isEmpty()) return;
    const int count = items.size();
    const int next = (selected + (delta % count) + count) % count;
    choose (next);
}

void LcdDisplay::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    const float scale = bounds.getWidth() / layout::presetLcdWidth;
    const float margin = layout::presetLcdMargin * scale;
    const float headerHeight = layout::presetLcdHeaderHeight * scale;
    const float rowHeight = (bounds.getHeight() - margin * 2.0f - headerHeight) / (float) visibleRows;
    const float right = bounds.getRight() - margin;
    const float trackX = right - layout::presetLcdScrollWidth * scale;
    const juce::Rectangle<float> screen = bounds;

    juce::ColourGradient glass (juce::Colour (0xff375d34), bounds.getCentreX(), bounds.getY(),
                                juce::Colour (0xff20391e), bounds.getCentreX(), bounds.getBottom(), false);
    g.setGradientFill (glass);
    g.fillRect (screen);

    // Rayas de escaneo casi imperceptibles, recortadas al cristal.
    g.setColour (juce::Colour (0xffc2f1a5).withAlpha (0.025f));
    for (float y = layout::presetLcdScanlineStart * scale; y < bounds.getHeight();
         y += layout::presetLcdScanlinePitch * scale)
        g.fillRect (bounds.getX(), y, bounds.getWidth(), layout::presetLcdScanlineHeight * scale);

    const auto headerArea = juce::Rectangle<float> (margin, margin,
        trackX - layout::presetLcdScrollGap * scale, headerHeight);
    if (header.isNotEmpty())
    {
        g.setColour (lcdText.withAlpha (0.26f));
        theme::drawBitmapText (g, header, headerArea.translated (0.0f, layout::presetLcdGlowOffset * scale), headerHeight * 0.72f);
        g.setColour (lcdText);
        theme::drawBitmapText (g, header, headerArea, headerHeight * 0.72f);
    }

    const float listY = margin + headerHeight;
    const juce::Rectangle<float> track (trackX, listY, layout::presetLcdScrollWidth * scale, rowHeight * (float) visibleRows);
    g.setColour (lcdText.withAlpha (0.13f));
    g.fillRect (track);

    const int totalItems = items.size();
    if (totalItems > 0)
    {
        const float thumbHeight = track.getHeight() * (float) visibleRows / (float) juce::jmax (totalItems, visibleRows);
        const int maxOffset = juce::jmax (1, totalItems - visibleRows);
        const float thumbY = track.getY() + (track.getHeight() - thumbHeight) * (float) offset / (float) maxOffset;
        g.setColour (lcdText.withAlpha (0.8f));
        g.fillRoundedRectangle (track.getX(), thumbY, track.getWidth(), thumbHeight, track.getWidth() * 0.5f);
    }

    for (int row = 0; row < visibleRows; ++row)
    {
        const int index = offset + row;
        if (index >= items.size()) break;

        const juce::Rectangle<float> rowBounds (margin, listY + (float) row * rowHeight,
                                                  trackX - margin - layout::presetLcdScrollGap * scale, rowHeight);
        const bool isSelected = index == selected;
        if (isSelected)
        {
            g.setColour (lcdSelection);
            g.fillRoundedRectangle (rowBounds, layout::presetLcdSelectionCorner * scale);
        }

        const auto label = juce::String (index + 1) + " " + items[index];
        const auto textArea = rowBounds.reduced (layout::presetLcdTextInset * scale, 0.0f);
        if (! isSelected)
        {
            g.setColour (lcdText.withAlpha (0.18f));
            theme::drawBitmapText (g, label, textArea.translated (0.0f, layout::presetLcdGlowOffset * scale), rowHeight * 0.76f);
        }
        g.setColour (isSelected ? lcdSelectedText : lcdText);
        theme::drawBitmapText (g, label, textArea, rowHeight * 0.76f);
    }
}

void LcdDisplay::mouseDown (const juce::MouseEvent& e)
{
    const float scale = (float) getWidth() / layout::presetLcdWidth;
    const float margin = layout::presetLcdMargin * scale;
    const float headerHeight = layout::presetLcdHeaderHeight * scale;
    const float rowHeight = ((float) getHeight() - margin * 2.0f - headerHeight) / (float) visibleRows;
    const float listY = margin + headerHeight;
    if ((float) e.y < listY || rowHeight <= 0.0f) return;
    const int row = (int) (((float) e.y - listY) / rowHeight);
    const int index = offset + row;
    if (row >= 0 && row < visibleRows && index >= 0 && index < items.size()) choose (index);
}

void LcdDisplay::mouseWheelMove (const juce::MouseEvent&, const juce::MouseWheelDetails& wheel)
{
    if (wheel.deltaY > 0.0f) selectRelative (-1);
    else if (wheel.deltaY < 0.0f) selectRelative (1);
}
} // namespace junky
