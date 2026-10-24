#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <juce_gui_basics/juce_gui_basics.h>

namespace junky::theme
{
// Lienzo de referencia rasterizado a estas medidas para mantener fija la interfaz.
inline constexpr float designWidth = 1085.0f;
inline constexpr float designHeight = 1182.0f;

inline const juce::Colour metalLight { 0xffb9bbb7 };
inline const juce::Colour metalMid { 0xff9a9c98 };
inline const juce::Colour metalDark { 0xff5d5f5c };
inline const juce::Colour ink { 0xff161616 };
inline const juce::Colour rust { 0xff8c4a20 };
inline const juce::Colour cream { 0xffd9d4bd };
inline const juce::Colour lcdBackground { 0xff7fc46b };
inline const juce::Colour lcdText { 0xff17240f };
inline const juce::Colour ledRed { 0xffff3b2f };
inline const juce::Colour ledGreen { 0xff62e06a };

inline juce::Font panelFont (float height)
{
    return juce::Font (juce::FontOptions ("Arial", height, juce::Font::bold));
}

inline void drawPanelText (juce::Graphics& g, const juce::String& text,
                           float centreX, float centreY, float height,
                           juce::Justification justification = juce::Justification::centred)
{
    g.setColour (ink);
    g.setFont (panelFont (height));
    g.drawText (text, juce::Rectangle<float> (400.0f, height * 1.5f).withCentre ({ centreX, centreY }),
                justification, false);
}

inline std::array<std::uint8_t, 7> bitmapGlyph (juce::juce_wchar character)
{
    using Rows = std::array<std::uint8_t, 7>;
    switch (character)
    {
        case 'A': return Rows { 14, 17, 17, 31, 17, 17, 17 }; case 'a': return Rows { 0, 0, 14, 1, 15, 17, 15 };
        case 'B': return Rows { 30, 17, 17, 30, 17, 17, 30 }; case 'b': return Rows { 16, 16, 30, 17, 17, 17, 30 };
        case 'C': return Rows { 14, 17, 16, 16, 16, 17, 14 }; case 'c': return Rows { 0, 0, 14, 17, 16, 17, 14 };
        case 'D': return Rows { 30, 17, 17, 17, 17, 17, 30 }; case 'd': return Rows { 1, 1, 15, 17, 17, 17, 15 };
        case 'E': return Rows { 31, 16, 16, 30, 16, 16, 31 }; case 'e': return Rows { 0, 0, 14, 17, 31, 16, 14 };
        case 'F': return Rows { 31, 16, 16, 30, 16, 16, 16 }; case 'f': return Rows { 6, 9, 8, 28, 8, 8, 8 };
        case 'G': return Rows { 14, 17, 16, 23, 17, 17, 15 }; case 'g': return Rows { 0, 15, 17, 17, 15, 1, 14 };
        case 'H': return Rows { 17, 17, 17, 31, 17, 17, 17 }; case 'h': return Rows { 16, 16, 30, 17, 17, 17, 17 };
        case 'I': return Rows { 14, 4, 4, 4, 4, 4, 14 }; case 'i': return Rows { 4, 0, 12, 4, 4, 4, 14 };
        case 'J': return Rows { 7, 2, 2, 2, 18, 18, 12 }; case 'j': return Rows { 2, 0, 6, 2, 2, 18, 12 };
        case 'K': return Rows { 17, 18, 20, 24, 20, 18, 17 }; case 'k': return Rows { 16, 16, 18, 20, 24, 20, 18 };
        case 'L': return Rows { 16, 16, 16, 16, 16, 16, 31 }; case 'l': return Rows { 12, 4, 4, 4, 4, 4, 14 };
        case 'M': return Rows { 17, 27, 21, 21, 17, 17, 17 }; case 'm': return Rows { 0, 0, 26, 21, 21, 21, 21 };
        case 'N': return Rows { 17, 25, 25, 21, 19, 19, 17 }; case 'n': return Rows { 0, 0, 30, 17, 17, 17, 17 };
        case 'O': return Rows { 14, 17, 17, 17, 17, 17, 14 }; case 'o': return Rows { 0, 0, 14, 17, 17, 17, 14 };
        case 'P': return Rows { 30, 17, 17, 30, 16, 16, 16 }; case 'p': return Rows { 0, 30, 17, 17, 30, 16, 16 };
        case 'Q': return Rows { 14, 17, 17, 17, 21, 18, 13 }; case 'q': return Rows { 0, 15, 17, 17, 15, 1, 1 };
        case 'R': return Rows { 30, 17, 17, 30, 20, 18, 17 }; case 'r': return Rows { 0, 0, 22, 25, 16, 16, 16 };
        case 'S': return Rows { 15, 16, 16, 14, 1, 1, 30 }; case 's': return Rows { 0, 0, 15, 16, 14, 1, 30 };
        case 'T': return Rows { 31, 4, 4, 4, 4, 4, 4 }; case 't': return Rows { 8, 8, 30, 8, 8, 9, 6 };
        case 'U': return Rows { 17, 17, 17, 17, 17, 17, 14 }; case 'u': return Rows { 0, 0, 17, 17, 17, 19, 13 };
        case 'V': return Rows { 17, 17, 17, 17, 17, 10, 4 }; case 'v': return Rows { 0, 0, 17, 17, 17, 10, 4 };
        case 'W': return Rows { 17, 17, 17, 21, 21, 21, 10 }; case 'w': return Rows { 0, 0, 17, 17, 21, 21, 10 };
        case 'X': return Rows { 17, 17, 10, 4, 10, 17, 17 }; case 'x': return Rows { 0, 0, 17, 10, 4, 10, 17 };
        case 'Y': return Rows { 17, 17, 10, 4, 4, 4, 4 }; case 'y': return Rows { 0, 17, 17, 17, 15, 1, 14 };
        case 'Z': return Rows { 31, 1, 2, 4, 8, 16, 31 }; case 'z': return Rows { 0, 0, 31, 2, 4, 8, 31 };
        case '0': return Rows { 14, 17, 19, 21, 25, 17, 14 }; case '1': return Rows { 4, 12, 4, 4, 4, 4, 14 };
        case '2': return Rows { 14, 17, 1, 2, 4, 8, 31 }; case '3': return Rows { 30, 1, 1, 14, 1, 1, 30 };
        case '4': return Rows { 2, 6, 10, 18, 31, 2, 2 }; case '5': return Rows { 31, 16, 16, 30, 1, 1, 30 };
        case '6': return Rows { 14, 16, 16, 30, 17, 17, 14 }; case '7': return Rows { 31, 1, 2, 4, 8, 8, 8 };
        case '8': return Rows { 14, 17, 17, 14, 17, 17, 14 }; case '9': return Rows { 14, 17, 17, 15, 1, 1, 14 };
        case '-': return Rows { 0, 0, 0, 31, 0, 0, 0 }; case '&': return Rows { 12, 18, 20, 8, 21, 18, 13 };
        case '.': return Rows { 0, 0, 0, 0, 0, 12, 12 }; case ':': return Rows { 0, 12, 12, 0, 12, 12, 0 };
        case '/': return Rows { 1, 2, 2, 4, 8, 8, 16 }; case '(': return Rows { 2, 4, 8, 8, 8, 4, 2 };
        case ')': return Rows { 8, 4, 2, 2, 2, 4, 8 }; case '%': return Rows { 25, 26, 2, 4, 8, 22, 19 };
        default: return Rows {};
    }
}

inline void drawBitmapText (juce::Graphics& g, const juce::String& text,
                            juce::Rectangle<float> area, float requestedHeight)
{
    const auto utf8 = text.toRawUTF8();
    const int length = (int) std::strlen (utf8);
    if (length <= 0 || area.isEmpty())
        return;

    const float pixel = juce::jmin (requestedHeight / 7.0f, area.getWidth() / ((float) length * 6.0f));
    const float advance = pixel * 6.0f;
    const float glyphHeight = pixel * 7.0f;
    const float top = area.getCentreY() - glyphHeight * 0.5f;
    float left = area.getX();

    for (const auto* character = utf8; *character != 0; ++character, left += advance)
    {
        const auto rows = bitmapGlyph ((unsigned char) *character);
        for (int row = 0; row < 7; ++row)
            for (int column = 0; column < 5; ++column)
                if ((rows[(size_t) row] & (1u << (4 - column))) != 0)
                    g.fillRect (left + pixel * (float) column, top + pixel * (float) row,
                                pixel * 0.82f, pixel * 0.82f);
    }
}
} // namespace junky::theme
