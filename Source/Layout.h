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
inline constexpr ElementBounds presetLcd { 776.5f, 124.0f, 417.0f, 128.0f };
inline constexpr ElementBounds presetUpButton { 1098.0f, 83.0f, 84.0f, 70.0f };
inline constexpr ElementBounds presetDownButton { 1098.0f, 163.0f, 84.0f, 68.0f };
inline constexpr ElementBounds presetSaveButton { 1218.0f, 92.0f, 82.0f, 57.0f };
inline constexpr ElementBounds presetFolderButton { 1218.0f, 172.0f, 82.0f, 57.0f };
inline constexpr float presetLcdWidth = 417.0f;
inline constexpr float presetLcdMargin = 10.0f;
inline constexpr float presetLcdHeaderHeight = 16.0f;
inline constexpr float presetLcdScrollWidth = 3.0f;
inline constexpr float presetLcdScrollGap = 4.0f;
inline constexpr float presetLcdTextInset = 2.0f;
inline constexpr float presetLcdSelectionCorner = 1.5f;
inline constexpr float presetLcdScanlineStart = 1.0f;
inline constexpr float presetLcdScanlinePitch = 3.0f;
inline constexpr float presetLcdScanlineHeight = 0.6f;
inline constexpr float presetLcdGlowOffset = 0.5f;
inline constexpr int presetLcdVisibleRows = 6;
// Ventanas y columnas de los medidores (coordenadas del diseño 1618 x 972).
struct MeterLayout
{
    float x;
    float y;
    float width;
    float height;
    float leftColumnCentreX;
    float rightColumnCentreX;
};
inline constexpr MeterLayout inputMeter { 1306.0f, 478.0f, 71.0f, 221.0f, 1329.0f, 1354.0f };
inline constexpr MeterLayout outputMeter { 1458.0f, 478.0f, 68.0f, 221.0f, 1479.0f, 1504.0f };
inline constexpr float meterSegmentWidth = 20.0f;
inline constexpr float meterSegmentHeight = 8.0f;
inline constexpr float meterSegmentPitch = 10.0f;
inline constexpr float meterTopInset = 6.0f;
inline constexpr float meterBottomInset = 6.0f;
inline constexpr float meterScaleTopY = 484.0f;
inline constexpr float meterZeroDbY = 519.0f;
inline constexpr float meterMinus6DbY = 560.0f;
inline constexpr float meterMinus12DbY = 602.0f;
inline constexpr float meterMinus24DbY = 645.0f;
inline constexpr float meterMinus48DbY = 688.0f;
inline constexpr float meterMinus60DbY = 699.0f;
inline constexpr int meterSegmentCount = 21;
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
