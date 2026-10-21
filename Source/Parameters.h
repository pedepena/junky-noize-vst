#pragma once

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

inline juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
{
    using Attr = juce::AudioParameterFloatAttributes;

    auto dbText = [] (float value, int) { return juce::String (value, 1) + " dB"; };

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { input, 1 }, "Input",
        juce::NormalisableRange<float> (-24.0f, 12.0f, 0.1f), 0.0f,
        Attr().withStringFromValueFunction (dbText)));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { output, 1 }, "Output",
        juce::NormalisableRange<float> (-24.0f, 12.0f, 0.1f), 0.0f,
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

    return layout;
}
} // namespace junky::params
