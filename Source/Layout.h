#pragma once

namespace junky::layout
{
// Coordenadas en el lienzo base de 1618 x 972 px. El origen de cada knob
// se calcula a partir del centro indicado en el PDF y su tamano de imagen.
inline constexpr float designWidth = 1618.0f;
inline constexpr float designHeight = 972.0f;

struct KnobBounds
{
    float x;
    float y;
    float size;
};

inline constexpr float largeKnobSize = 220.0f;
inline constexpr float mediumKnobSize = 124.0f;
inline constexpr float smallKnobSize = 94.0f;

inline constexpr KnobBounds filterKnob { 470.0f - mediumKnobSize * 0.5f, 437.0f - mediumKnobSize * 0.5f, mediumKnobSize };
inline constexpr KnobBounds mixKnob { 800.0f - largeKnobSize * 0.5f, 442.0f - largeKnobSize * 0.5f, largeKnobSize };
inline constexpr KnobBounds drunkKnob { 1122.0f - mediumKnobSize * 0.5f, 437.0f - mediumKnobSize * 0.5f, mediumKnobSize };
inline constexpr KnobBounds bitsKnob { 459.0f - smallKnobSize * 0.5f, 702.0f - smallKnobSize * 0.5f, smallKnobSize };
inline constexpr KnobBounds rateKnob { 587.0f - smallKnobSize * 0.5f, 780.0f - smallKnobSize * 0.5f, smallKnobSize };
inline constexpr KnobBounds driveKnob { 724.0f - smallKnobSize * 0.5f, 810.0f - smallKnobSize * 0.5f, smallKnobSize };
inline constexpr KnobBounds aliasKnob { 878.0f - smallKnobSize * 0.5f, 810.0f - smallKnobSize * 0.5f, smallKnobSize };
inline constexpr KnobBounds noiseKnob { 1020.0f - smallKnobSize * 0.5f, 780.0f - smallKnobSize * 0.5f, smallKnobSize };
inline constexpr KnobBounds characterKnob { 1156.0f - smallKnobSize * 0.5f, 702.0f - smallKnobSize * 0.5f, smallKnobSize };
inline constexpr KnobBounds bassFreqKnob { 281.0f - smallKnobSize * 0.5f, 684.0f - smallKnobSize * 0.5f, smallKnobSize };
inline constexpr KnobBounds inputKnob { 1362.0f - smallKnobSize * 0.5f, 799.0f - smallKnobSize * 0.5f, smallKnobSize };
inline constexpr KnobBounds outputKnob { 1494.0f - smallKnobSize * 0.5f, 799.0f - smallKnobSize * 0.5f, smallKnobSize };
} // namespace junky::layout
