#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace junky::theme
{
// Medidas del diseno original (en pixeles). Toda la interfaz se dibuja en estas
// coordenadas y despues se escala, asi se puede cambiar el tamano de la ventana.
inline constexpr float designWidth = 1085.0f;
inline constexpr float designHeight = 992.0f;

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

inline juce::Font boldFont (float height)
{
    return juce::Font (juce::FontOptions (height, juce::Font::bold));
}

inline juce::Font monoFont (float height)
{
    return juce::Font (juce::FontOptions (juce::Font::getDefaultMonospacedFontName(), height, juce::Font::bold));
}
} // namespace junky::theme
