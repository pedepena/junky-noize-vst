#pragma once

#include <cmath>

namespace junky
{
inline float saturate (float x, float drive01) noexcept
{
    if (drive01 <= 0.0005f)
        return x;

    const float gain = 1.0f + 7.0f * drive01;
    return std::tanh (gain * x) / std::pow (gain, 0.6f);
}
} // namespace junky
