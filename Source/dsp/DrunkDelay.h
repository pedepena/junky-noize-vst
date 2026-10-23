#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include <juce_core/juce_core.h>

namespace junky
{
/** Retardo modulable de cinta; toda la memoria se reserva en prepare(). */
class DrunkDelay
{
public:
    void prepare (double sampleRate)
    {
        sr = juce::jmax (1.0, sampleRate);
        int size = 1;
        const int wanted = juce::jmax (2, static_cast<int> (std::ceil (sr * 0.05)));
        while (size < wanted)
            size <<= 1;

        for (auto& channel : buffer)
            channel.assign (static_cast<size_t> (size), 0.0f);
        mask = size - 1;
        reset();
    }

    void reset() noexcept
    {
        writePosition = 0;
        wowPhase = flutterPhase = 0.0;
        drift = driftTarget = 0.0f;
        driftSamples = 0;
        random.setSeedRandomly();
        for (auto& channel : buffer)
            std::fill (channel.begin(), channel.end(), 0.0f);
    }

    void process (float* samples, int numChannels, float amount) noexcept
    {
        amount = juce::jlimit (0.0f, 1.0f, amount);
        const double wowDepth = 0.0060 * sr * amount;
        const double flutterDepth = 0.00035 * sr * amount;
        const double driftDepth = 0.0025 * sr * amount;
        const double baseDelay = wowDepth + flutterDepth + driftDepth + 2.0;

        ++driftSamples;
        if (driftSamples >= static_cast<int> (sr * 0.4))
        {
            driftSamples = 0;
            driftTarget = random.nextFloat() * 2.0f - 1.0f;
        }
        drift += static_cast<float> (1.0 - std::exp (-1.0 / (0.5 * sr))) * (driftTarget - drift);

        for (int channel = 0; channel < numChannels && channel < 2; ++channel)
        {
            auto& line = buffer[static_cast<size_t> (channel)];
            if (line.empty())
                continue;

            line[static_cast<size_t> (writePosition)] = samples[channel];
            if (amount >= 0.0005f)
            {
                const double delay = baseDelay
                    + wowDepth * std::sin (juce::MathConstants<double>::twoPi * (wowPhase + channel * 0.15))
                    + flutterDepth * std::sin (juce::MathConstants<double>::twoPi * (flutterPhase + channel * 0.31))
                    + driftDepth * drift;
                const double readPosition = static_cast<double> (writePosition) - delay;
                const auto index0 = static_cast<int> (std::floor (readPosition)) & mask;
                const auto index1 = (index0 + 1) & mask;
                const float fraction = static_cast<float> (readPosition - std::floor (readPosition));
                samples[channel] = line[static_cast<size_t> (index0)]
                                   + fraction * (line[static_cast<size_t> (index1)] - line[static_cast<size_t> (index0)]);
            }
        }

        writePosition = (writePosition + 1) & mask;
        wowPhase += 0.55 / sr;
        flutterPhase += 8.7 / sr;
        if (wowPhase >= 1.0) wowPhase -= 1.0;
        if (flutterPhase >= 1.0) flutterPhase -= 1.0;
    }

private:
    double sr = 44100.0;
    std::vector<float> buffer[2];
    int mask = 0;
    int writePosition = 0;
    double wowPhase = 0.0, flutterPhase = 0.0;
    float drift = 0.0f, driftTarget = 0.0f;
    int driftSamples = 0;
    juce::Random random;
};
} // namespace junky
