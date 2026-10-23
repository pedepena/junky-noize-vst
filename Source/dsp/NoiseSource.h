#pragma once

#include <cmath>
#include <juce_core/juce_core.h>

namespace junky
{
/** Ruido blanco filtrado con un polo independiente por canal. */
class NoiseSource
{
public:
    void reset() noexcept
    {
        lowPass[0] = lowPass[1] = 0.0f;
    }

    float process (int channel, float colour, float gain) noexcept
    {
        if (gain <= 0.0f || channel < 0 || channel > 1)
            return 0.0f;

        const float alpha = 0.05f + 0.95f * juce::jlimit (0.0f, 1.0f, colour);
        const float white = random.nextFloat() * 2.0f - 1.0f;
        auto& state = lowPass[channel];
        state += alpha * (white - state);
        return state * std::sqrt ((2.0f - alpha) / alpha) * gain;
    }

private:
    juce::Random random;
    float lowPass[2] { 0.0f, 0.0f };
};
} // namespace junky
