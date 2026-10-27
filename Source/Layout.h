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

struct ElementBounds
{
    float centreX;
    float centreY;
    float width;
    float height;

    constexpr float x() const { return centreX - width * 0.5f; }
    constexpr float y() const { return centreY - height * 0.5f; }
};

inline constexpr ElementBounds bassButton { 146.0f, 661.0f, 62.0f, 62.0f };
inline constexpr ElementBounds bassLed { 96.0f, 611.0f, 22.0f, 22.0f };
inline constexpr ElementBounds monoLed { 139.0f, 783.0f, 22.0f, 22.0f };
inline constexpr ElementBounds stereoLed { 302.0f, 783.0f, 22.0f, 22.0f };
inline constexpr float switchTrackStartX = 173.0f;
inline constexpr float switchTrackEndX = 270.0f;
inline constexpr float switchTrackCentreY = 779.0f;
inline constexpr float switchTrackHitHeight = 44.0f;
inline constexpr float switchLeverMonoCentreX = 195.0f;
inline constexpr float switchLeverStereoCentreX = 248.0f;
inline constexpr float switchLeverWidth = 40.0f;
inline constexpr float switchLeverHeight = 35.0f;
inline constexpr float bassButtonPressInset = 2.0f;
inline constexpr int switchAnimationDurationMs = 120;

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