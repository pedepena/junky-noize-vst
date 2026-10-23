#pragma once
#include <array>
namespace junky
{
struct Preset
{
    const char* name; int machine; float filter, mix; bool bassBoost; float bassFreq; bool mono;
    float drive; int bitsIndex; float rate, alias, noise, drunk, character;
    enum class Kind { init, factory, custom } kind;
};
inline constexpr std::array<Preset, 55> factoryPresets {{
    {"INIT",-1,0,1,false,100,false,0,0,0,.5f,0,0,1,Preset::Kind::init},
    {"MBC-60",0,-.10f,1,true,90,false,.48f,1,0,0,0,0,1,Preset::Kind::factory},
    {"MBC-60 II",1,0,1,true,85,false,.42f,1,0,0,0,0,1,Preset::Kind::factory},
    {"MBC-500",2,-.05f,1,false,100,false,.30f,0,0,0,0,0,1,Preset::Kind::factory},
    {"MBC-1000",3,-.05f,1,false,100,false,.28f,0,0,0,0,0,1,Preset::Kind::factory},
    {"MBC-2000",4,-.10f,1,true,100,false,.38f,0,0,0,0,0,1,Preset::Kind::factory},
    {"MBC-2000XL",5,0,1,true,100,false,.35f,0,0,0,0,0,1,Preset::Kind::factory},
    {"MBC-3000",6,-.15f,1,true,80,false,.42f,0,0,0,0,0,1,Preset::Kind::factory},
    {"AQS-900",7,-.30f,1,false,100,false,.55f,1,0,0,0,0,1,Preset::Kind::factory},
    {"AQS-950",8,-.20f,1,true,90,false,.60f,1,0,0,0,0,1,Preset::Kind::factory},
    {"AQS-1000",9,-.10f,1,false,100,false,.32f,0,0,0,0,0,1,Preset::Kind::factory},
    {"AQS-3000XL",10,-.10f,1,true,70,false,.26f,0,0,0,0,0,1,Preset::Kind::factory},
    {"EMV-12",11,-.35f,1,true,100,false,.58f,1,0,0,0,0,1,Preset::Kind::factory},
    {"EMV-1200",12,-.30f,1,true,110,false,.62f,1,0,0,0,0,1,Preset::Kind::factory},
    {"EMV-II",13,-.45f,1,false,100,false,.50f,2,0,0,0,0,1,Preset::Kind::factory},
    {"EMV-III",14,-.05f,1,false,100,false,.28f,0,0,0,0,0,1,Preset::Kind::factory},
    {"EMV-ESI32",15,-.10f,1,true,90,false,.40f,0,0,0,0,0,1,Preset::Kind::factory},
    {"EMV-ESI4000",16,-.05f,1,false,100,false,.28f,0,0,0,0,0,1,Preset::Kind::factory},
    {"EMV-E4X",17,0,1,false,100,false,.22f,0,0,0,0,0,1,Preset::Kind::factory},
    {"EMV-E64",18,0,1,false,100,false,.20f,0,0,0,0,0,1,Preset::Kind::factory},
    {"PS-202",19,-.35f,1,false,100,false,.40f,0,0,0,0,0,1,Preset::Kind::factory},
    {"PS-303",20,-.35f,1,true,80,false,.42f,0,0,0,0,0,1,Preset::Kind::factory},
    {"PS-404",21,-.20f,1,true,90,false,.48f,0,0,0,0,0,1,Preset::Kind::factory},
    {"PS-404SX",22,-.15f,1,true,90,false,.44f,0,0,0,0,0,1,Preset::Kind::factory},
    {"PS-404MK2",23,-.05f,1,true,90,false,.36f,0,0,0,0,0,1,Preset::Kind::factory},
    {"PS-505",24,-.10f,1,false,100,false,.40f,0,0,0,0,0,1,Preset::Kind::factory},
    {"PS-555",25,-.10f,1,true,85,false,.42f,0,0,0,0,0,1,Preset::Kind::factory},
    {"PS-606",26,-.10f,1,false,100,false,.36f,0,0,0,0,0,1,Preset::Kind::factory},
    {"PS-808",27,-.10f,1,false,100,false,.38f,0,0,0,0,0,1,Preset::Kind::factory},
    {"RLD-50",28,-.30f,1,false,100,false,.48f,1,0,0,0,0,1,Preset::Kind::factory},
    {"RLD-550",29,-.25f,1,true,90,false,.52f,1,0,0,0,0,1,Preset::Kind::factory},
    {"RLD-770",30,-.05f,1,false,100,false,.26f,0,0,0,0,0,1,Preset::Kind::factory},
    {"RLD-DJ70",31,-.25f,1,false,100,false,.58f,1,0,0,0,0,1,Preset::Kind::factory},
    {"ENQ-Mirage",32,-.40f,1,false,100,false,.52f,2,0,0,0,0,1,Preset::Kind::factory},
    {"ENQ-EPS",33,-.15f,1,false,100,false,.50f,1,0,0,0,0,1,Preset::Kind::factory},
    {"ENQ-EPS16",34,-.10f,1,false,100,false,.38f,0,0,0,0,0,1,Preset::Kind::factory},
    {"ENQ-ASR10",35,-.10f,1,false,100,false,.40f,0,0,0,0,0,1,Preset::Kind::factory},
    {"KRG-DSS1",36,-.25f,1,false,100,false,.45f,1,0,0,0,0,1,Preset::Kind::factory},
    {"CSO-RZ1",37,-.40f,1,false,100,false,.50f,2,0,0,0,0,1,Preset::Kind::factory},
    {"CSO-FZ1",38,.10f,1,false,100,false,.38f,0,0,0,0,0,1,Preset::Kind::factory},
    {"MBC DRUMS",0,-.15f,1,true,80,false,.50f,1,.18f,.45f,.08f,0,1,Preset::Kind::factory},
    {"PS DRUNK",21,-.30f,1,false,100,false,.35f,0,.22f,.40f,.22f,.60f,1,Preset::Kind::factory},
    {"EMV GRIT",12,-.20f,1,true,100,false,.40f,1,.35f,.85f,.20f,0,1,Preset::Kind::factory},
    {"AQS CRUNCH",8,-.10f,1,true,90,false,.55f,1,.28f,.65f,.18f,0,1,Preset::Kind::factory},
    {"DUSTY KEYS",28,-.40f,1,false,100,false,.25f,1,.24f,.40f,.28f,0,1,Preset::Kind::factory},
    {"VINYL CRACKLE",20,-.50f,.70f,false,100,true,.20f,1,.20f,.30f,.45f,0,1,Preset::Kind::factory},
    {"TAPE WARM",14,-.15f,1,true,60,false,.40f,0,.08f,.20f,.15f,0,1,Preset::Kind::factory},
    {"BOOM BAP CLASSIC",6,-.20f,1,true,80,false,.30f,1,.26f,.50f,.12f,0,1,Preset::Kind::factory},
    {"DIRTY FUNK",4,.35f,1,true,150,false,.55f,2,.20f,.60f,.15f,0,1,Preset::Kind::factory},
    {"WARM & ROUND",35,-.30f,1,true,70,false,.22f,1,.12f,.25f,.10f,0,1,Preset::Kind::factory},
    {"CRUSHED BREAKS",7,0,1,false,100,false,.45f,2,.40f,.90f,.10f,0,1,Preset::Kind::factory},
    {"CUSTOM 1",-1,0,1,false,100,false,0,0,0,.5f,0,0,1,Preset::Kind::custom},
    {"CUSTOM 2",-1,0,1,false,100,false,0,0,0,.5f,0,0,1,Preset::Kind::custom},
    {"CUSTOM 3",-1,0,1,false,100,false,0,0,0,.5f,0,0,1,Preset::Kind::custom},
    {"CUSTOM 4",-1,0,1,false,100,false,0,0,0,.5f,0,0,1,Preset::Kind::custom}
}};
} // namespace junky
