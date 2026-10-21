#pragma once

#include <cmath>
#include <juce_core/juce_core.h>

namespace junky
{
/** Filtro de 2 polos (biquad) para hasta 2 canales.
    Calcula sus coeficientes sin reservar memoria, asi que es seguro usarlo en el hilo de audio. */
class Biquad
{
public:
    void reset() noexcept
    {
        for (int i = 0; i < 2; ++i)
        {
            z1[i] = 0.0;
            z2[i] = 0.0;
        }
    }

    void setLowPass (double sampleRate, double frequency, double q = 0.70710678) noexcept
    {
        const auto w0 = juce::MathConstants<double>::twoPi * limitFrequency (sampleRate, frequency) / sampleRate;
        const auto cosW = std::cos (w0);
        const auto alpha = std::sin (w0) / (2.0 * q);

        setCoefficients ((1.0 - cosW) * 0.5, 1.0 - cosW, (1.0 - cosW) * 0.5,
                         1.0 + alpha, -2.0 * cosW, 1.0 - alpha);
    }

    void setLowShelf (double sampleRate, double frequency, double gainDb) noexcept
    {
        const auto a = std::pow (10.0, gainDb / 40.0);
        const auto w0 = juce::MathConstants<double>::twoPi * limitFrequency (sampleRate, frequency) / sampleRate;
        const auto cosW = std::cos (w0);
        const auto alpha = std::sin (w0) * 0.5 * std::sqrt (2.0);
        const auto beta = 2.0 * std::sqrt (a) * alpha;

        setCoefficients (a * ((a + 1.0) - (a - 1.0) * cosW + beta),
                         2.0 * a * ((a - 1.0) - (a + 1.0) * cosW),
                         a * ((a + 1.0) - (a - 1.0) * cosW - beta),
                         (a + 1.0) + (a - 1.0) * cosW + beta,
                         -2.0 * ((a - 1.0) + (a + 1.0) * cosW),
                         (a + 1.0) + (a - 1.0) * cosW - beta);
    }

    void setHighShelf (double sampleRate, double frequency, double gainDb) noexcept
    {
        const auto a = std::pow (10.0, gainDb / 40.0);
        const auto w0 = juce::MathConstants<double>::twoPi * limitFrequency (sampleRate, frequency) / sampleRate;
        const auto cosW = std::cos (w0);
        const auto alpha = std::sin (w0) * 0.5 * std::sqrt (2.0);
        const auto beta = 2.0 * std::sqrt (a) * alpha;

        setCoefficients (a * ((a + 1.0) + (a - 1.0) * cosW + beta),
                         -2.0 * a * ((a - 1.0) + (a + 1.0) * cosW),
                         a * ((a + 1.0) + (a - 1.0) * cosW - beta),
                         (a + 1.0) - (a - 1.0) * cosW + beta,
                         2.0 * ((a - 1.0) - (a + 1.0) * cosW),
                         (a + 1.0) - (a - 1.0) * cosW - beta);
    }

    float process (int channel, float input) noexcept
    {
        const double x = input;
        const double y = b0 * x + z1[channel];
        z1[channel] = b1 * x - a1 * y + z2[channel];
        z2[channel] = b2 * x - a2 * y;
        return static_cast<float> (y);
    }

private:
    static double limitFrequency (double sampleRate, double frequency) noexcept
    {
        return juce::jlimit (10.0, sampleRate * 0.45, frequency);
    }

    void setCoefficients (double nb0, double nb1, double nb2,
                          double na0, double na1, double na2) noexcept
    {
        b0 = nb0 / na0;
        b1 = nb1 / na0;
        b2 = nb2 / na0;
        a1 = na1 / na0;
        a2 = na2 / na0;
    }

    double b0 = 1.0, b1 = 0.0, b2 = 0.0, a1 = 0.0, a2 = 0.0;
    double z1[2] { 0.0, 0.0 };
    double z2[2] { 0.0, 0.0 };
};
} // namespace junky
