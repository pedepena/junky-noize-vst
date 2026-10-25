#pragma once

#include <map>
#include <cstdint>
#include <juce_gui_basics/juce_gui_basics.h>

namespace junky::wear
{
enum class Surface : std::uint8_t { knob, key, button, arrow, lcd, meter, panel, lettering };

namespace detail
{
struct Cache
{
    juce::CriticalSection lock;
    std::map<std::uint64_t, juce::Image> images;
};

inline juce::Image makeTexture (Surface surface, int seed)
{
    const bool fullPanel = surface == Surface::panel || surface == Surface::lettering;
    const int width = fullPanel ? 1085 : 256;
    const int height = fullPanel ? 1182 : 256;
    juce::Image image (juce::Image::ARGB, width, height, true);
    juce::Graphics g (image);
    juce::Random random ((juce::int64) seed);

    auto fleck = [&] (int count, float minimum, float maximum, juce::Colour colour)
    {
        for (int i = 0; i < count; ++i)
        {
            const float size = minimum + random.nextFloat() * (maximum - minimum);
            const float x = random.nextFloat() * (float) width;
            const float y = random.nextFloat() * (float) height;
            g.setColour (colour.withAlpha (0.10f + random.nextFloat() * 0.22f));
            g.fillEllipse (x, y, size, size * (0.55f + random.nextFloat() * 0.8f));
        }
    };

    if (surface == Surface::knob)
    {
        const float centre = width * 0.5f, radius = width * 0.49f;
        for (int i = 0; i < 42; ++i)
        {
            const float angle = random.nextFloat() * juce::MathConstants<float>::twoPi;
            const float distance = radius * (0.70f + random.nextFloat() * 0.28f);
            const float length = 3.0f + random.nextFloat() * 16.0f;
            const float x = centre + std::cos (angle) * distance;
            const float y = centre + std::sin (angle) * distance;
            const float tilt = (random.nextFloat() - 0.5f) * 0.55f;
            g.setColour (juce::Colour (0xffc4bda8).withAlpha (0.18f + random.nextFloat() * 0.24f));
            g.drawLine (x, y, x + std::cos (angle + tilt) * length,
                        y + std::sin (angle + tilt) * length, 0.7f + random.nextFloat() * 0.8f);
        }
        fleck (42, 1.0f, 3.5f, juce::Colour (0xffb9b3a1));
        // A thin, irregular dull-metal rim makes the edge look handled without hiding the pointer.
        for (int i = 0; i < 34; ++i)
        {
            const float angle = random.nextFloat() * juce::MathConstants<float>::twoPi;
            const float r = radius * (0.91f + random.nextFloat() * 0.08f);
            const float x = centre + std::cos (angle) * r, y = centre + std::sin (angle) * r;
            g.setColour ((i % 3 == 0 ? juce::Colour (0xff9b5931) : juce::Colour (0xffd1c8ad))
                             .withAlpha (0.18f + random.nextFloat() * 0.25f));
            g.drawLine (x, y, x + std::cos (angle) * (2.0f + random.nextFloat() * 7.0f),
                        y + std::sin (angle) * (2.0f + random.nextFloat() * 7.0f), 1.0f);
        }
    }
    else if (surface == Surface::lcd)
    {
        for (int i = 0; i < 15; ++i)
        {
            const float x = random.nextFloat() * (float) width;
            const float y = random.nextFloat() * (float) height;
            const float length = 12.0f + random.nextFloat() * 70.0f;
            g.setColour (juce::Colour (0xffe5efcf).withAlpha (0.13f + random.nextFloat() * 0.12f));
            g.drawLine (x, y, x + length, y + (random.nextFloat() - 0.5f) * 5.0f,
                        0.6f + random.nextFloat() * 0.7f);
        }
        fleck (75, 1.0f, 2.0f, juce::Colour (0xff283b20));
        for (int i = 0; i < 4; ++i)
        {
            const auto corner = juce::Rectangle<float> (12.0f, 12.0f).withPosition (
                i % 2 == 0 ? 0.0f : (float) width - 12.0f,
                i < 2 ? 0.0f : (float) height - 12.0f);
            g.setColour (juce::Colour (0xff34472b).withAlpha (0.12f));
            g.fillEllipse (corner);
        }
    }
    else if (surface == Surface::meter)
    {
        for (int i = 0; i < 12; ++i)
        {
            const float x = random.nextFloat() * (float) width;
            const float y = random.nextFloat() * (float) height;
            g.setColour (juce::Colour (0xffd6d2c5).withAlpha (0.10f + random.nextFloat() * 0.12f));
            g.drawLine (x, y, x + 4.0f + random.nextFloat() * 24.0f,
                        y + (random.nextFloat() - 0.5f) * 3.0f, 0.6f);
        }
        fleck (45, 1.0f, 2.5f, juce::Colour (0xffc8c2b1));
    }
    else if (surface == Surface::panel)
    {
        // Overall age is strongest near the outer frame and common hand-contact areas.
        for (int i = 0; i < 300; ++i)
        {
            float x = random.nextFloat() * (float) width;
            float y = random.nextFloat() * (float) height;
            if (i < 190)
            {
                const int edge = random.nextInt (4);
                x = edge < 2 ? random.nextFloat() * (float) width : (edge == 2 ? 1.0f : (float) width - 1.0f);
                y = edge >= 2 ? random.nextFloat() * (float) height : (edge == 0 ? 1.0f : (float) height - 1.0f);
            }
            const float size = 1.0f + random.nextFloat() * 5.0f;
            g.setColour ((i % 4 == 0 ? juce::Colour (0xff87502e) : juce::Colour (0xffd3cbb7))
                             .withAlpha (0.07f + random.nextFloat() * 0.13f));
            g.fillEllipse (x, y, size, size * 0.65f);
        }
        for (int i = 0; i < 90; ++i)
        {
            const float x = random.nextFloat() * (float) width;
            const float y = random.nextFloat() * (float) height;
            const float length = 8.0f + random.nextFloat() * 46.0f;
            const float angle = random.nextFloat() * juce::MathConstants<float>::twoPi;
            g.setColour (juce::Colour (0xffd2c9b4).withAlpha (0.07f + random.nextFloat() * 0.13f));
            g.drawLine (x, y, x + std::cos (angle) * length, y + std::sin (angle) * length, 0.5f);
        }
    }
    else
    {
        const int count = surface == Surface::lettering ? 65 : 38;
        fleck (count, 1.0f, 4.0f, juce::Colour (0xff8a5736));
        for (int i = 0; i < count / 3; ++i)
        {
            const float x = random.nextFloat() * (float) width;
            const float y = random.nextFloat() * (float) height;
            const float length = 4.0f + random.nextFloat() * 20.0f;
            const float angle = random.nextFloat() * juce::MathConstants<float>::twoPi;
            g.setColour (juce::Colour (0xffd2cbb9).withAlpha (0.16f + random.nextFloat() * 0.16f));
            g.drawLine (x, y, x + std::cos (angle) * length, y + std::sin (angle) * length, 0.8f);
        }
    }

    return image;
}
} // namespace detail

inline juce::Image texture (Surface surface, int seed)
{
    static detail::Cache cache;
    const auto key = (static_cast<std::uint64_t> (surface) << 32)
                   | static_cast<std::uint32_t> (seed);
    const juce::ScopedLock lock (cache.lock);
    if (const auto found = cache.images.find (key); found != cache.images.end())
        return found->second;

    auto image = detail::makeTexture (surface, seed);
    cache.images.emplace (key, image);
    return image;
}

inline void draw (juce::Graphics& g, juce::Rectangle<float> bounds, Surface surface, int seed)
{
    const auto image = texture (surface, seed);
    if (image.isValid() && ! bounds.isEmpty())
        g.drawImage (image, bounds);
}
} // namespace junky::wear
