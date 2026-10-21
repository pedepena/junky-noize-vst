#pragma once

#include <array>

namespace junky
{
/** Un preset es una combinacion de ajustes. Cuando existan los modelos de maquinas
    (fase 2), cada preset tambien indicara que maquina usa. */
struct Preset
{
    const char* name;
    float filter;     // -1 (oscuro) .. +1 (brillante)
    float mix;        // 0..1
    bool bassBoost;
    float bassFreq;   // 20..500 Hz
    bool mono;
    bool isCustom;    // "Custom": no cambia ningun ajuste al elegirlo
};

inline constexpr std::array<Preset, 8> factoryPresets { {
    { "MPC 60 LoFi",      -0.55f, 0.85f, true,   90.0f, false, false },
    { "SP-1200 Dusty",    -0.35f, 1.00f, true,  120.0f, false, false },
    { "Boom Bap Classic", -0.20f, 1.00f, true,   80.0f, false, false },
    { "Vinyl Crackle",    -0.50f, 0.70f, false, 100.0f, true,  false },
    { "Tape Saturation",  -0.15f, 0.90f, true,   60.0f, false, false },
    { "Dirty Funk",        0.35f, 1.00f, true,  150.0f, false, false },
    { "Warm & Round",     -0.30f, 1.00f, true,   70.0f, false, false },
    { "Custom 1",          0.00f, 1.00f, false, 100.0f, false, true  },
} };
} // namespace junky
