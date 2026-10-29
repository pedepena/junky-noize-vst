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
    {"MBC-60",0,-.10f,1,true,90,false,.48f,1,.14f,.55f,.18f,0,.95f,Preset::Kind::factory},
    {"MBC-60 II",1,-.06f,1,true,90,false,.34f,1,.11f,.48f,.12f,0,.90f,Preset::Kind::factory},
    {"MBC-500",2,-.03f,1,false,100,false,.30f,0,.12f,.42f,.10f,0,.90f,Preset::Kind::factory},
    {"MBC-1000",3,-.08f,1,true,95,false,.32f,0,.13f,.45f,.10f,0,.95f,Preset::Kind::factory},
    {"MBC-2000",4,-.06f,1,true,100,false,.42f,0,.15f,.55f,.12f,0,1,Preset::Kind::factory},
    {"MBC-2000XL",5,.08f,1,false,100,false,.32f,0,.12f,.43f,.08f,0,.92f,Preset::Kind::factory},
    {"MBC-3000",6,-.12f,1,true,80,false,.40f,0,.16f,.46f,.12f,0,.98f,Preset::Kind::factory},
    {"AKS-900",7,-.34f,.92f,false,100,false,.58f,1,.58f,.82f,.28f,0,.95f,Preset::Kind::factory},
    {"AKS-950",8,-.20f,1,false,100,false,.48f,1,.40f,.68f,.20f,0,.92f,Preset::Kind::factory},
    {"AKS-1000",9,-.06f,1,true,100,false,.32f,0,.14f,.44f,.08f,0,.95f,Preset::Kind::factory},
    {"AKS-3000XL",10,.10f,1,false,100,false,.28f,0,.10f,.40f,.05f,0,.90f,Preset::Kind::factory},
    {"EMV-SP12",11,-.20f,1,true,100,false,.48f,1,.30f,.68f,.22f,0,1,Preset::Kind::factory},
    {"EMV-SP1200",12,-.22f,1,true,110,false,.56f,1,.30f,.88f,.22f,0,1,Preset::Kind::factory},
    {"EMV-EMULATOR II",13,-.44f,.92f,false,100,false,.48f,2,.32f,.76f,.30f,0,.96f,Preset::Kind::factory},
    {"EMV-EMULATOR III",14,-.04f,1,false,100,false,.28f,0,.12f,.36f,.06f,0,.92f,Preset::Kind::factory},
    {"EMV-ESI32",15,-.18f,1,true,95,false,.36f,0,.18f,.46f,.12f,0,.94f,Preset::Kind::factory},
    {"EMV-ESI4000",16,.04f,1,false,100,false,.30f,0,.12f,.44f,.08f,0,.94f,Preset::Kind::factory},
    {"EMV-E4XT",17,.08f,1,false,100,false,.26f,0,.09f,.38f,.04f,0,.92f,Preset::Kind::factory},
    {"EMV-E6400",18,.03f,1,false,100,false,.27f,0,.08f,.40f,.05f,0,.94f,Preset::Kind::factory},
    {"RLD-S50",28,.02f,1,false,100,false,.48f,1,.28f,.72f,.18f,0,.94f,Preset::Kind::factory},
    {"RLD-S550",29,-.20f,1,true,85,false,.46f,1,.25f,.62f,.16f,0,.94f,Preset::Kind::factory},
    {"RLD-S770",30,.02f,1,false,100,false,.30f,0,.12f,.42f,.08f,0,.92f,Preset::Kind::factory},
    {"RLD-DJ70",31,-.30f,1,false,100,false,.50f,1,.34f,.72f,.22f,0,.96f,Preset::Kind::factory},
    {"PS-202 DR.SAMPLE",19,-.22f,1,false,100,false,.38f,0,.24f,.60f,.12f,0,.96f,Preset::Kind::factory},
    {"PS-303",20,-.30f,1,false,100,false,.38f,0,.22f,.56f,.34f,0,.95f,Preset::Kind::factory},
    {"PS-404",21,-.16f,1,false,100,false,.52f,0,.24f,.58f,.12f,0,.96f,Preset::Kind::factory},
    {"PS-404SX",22,-.28f,1,false,100,false,.48f,0,.22f,.54f,.10f,0,.96f,Preset::Kind::factory},
    {"PS-404MK2",23,.05f,1,false,100,false,.38f,0,.14f,.46f,.06f,0,.94f,Preset::Kind::factory},
    {"PS-555",25,-.16f,1,true,80,false,.42f,0,.20f,.50f,.10f,0,.98f,Preset::Kind::factory},
    {"PS-500",24,.10f,1,false,100,false,.30f,0,.11f,.42f,.05f,0,.90f,Preset::Kind::factory},
    {"ENS-MIRAGE",32,-.38f,1,false,100,false,.50f,2,.40f,.78f,.26f,0,.96f,Preset::Kind::factory},
    {"ENS-EPS",33,-.14f,1,true,100,false,.40f,1,.20f,.52f,.12f,0,.96f,Preset::Kind::factory},
    {"ENS-EPS16",34,-.10f,1,true,100,false,.34f,0,.16f,.48f,.08f,0,.96f,Preset::Kind::factory},
    {"ENS-ASR10",35,.08f,1,false,100,false,.38f,0,.14f,.52f,.08f,0,.96f,Preset::Kind::factory},
    {"KRG-DSS1",36,-.32f,1,false,100,false,.42f,1,.22f,.60f,.16f,0,.96f,Preset::Kind::factory},
    {"CSO-RZ1",37,-.42f,1,false,100,false,.50f,2,.24f,.82f,.20f,0,.96f,Preset::Kind::factory},
    {"CSO-FZ1",38,.16f,1,false,100,false,.38f,0,.22f,.62f,.08f,0,.94f,Preset::Kind::factory},
    {"MBC DRUMS",0,-.12f,1,true,80,false,.48f,1,.18f,.56f,.08f,0,1,Preset::Kind::factory},
    {"PS DRUNK",21,-.28f,1,false,100,false,.36f,0,.22f,.54f,.10f,0,.96f,Preset::Kind::factory},
    {"EMV GRIT",12,-.20f,1,true,100,false,.48f,1,.34f,.84f,.18f,0,1,Preset::Kind::factory},
    {"DUSTY TAPE",19,-.40f,1,false,100,false,.26f,1,.18f,.44f,.36f,0,.98f,Preset::Kind::factory},
    {"LO-FI KEYS",34,-.16f,1,false,100,false,.30f,1,.18f,.40f,.14f,0,.94f,Preset::Kind::factory},
    {"BROKEN VINYL",20,-.46f,.82f,false,100,true,.24f,1,.20f,.48f,.62f,0,.96f,Preset::Kind::factory},
    {"TRASH BREAK",7,-.10f,.92f,false,100,false,.62f,3,.46f,.92f,.16f,0,1,Preset::Kind::factory},
    {"SUB WARM",6,-.22f,1,true,65,false,.28f,1,.12f,.38f,.08f,0,1,Preset::Kind::factory},
    {"CRUSH BUS",4,0,1,false,100,false,.72f,1,.20f,.70f,.10f,0,.98f,Preset::Kind::factory},
    {"RADIO DUST",31,-.48f,.88f,false,100,true,.32f,1,.28f,.64f,.38f,0,.96f,Preset::Kind::factory},
    {"BASEMENT",13,-.52f,1,false,100,false,.34f,2,.25f,.62f,.20f,0,.98f,Preset::Kind::factory},
    {"SMOKY LOOP",3,-.20f,1,true,90,false,.24f,0,.10f,.30f,.10f,0,.94f,Preset::Kind::factory},
    {"MELT",14,-.58f,.94f,false,100,false,.20f,1,.16f,.20f,.18f,0,.98f,Preset::Kind::factory}
}};
} // namespace junky
