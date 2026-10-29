#include "LcdDisplay.h"
#include "../Layout.h"
#include <array>
#include <cstdint>

namespace junky
{
namespace
{
const juce::Colour lcdText (0xff8fe07a);
const juce::Colour lcdSelection (0xff8fe07a);
const juce::Colour lcdSelectedText (0xff10200c);

std::array<std::uint8_t, 7> glyphFor (juce::juce_wchar character)
{
    switch (juce::CharacterFunctions::toUpperCase (character))
    {
        case 'A': return { 14, 17, 17, 31, 17, 17, 17 };
        case 'B': return { 30, 17, 17, 30, 17, 17, 30 };
        case 'C': return { 14, 17, 16, 16, 16, 17, 14 };
        case 'D': return { 30, 17, 17, 17, 17, 17, 30 };
        case 'E': return { 31, 16, 16, 30, 16, 16, 31 };
        case 'F': return { 31, 16, 16, 30, 16, 16, 16 };
        case 'G': return { 14, 17, 16, 23, 17, 17, 15 };
        case 'H': return { 17, 17, 17, 31, 17, 17, 17 };
        case 'I': return { 14, 4, 4, 4, 4, 4, 14 };
        case 'J': return { 7, 2, 2, 2, 2, 18, 12 };
        case 'K': return { 17, 18, 20, 24, 20, 18, 17 };
        case 'L': return { 16, 16, 16, 16, 16, 16, 31 };
        case 'M': return { 17, 27, 21, 21, 17, 17, 17 };
        case 'N': return { 17, 25, 21, 19, 17, 17, 17 };
        case 'O': return { 14, 17, 17, 17, 17, 17, 14 };
        case 'P': return { 30, 17, 17, 30, 16, 16, 16 };
        case 'Q': return { 14, 17, 17, 17, 21, 18, 13 };
        case 'R': return { 30, 17, 17, 30, 20, 18, 17 };
        case 'S': return { 15, 16, 16, 14, 1, 1, 30 };
        case 'T': return { 31, 4, 4, 4, 4, 4, 4 };
        case 'U': return { 17, 17, 17, 17, 17, 17, 14 };
        case 'V': return { 17, 17, 17, 17, 17, 10, 4 };
        case 'W': return { 17, 17, 17, 21, 21, 21, 10 };
        case 'X': return { 17, 17, 10, 4, 10, 17, 17 };
        case 'Y': return { 17, 17, 10, 4, 4, 4, 4 };
        case 'Z': return { 31, 1, 2, 4, 8, 16, 31 };
        case '0': return { 14, 17, 19, 21, 25, 17, 14 };
        case '1': return { 4, 12, 4, 4, 4, 4, 14 };
        case '2': return { 14, 17, 1, 2, 4, 8, 31 };
        case '3': return { 30, 1, 1, 14, 1, 1, 30 };
        case '4': return { 2, 6, 10, 18, 31, 2, 2 };
        case '5': return { 31, 16, 16, 30, 1, 1, 30 };
        case '6': return { 14, 16, 16, 30, 17, 17, 14 };
        case '7': return { 31, 1, 2, 4, 8, 8, 8 };
        case '8': return { 14, 17, 17, 14, 17, 17, 14 };
        case '9': return { 14, 17, 17, 15, 1, 1, 14 };
        case '-': return { 0, 0, 0, 31, 0, 0, 0 };
        case '_': return { 0, 0, 0, 0, 0, 0, 31 };
        case '.': return { 0, 0, 0, 0, 0, 12, 12 };
        case '/': return { 1, 2, 2, 4, 8, 8, 16 };
        case '+': return { 0, 4, 4, 31, 4, 4, 0 };
        case '#': return { 10, 31, 10, 10, 31, 10, 0 };
        default:  return {};
    }
}

void drawDotText (juce::Graphics& g, const juce::String& text, juce::Rectangle<float> area,
                  juce::Colour colour)
{
    if (text.isEmpty() || area.isEmpty())
        return;

    const float unit = juce::jmin (area.getHeight() / 8.0f,
                                   area.getWidth() / (6.0f * (float) text.length()));
    const float dot = unit * 0.78f;
    const float textWidth = unit * 6.0f * (float) text.length() - unit;
    const float startX = area.getX() + (area.getWidth() - textWidth) * 0.5f;
    const float startY = area.getY() + (area.getHeight() - unit * 7.0f) * 0.5f;

    g.setColour (colour);
    for (int characterIndex = 0; characterIndex < text.length(); ++characterIndex)
    {
        const auto glyph = glyphFor (text[characterIndex]);
        for (int row = 0; row < 7; ++row)
            for (int column = 0; column < 5; ++column)
                if ((glyph[(size_t) row] & (1u << (4 - column))) != 0)
                    g.fillRect (juce::Rectangle<float> (startX + (float) characterIndex * 6.0f * unit + (float) column * unit,
                                                        startY + (float) row * unit, dot, dot));
    }
}
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
        drawDotText (g, header.toUpperCase(),
                     headerArea.translated (0.0f, layout::presetLcdGlowOffset * scale),
                     lcdText.withAlpha (0.26f));
        drawDotText (g, header.toUpperCase(), headerArea, lcdText);
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
            drawDotText (g, label.toUpperCase(),
                         textArea.translated (0.0f, layout::presetLcdGlowOffset * scale),
                         lcdText.withAlpha (0.18f));
        }
        drawDotText (g, label.toUpperCase(), textArea, isSelected ? lcdSelectedText : lcdText);
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
