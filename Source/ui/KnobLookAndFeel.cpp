#include "KnobLookAndFeel.h"
#include "Theme.h"

namespace junky
{
void KnobLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPosProportional, float rotaryStartAngle,
                                        float rotaryEndAngle, juce::Slider&)
{
    const auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height);
    const auto centre = bounds.getCentre();
    const float outerRadius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const float capRadius = outerRadius * 0.70f;
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // Disco opaco sobre el arte de fondo: evita agujas y escalas estáticas bajo el control.
    juce::ColourGradient panel (juce::Colour (0xffb8b8b1), centre.x, centre.y - outerRadius,
                                juce::Colour (0xff92938e), centre.x, centre.y + outerRadius, false);
    g.setGradientFill (panel);
    g.fillEllipse (centre.x - outerRadius, centre.y - outerRadius, outerRadius * 2.0f, outerRadius * 2.0f);
    g.setColour (juce::Colour (0xff262726).withAlpha (0.85f));
    g.drawEllipse (centre.x - outerRadius, centre.y - outerRadius, outerRadius * 2.0f, outerRadius * 2.0f, 2.0f);

    // Marcas alrededor del knob (11 marcas repartidas por el recorrido)
    g.setColour (theme::ink.withAlpha (0.85f));
    const int numTicks = 11;

    for (int i = 0; i < numTicks; ++i)
    {
        const float t = (float) i / (float) (numTicks - 1);
        const float a = rotaryStartAngle + t * (rotaryEndAngle - rotaryStartAngle);
        const bool major = (i == 0 || i == numTicks - 1 || i == numTicks / 2);
        const float r1 = outerRadius * (major ? 0.80f : 0.84f);
        const float r2 = outerRadius * 0.96f;

        juce::Line<float> tick (centre.x + std::sin (a) * r1, centre.y - std::cos (a) * r1,
                                centre.x + std::sin (a) * r2, centre.y - std::cos (a) * r2);
        g.drawLine (tick, major ? outerRadius * 0.035f : outerRadius * 0.022f);
    }

    // Sombra bajo el knob
    g.setColour (juce::Colours::black.withAlpha (0.35f));
    g.fillEllipse (centre.x - capRadius + capRadius * 0.06f, centre.y - capRadius + capRadius * 0.10f,
                   capRadius * 2.0f, capRadius * 2.0f);

    // Anillo exterior
    {
        juce::ColourGradient ring (juce::Colour (0xff4a4a4a), centre.x, centre.y - capRadius,
                                   juce::Colour (0xff0d0d0d), centre.x, centre.y + capRadius, false);
        g.setGradientFill (ring);
        g.fillEllipse (centre.x - capRadius, centre.y - capRadius, capRadius * 2.0f, capRadius * 2.0f);
    }

    // Tapa
    {
        const float inner = capRadius * 0.88f;
        juce::ColourGradient cap (juce::Colour (0xff343434), centre.x - inner * 0.4f, centre.y - inner * 0.5f,
                                  juce::Colour (0xff111111), centre.x + inner * 0.6f, centre.y + inner * 0.8f, true);
        g.setGradientFill (cap);
        g.fillEllipse (centre.x - inner, centre.y - inner, inner * 2.0f, inner * 2.0f);

        // Desgaste: puntitos claros en posiciones fijas (siempre los mismos)
        juce::Random wear (1234);
        g.setColour (juce::Colour (0xff8a8a85).withAlpha (0.35f));

        for (int i = 0; i < 26; ++i)
        {
            const float a = wear.nextFloat() * juce::MathConstants<float>::twoPi;
            const float d = std::sqrt (wear.nextFloat()) * inner * 0.92f;
            const float s = 0.8f + wear.nextFloat() * (capRadius * 0.05f);
            g.fillEllipse (centre.x + std::cos (a) * d - s * 0.5f, centre.y + std::sin (a) * d - s * 0.5f, s, s * 0.7f);
        }

        g.setColour (juce::Colours::white.withAlpha (0.10f));
        g.drawEllipse (centre.x - inner, centre.y - inner, inner * 2.0f, inner * 2.0f, 1.0f);
    }

    // Aguja
    {
        const float r1 = capRadius * 0.25f;
        const float r2 = capRadius * 0.92f;
        juce::Line<float> pointer (centre.x + std::sin (angle) * r1, centre.y - std::cos (angle) * r1,
                                   centre.x + std::sin (angle) * r2, centre.y - std::cos (angle) * r2);
        g.setColour (juce::Colour (0xffeae6da));
        g.drawLine (pointer, juce::jmax (2.0f, capRadius * 0.08f));
    }
}
} // namespace junky
