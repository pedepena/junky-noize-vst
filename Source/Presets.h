#pragma once
#include <array>
namespace junky
{
struct Preset
{
    const char* name; int machine; float filter, mix; bool bassBoost; float bassFreq; bool mono;
    float drive; int bitsIndex; float rate, alias, noise, drunk, character;
    enum class Kind { init, factory } kind;
};
inline constexpr std::array<Preset, 51> factoryPresets {{
    {"DEFAULT",-1,0,1,false,100,false,0,0,0,.5f,0,0,1,Preset::Kind::init},
    {"MBC-60",0,-.10f,1,true,90,false,.48f,1,.14f,.55f,.18f,0,1,Preset::Kind::factory},
    {"MBC-60 II",1,0,1,true,85,false,.42f,1,.12f,.45f,.14f,0,1,Preset::Kind::factory},
    {"MBC-500",2,-.05f,1,false,100,false,.3f,0,.12f,.3f,.1f,0,1,Preset::Kind::factory},
    {"MBC-1000",3,-.05f,1,false,100,false,.28f,0,.12f,.28f,.08f,0,1,Preset::Kind::factory},
    {"MBC-2000",4,-.10f,1,true,100,false,.38f,0,.15f,.35f,.12f,0,1,Preset::Kind::factory},
    {"MBC-2000XL",5,0,1,true,100,false,.35f,0,.15f,.35f,.1f,0,1,Preset::Kind::factory},
    {"MBC-3000",6,-.15f,1,true,80,false,.42f,0,.18f,.3f,.12f,0,1,Preset::Kind::factory},
    {"AQS-900",7,-.30f,1,false,100,false,.55f,1,.16f,.65f,.3f,0,1,Preset::Kind::factory},
    {"AQS-950",8,-.20f,1,true,90,false,.6f,1,.21f,.6f,.24f,0,1,Preset::Kind::factory},
    {"AQS-1000",9,-.10f,1,false,100,false,.32f,0,.15f,.25f,.1f,0,1,Preset::Kind::factory},
    {"AQS-3000XL",10,-.10f,1,true,70,false,.26f,0,.12f,.2f,.07f,0,1,Preset::Kind::factory},
    {"EMV-12",11,-.35f,1,true,100,false,.58f,1,.31f,.8f,.28f,0,1,Preset::Kind::factory},
    {"EMV-1200",12,-.30f,1,true,110,false,.62f,1,.31f,.85f,.26f,0,1,Preset::Kind::factory},
    {"EMV-II",13,-.45f,1,false,100,false,.5f,2,.29f,.7f,.35f,0,1,Preset::Kind::factory},
    {"EMV-III",14,-.05f,1,false,100,false,.28f,0,.12f,.2f,.08f,0,1,Preset::Kind::factory},
    {"EMV-ESI32",15,-.10f,1,true,90,false,.4f,0,.18f,.4f,.14f,0,1,Preset::Kind::factory},
    {"EMV-ESI4000",16,-.05f,1,false,100,false,.28f,0,.12f,.28f,.1f,0,1,Preset::Kind::factory},
    {"EMV-E4X",17,0,1,false,100,false,.22f,0,.1f,.18f,.06f,0,1,Preset::Kind::factory},
    {"EMV-E64",18,0,1,false,100,false,.2f,0,.07f,.15f,.05f,0,1,Preset::Kind::factory},
    {"PS-202",19,-.35f,1,false,100,false,.4f,0,.25f,.55f,.3f,0,1,Preset::Kind::factory},
    {"PS-303",20,-.35f,1,true,80,false,.42f,0,.24f,.4f,.35f,0,1,Preset::Kind::factory},
    {"PS-404",21,-.20f,1,true,90,false,.48f,0,.27f,.5f,.22f,0,1,Preset::Kind::factory},
    {"PS-404SX",22,-.15f,1,true,90,false,.44f,0,.24f,.45f,.18f,0,1,Preset::Kind::factory},
    {"PS-404MK2",23,-.05f,1,true,90,false,.36f,0,.14f,.35f,.14f,0,1,Preset::Kind::factory},
    {"PS-505",24,-.10f,1,false,100,false,.4f,0,.21f,.42f,.18f,0,1,Preset::Kind::factory},
    {"PS-555",25,-.10f,1,true,85,false,.42f,0,.21f,.4f,.16f,0,1,Preset::Kind::factory},
    {"PS-606",26,-.10f,1,false,100,false,.36f,0,.18f,.35f,.14f,0,1,Preset::Kind::factory},
    {"PS-808",27,-.10f,1,false,100,false,.38f,0,.18f,.35f,.14f,0,1,Preset::Kind::factory},
    {"RLD-50",28,-.30f,1,false,100,false,.48f,1,.25f,.5f,.22f,0,1,Preset::Kind::factory},
    {"RLD-550",29,-.25f,1,true,90,false,.52f,1,.25f,.52f,.22f,0,1,Preset::Kind::factory},
    {"RLD-770",30,-.05f,1,false,100,false,.26f,0,.12f,.22f,.08f,0,1,Preset::Kind::factory},
    {"RLD-DJ70",31,-.25f,1,false,100,false,.58f,1,.33f,.75f,.3f,0,1,Preset::Kind::factory},
    {"ENQ-Mirage",32,-.40f,1,false,100,false,.52f,2,.24f,.75f,.4f,0,1,Preset::Kind::factory},
    {"ENQ-EPS",33,-.15f,1,false,100,false,.5f,1,.21f,.55f,.2f,0,1,Preset::Kind::factory},
    {"ENQ-EPS16",34,-.10f,1,false,100,false,.38f,0,.2f,.4f,.14f,0,1,Preset::Kind::factory},
    {"ENQ-ASR10",35,-.10f,1,false,100,false,.4f,0,.15f,.38f,.14f,0,1,Preset::Kind::factory},
    {"KRG-DSS1",36,-.25f,1,false,100,false,.45f,1,.21f,.6f,.28f,0,1,Preset::Kind::factory},
    {"CSO-RZ1",37,-.40f,1,false,100,false,.5f,2,.35f,.8f,.35f,0,1,Preset::Kind::factory},
    {"CSO-FZ1",38,.10f,1,false,100,false,.38f,0,.23f,.6f,.2f,0,1,Preset::Kind::factory},
    {"MBC DRUMS",0,-.15f,1,true,80,false,.5f,1,.18f,.45f,.08f,0,1,Preset::Kind::factory},
    {"PS DRUNK",21,-.30f,1,false,100,false,.35f,0,.22f,.4f,.22f,.60f,1,Preset::Kind::factory},
    {"EMV GRIT",12,-.20f,1,true,100,false,.4f,1,.35f,.85f,.2f,0,1,Preset::Kind::factory},
    {"AQS CRUNCH",8,-.10f,1,true,90,false,.55f,1,.28f,.65f,.18f,0,1,Preset::Kind::factory},
    {"DUSTY KEYS",28,-.40f,1,false,100,false,.25f,1,.24f,.4f,.28f,0,1,Preset::Kind::factory},
    {"VINYL CRACKLE",20,-.50f,.70f,false,100,true,.2f,1,.2f,.3f,.45f,0,1,Preset::Kind::factory},
    {"TAPE WARM",14,-.15f,1,true,60,false,.4f,0,.08f,.2f,.15f,0,1,Preset::Kind::factory},
    {"BOOM BAP CLASSIC",6,-.20f,1,true,80,false,.3f,1,.26f,.5f,.12f,0,1,Preset::Kind::factory},
    {"DIRTY FUNK",4,.35f,1,true,150,false,.55f,2,.2f,.6f,.15f,0,1,Preset::Kind::factory},
    {"WARM & ROUND",35,-.30f,1,true,70,false,.22f,1,.12f,.25f,.1f,0,1,Preset::Kind::factory},
    {"CRUSHED BREAKS",7,0,1,false,100,false,.45f,2,.4f,.9f,.1f,0,1,Preset::Kind::factory},
}};
} // namespace junky
