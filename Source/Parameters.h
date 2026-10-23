#pragma once

#include <cmath>
#include <juce_audio_processors/juce_audio_processors.h>

namespace junky::params
{
// Identificadores de los parametros (no cambiarlos una vez publicado el plugin:
// los DAW los usan para guardar la automatizacion de los proyectos).
inline constexpr const char* input = "input";
inline constexpr const char* output = "output";
inline constexpr const char* mix = "mix";
inline constexpr const char* filter = "filter";
inline constexpr const char* bassBoost = "bassBoost";
inline constexpr const char* bassFreq = "bassFreq";
inline constexpr const char* mono = "mono";
inline constexpr const char* drive = "drive";
inline constexpr const char* bits = "bits";
inline constexpr const char* rate = "rate";
inline constexpr const char* alias = "alias";
inline constexpr const char* noise = "noise";
inline constexpr const char* drunk = "drunk";
inline constexpr const char* character = "character";

inline juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
{
    using Attr = juce::AudioParameterFloatAttributes;

    auto dbText = [] (float value, int) { return juce::String (value, 1) + " dB"; };

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Mantiene el rango -24..+12 dB, pero centra el recorrido visual en 0 dB.
    juce::NormalisableRange<float> gainRange (-24.0f, 12.0f, 0.1f);
    gainRange.setSkewForCentre (0.0f);

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { input, 1 }, "Input", gainRange, 0.0f,
        Attr().withStringFromValueFunction (dbText)));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { output, 1 }, "Output", gainRange, 0.0f,
        Attr().withStringFromValueFunction (dbText)));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { mix, 1 }, "Mix",
        juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f,
        Attr().withStringFromValueFunction ([] (float value, int)
                                            { return juce::String (juce::roundToInt (value * 100.0f)) + " %"; })));

    // -1 = muy oscuro, 0 = neutro, +1 = muy brillante
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { filter, 1 }, "Filter",
        juce::NormalisableRange<float> (-1.0f, 1.0f, 0.001f), 0.0f,
        Attr().withStringFromValueFunction ([] (float value, int)
                                            {
                                                if (std::abs (value) < 0.01f)
                                                    return juce::String ("Neutral");
                                                return juce::String (value < 0.0f ? "Dark " : "Bright ")
                                                       + juce::String (juce::roundToInt (std::abs (value) * 100.0f)) + " %";
                                            })));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { bassBoost, 1 }, "Bass Boost", false));

    // Frecuencia del bass boost: de 20 Hz a 500 Hz
    juce::NormalisableRange<float> freqRange (20.0f, 500.0f, 1.0f);
    freqRange.setSkewForCentre (100.0f);
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { bassFreq, 1 }, "Bass Freq", freqRange, 100.0f,
        Attr().withStringFromValueFunction ([] (float value, int)
                                            { return juce::String (juce::roundToInt (value)) + " Hz"; })));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { mono, 1 }, "Mono", false));

    auto percent = [] (float value, int)
    {
        return juce::String (juce::roundToInt (value * 100.0f)) + " %";
    };
    const juce::NormalisableRange<float> unit (0.0f, 1.0f, 0.001f);
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { drive, 1 }, "Drive", unit, 0.0f,
        Attr().withStringFromValueFunction (percent)));
    layout.add (std::make_unique<juce::AudioParameterChoice> (
        juce::ParameterID { bits, 1 }, "Bits", juce::StringArray { "16", "12", "8", "4" }, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { rate, 1 }, "Rate", unit, 0.0f,
        Attr().withStringFromValueFunction ([] (float value, int)
        {
            return value < 0.001f ? juce::String ("Off")
                                  : juce::String (48.0f * std::pow (4.0f / 48.0f, value), 1) + " kHz";
        })));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { alias, 1 }, "Alias", unit, 0.5f,
        Attr().withStringFromValueFunction ([] (float value, int)
        {
            const auto label = value < 0.5f ? "Smooth " : "Gritty ";
            return juce::String (label) + juce::String (juce::roundToInt (value * 100.0f)) + " %";
        })));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { noise, 1 }, "Noise", unit, 0.0f,
        Attr().withStringFromValueFunction (percent)));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { drunk, 1 }, "Drunk", unit, 0.0f,
        Attr().withStringFromValueFunction (percent)));
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { character, 1 }, "Character", unit, 1.0f,
        Attr().withStringFromValueFunction (percent)));

    return layout;
}
} // namespace junky::params
