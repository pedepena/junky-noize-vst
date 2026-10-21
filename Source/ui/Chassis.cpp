#include "Chassis.h"
#include "Theme.h"

namespace junky
{
namespace
{
    void drawScrew (juce::Graphics& g, float cx, float cy, float r, float slotAngle)
    {
        juce::ColourGradient grad (juce::Colour (0xff6b6b68), cx - r * 0.4f, cy - r * 0.4f,
                                   juce::Colour (0xff1c1c1b), cx + r * 0.6f, cy + r * 0.6f, true);
        g.setGradientFill (grad);
        g.fillEllipse (cx - r, cy - r, r * 2.0f, r * 2.0f);
        g.setColour (juce::Colours::black.withAlpha (0.6f));
        g.drawEllipse (cx - r, cy - r, r * 2.0f, r * 2.0f, 1.5f);

        juce::Line<float> slot (cx - std::cos (slotAngle) * r * 0.75f, cy - std::sin (slotAngle) * r * 0.75f,
                                cx + std::cos (slotAngle) * r * 0.75f, cy + std::sin (slotAngle) * r * 0.75f);
        g.setColour (juce::Colour (0xff0b0b0b));
        g.drawLine (slot, r * 0.22f);
    }

    void drawInset (juce::Graphics& g, juce::Rectangle<float> r, float corner)
    {
        g.setColour (juce::Colours::black.withAlpha (0.25f));
        g.fillRoundedRectangle (r.translated (0.0f, 3.0f), corner);

        juce::ColourGradient grad (theme::metalMid.brighter (0.1f), r.getX(), r.getY(),
                                   theme::metalMid.darker (0.12f), r.getX(), r.getBottom(), false);
        g.setGradientFill (grad);
        g.fillRoundedRectangle (r, corner);

        g.setColour (theme::ink.withAlpha (0.75f));
        g.drawRoundedRectangle (r, corner, 3.0f);
    }

    void drawRustPatch (juce::Graphics& g, float cx, float cy, float radius, float alpha)
    {
        juce::ColourGradient grad (theme::rust.withAlpha (alpha), cx, cy,
                                   theme::rust.withAlpha (0.0f), cx + radius, cy, true);
        g.setGradientFill (grad);
        g.fillEllipse (cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
    }
} // namespace

void paintChassis (juce::Graphics& g)
{
    using namespace theme;
    const juce::Rectangle<float> whole (0.0f, 0.0f, designWidth, designHeight);

    // Chapa base
    g.setGradientFill (juce::ColourGradient (metalLight, 0.0f, 0.0f, metalMid, 0.0f, designHeight, false));
    g.fillRect (whole);

    // Tres paneles apilados
    const juce::Rectangle<float> panels[] = { { 0.0f, 0.0f, designWidth, 322.0f },
                                              { 0.0f, 326.0f, designWidth, 354.0f },
                                              { 0.0f, 684.0f, designWidth, designHeight - 684.0f } };

    for (auto p : panels)
    {
        g.setColour (ink.withAlpha (0.9f));
        g.drawRect (p, 5.0f);
    }

    // Rejilla de ventilacion
    for (int i = 0; i < 8; ++i)
    {
        const juce::Rectangle<float> slot (52.0f, 375.0f + (float) i * 31.0f, 108.0f, 16.0f);
        g.setColour (juce::Colours::black.withAlpha (0.35f));
        g.fillRoundedRectangle (slot.translated (0.0f, 2.0f), 4.0f);
        g.setColour (ink);
        g.fillRoundedRectangle (slot, 4.0f);
    }

    // Marco oscuro de la pantalla LCD
    {
        const juce::Rectangle<float> bezel (305.0f, 28.0f, 592.0f, 280.0f);
        g.setColour (juce::Colours::black.withAlpha (0.35f));
        g.fillRoundedRectangle (bezel.translated (0.0f, 4.0f), 18.0f);
        g.setGradientFill (juce::ColourGradient (juce::Colour (0xff3a3a3a), 0.0f, bezel.getY(),
                                                 juce::Colour (0xff171717), 0.0f, bezel.getBottom(), false));
        g.fillRoundedRectangle (bezel, 18.0f);
        g.setColour (ink);
        g.drawRoundedRectangle (bezel, 18.0f, 3.0f);
    }

    // Paneles hundidos
    drawInset (g, { 775.0f, 395.0f, 262.0f, 218.0f }, 10.0f); // A/B
    drawInset (g, { 55.0f, 735.0f, 335.0f, 195.0f }, 10.0f);  // Mono / Stereo
    drawInset (g, { 410.0f, 710.0f, 392.0f, 225.0f }, 10.0f); // Bass boost
    drawInset (g, { 822.0f, 695.0f, 215.0f, 250.0f }, 10.0f); // Input / Output

    // Linea que separa MONO de STEREO
    g.setColour (ink.withAlpha (0.6f));
    g.fillRect (222.0f, 760.0f, 3.0f, 150.0f);

    // Aranazos (siempre los mismos)
    juce::Random random (42);

    for (int i = 0; i < 70; ++i)
    {
        const float x = random.nextFloat() * designWidth;
        const float y = random.nextFloat() * designHeight;
        const float len = 40.0f + random.nextFloat() * 220.0f;
        const float a = random.nextFloat() * juce::MathConstants<float>::twoPi;

        g.setColour ((i % 3 == 0 ? juce::Colours::white : juce::Colours::black).withAlpha (0.10f));
        g.drawLine (x, y, x + std::cos (a) * len, y + std::sin (a) * len, 1.0f + random.nextFloat());
    }

    // Oxido en bordes y esquinas
    for (int i = 0; i < 26; ++i)
    {
        const bool horizontalEdge = random.nextBool();
        const float cx = horizontalEdge ? random.nextFloat() * designWidth
                                        : (random.nextBool() ? 0.0f : designWidth) + (random.nextFloat() - 0.5f) * 30.0f;
        const float cy = horizontalEdge ? (random.nextBool() ? 0.0f : designHeight) + (random.nextFloat() - 0.5f) * 30.0f
                                        : random.nextFloat() * designHeight;
        drawRustPatch (g, cx, cy, 25.0f + random.nextFloat() * 60.0f, 0.55f);
    }

    drawRustPatch (g, 40.0f, 330.0f, 60.0f, 0.5f);
    drawRustPatch (g, 1050.0f, 684.0f, 70.0f, 0.5f);

    // Tornillos
    const juce::Point<float> screws[] = { { 36.0f, 34.0f },    { 1048.0f, 34.0f },
                                          { 36.0f, 350.0f },   { 1048.0f, 350.0f },
                                          { 36.0f, 664.0f },   { 1048.0f, 664.0f },
                                          { 36.0f, 944.0f },   { 1048.0f, 944.0f },
                                          { 432.0f, 732.0f },  { 780.0f, 732.0f },
                                          { 432.0f, 915.0f },  { 780.0f, 915.0f },
                                          { 795.0f, 585.0f },  { 1015.0f, 585.0f } };
    float angle = 0.4f;

    for (auto s : screws)
    {
        drawScrew (g, s.x, s.y, 14.0f, angle);
        angle += 0.9f;
    }
}

void paintLabels (juce::Graphics& g, bool isSlotB)
{
    using namespace theme;

    // ---- Logo ----------------------------------------------------------------
    {
        juce::Graphics::ScopedSaveState state (g);
        g.addTransform (juce::AffineTransform::rotation (-0.10f, 170.0f, 180.0f));

        // Corona
        juce::Path crown;
        crown.startNewSubPath (135.0f, 84.0f);
        crown.lineTo (132.0f, 38.0f);
        crown.lineTo (152.0f, 58.0f);
        crown.lineTo (168.0f, 28.0f);
        crown.lineTo (184.0f, 58.0f);
        crown.lineTo (204.0f, 38.0f);
        crown.lineTo (200.0f, 84.0f);
        crown.closeSubPath();
        g.setColour (ink);
        g.strokePath (crown, juce::PathStrokeType (7.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        g.setFont (boldFont (125.0f));
        g.setColour (ink);
        g.drawFittedText ("JUNKY", { 45, 82, 250, 105 }, juce::Justification::centredLeft, 1, 0.6f);
        g.drawFittedText ("NOIZE", { 50, 180, 255, 105 }, juce::Justification::centredLeft, 1, 0.6f);
    }

    // ---- Nota adhesiva "Beats Not Borders" ------------------------------------------
    {
        juce::Graphics::ScopedSaveState state (g);
        const juce::Rectangle<float> note (925.0f, 65.0f, 135.0f, 190.0f);
        g.addTransform (juce::AffineTransform::rotation (0.07f, note.getCentreX(), note.getCentreY()));

        g.setColour (juce::Colours::black.withAlpha (0.3f));
        g.fillRect (note.translated (5.0f, 6.0f));
        g.setColour (juce::Colour (0xffeed9a8));
        g.fillRect (note);
        g.setColour (juce::Colour (0xffd6bf86));
        g.fillRect (note.withHeight (22.0f));

        g.setColour (ink);
        g.setFont (boldFont (44.0f).italicised());
        g.drawFittedText ("BEATS\nNOT\nBORDERS", note.toNearestInt().withTrimmedTop (12).reduced (8, 6),
                          juce::Justification::centred, 3, 0.5f);
    }

    // ---- Etiquetas de los controles ---------------------------------------------
    g.setColour (ink);

    auto label = [&g] (const juce::String& text, float cx, float cy, float size, float width = 220.0f)
    {
        g.setFont (boldFont (size));
        g.drawText (text, juce::Rectangle<float> (width, size * 1.4f).withCentre ({ cx, cy }),
                    juce::Justification::centred);
    };

    label ("FILTER", 287.0f, 428.0f, 30.0f);
    label ("DARK", 222.0f, 620.0f, 24.0f, 120.0f);
    label ("BRIGHT", 338.0f, 620.0f, 24.0f, 120.0f);

    label ("MIX", 540.0f, 650.0f, 48.0f);
    label ("0%", 428.0f, 618.0f, 22.0f, 80.0f);
    label ("100%", 672.0f, 618.0f, 22.0f, 100.0f);

    label ("A/B", 905.0f, 432.0f, 40.0f);
    {
        // Pilotos de A y B: se enciende el del ajuste activo
        const juce::Point<float> ledA (840.0f, 432.0f), ledB (975.0f, 432.0f);
        auto led = [&g] (juce::Point<float> p, bool on)
        {
            if (on)
            {
                g.setColour (theme::ledRed.withAlpha (0.35f));
                g.fillEllipse (p.x - 20.0f, p.y - 20.0f, 40.0f, 40.0f);
            }
            g.setColour (on ? theme::ledRed : juce::Colour (0xff3a0f0c));
            g.fillEllipse (p.x - 13.0f, p.y - 13.0f, 26.0f, 26.0f);
            g.setColour (juce::Colours::black.withAlpha (0.6f));
            g.drawEllipse (p.x - 13.0f, p.y - 13.0f, 26.0f, 26.0f, 2.0f);
        };
        led (ledA, ! isSlotB);
        led (ledB, isSlotB);
    }

    label ("MONO", 145.0f, 758.0f, 30.0f, 150.0f);
    label ("STEREO", 305.0f, 758.0f, 30.0f, 170.0f);

    label ("BASS BOOST", 525.0f, 745.0f, 28.0f, 240.0f);
    label ("FREQ", 680.0f, 745.0f, 28.0f, 120.0f);

    // Valores alrededor del knob de frecuencia (20 - 500 Hz)
    {
        const juce::Point<float> centre (680.0f, 840.0f);
        const float startAngle = juce::MathConstants<float>::pi * 1.25f;
        const float endAngle = juce::MathConstants<float>::pi * 2.75f;

        juce::NormalisableRange<float> range (20.0f, 500.0f, 1.0f);
        range.setSkewForCentre (100.0f);

        const std::pair<float, const char*> marks[] = { { 20.0f, "20" }, { 50.0f, "50" }, { 100.0f, "100" },
                                                        { 200.0f, "200" }, { 500.0f, "500" } };

        for (auto [hz, text] : marks)
        {
            const float a = startAngle + range.convertTo0to1 (hz) * (endAngle - startAngle);
            const float r = 98.0f;
            label (text, centre.x + std::sin (a) * r, centre.y - std::cos (a) * r, 20.0f, 70.0f);
        }
    }

    label ("INPUT", 880.0f, 712.0f, 20.0f, 100.0f);
    label ("OUTPUT", 985.0f, 712.0f, 20.0f, 110.0f);
    label ("IN", 873.0f, 940.0f, 24.0f, 80.0f);
    label ("OUT", 978.0f, 940.0f, 24.0f, 80.0f);
}
} // namespace junky
