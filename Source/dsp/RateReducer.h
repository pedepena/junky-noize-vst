#pragma once

namespace junky
{
/** Reductor sample & hold con estado compartido entre canales. */
struct RateReducer
{
    double phase = 1.0;
    float held[2] { 0.0f, 0.0f };

    void reset() noexcept
    {
        phase = 1.0;
        held[0] = held[1] = 0.0f;
    }

    void process (float* samples, int numChannels, double ratio) noexcept
    {
        if (ratio >= 0.999)
            return;

        phase += ratio;
        if (phase >= 1.0)
        {
            phase -= 1.0;
            for (int channel = 0; channel < numChannels && channel < 2; ++channel)
                held[channel] = samples[channel];
        }

        for (int channel = 0; channel < numChannels && channel < 2; ++channel)
            samples[channel] = held[channel];
    }
};
} // namespace junky
