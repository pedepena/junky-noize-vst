#pragma once

#include <cmath>
#include <juce_core/juce_core.h>

namespace junky
{
inline float quantise (float x, int bits, bool companded = false) noexcept
{
    if (bits >= 16)
        return x;

    const float levels = static_cast<float> (1 << (bits - 1));
    x = juce::jlimit (-1.0f, 1.0f, x);
    if (! companded)
        return std::round (x * levels) / levels;

    constexpr float mu = 255.0f;
    const float compressed = (x < 0.0f ? -1.0f : 1.0f)
                             * std::log1p (mu * std::abs (x)) / std::log1p (mu);
    const float quantised = std::round (compressed * levels) / levels;
    return (quantised < 0.0f ? -1.0f : 1.0f)
           * (std::pow (1.0f + mu, std::abs (quantised)) - 1.0f) / mu;
}
} // namespace junky
