#pragma once

namespace junky::layout
{
// Coordenadas medidas sobre el fondo base de 1618 x 972 px.
inline constexpr float designWidth = 1618.0f;
inline constexpr float designHeight = 972.0f;

struct KnobBounds
{
    float centreX;
    float centreY;
    float size;

    constexpr float x() const { return centreX - size * 0.5f; }
    constexpr float y() const { return centreY - size * 0.5f; }
};

inline constexpr float largeKnobSize = 190.0f;
inline constexpr float mediumKnobSize = 100.0f;
inline constexpr float smallKnobSize = 76.0f;

inline constexpr KnobBounds filterKnob { 499.0f, 371.0f, mediumKnobSize };
inline constexpr KnobBounds mixKnob { 808.0f, 398.0f, largeKnobSize };
inline constexpr KnobBounds drunkKnob { 1117.0f, 371.0f, mediumKnobSize };
inline constexpr KnobBounds bitsKnob { 464.0f, 667.0f, smallKnobSize };
inline constexpr KnobBounds rateKnob { 590.0f, 752.0f, smallKnobSize };
inline constexpr KnobBounds driveKnob { 727.0f, 781.0f, smallKnobSize };
inline constexpr KnobBounds aliasKnob { 869.0f, 780.0f, smallKnobSize };
inline constexpr KnobBounds noiseKnob { 1015.0f, 753.0f, smallKnobSize };
inline constexpr KnobBounds characterKnob { 1160.0f, 675.0f, smallKnobSize };
inline constexpr KnobBounds bassFreqKnob { 288.0f, 653.0f, smallKnobSize };
inline constexpr KnobBounds inputKnob { 1343.0f, 774.0f, smallKnobSize };
inline constexpr KnobBounds outputKnob { 1488.0f, 779.0f, smallKnobSize };
} // namespace junky::layout