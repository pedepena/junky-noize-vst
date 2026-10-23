#pragma once

#include <array>

namespace junky
{
struct MachineProfile
{
    const char* name;
    float lpHz;
    float lowShelfDb;
    float highShelfDb;
    bool companded;
};

inline constexpr std::array<MachineProfile, 39> machineProfiles {{
    {"MBC-60",16000,3,-1,false},{"MBC-60 II",17000,2.5f,-.5f,false},{"MBC-500",18000,1.5f,0,false},
    {"MBC-1000",18000,1,0,false},{"MBC-2000",17000,2,-.5f,false},{"MBC-2000XL",18000,1.5f,.5f,false},
    {"MBC-3000",17000,2.5f,-1,false},{"AQS-900",12000,1.5f,-3,false},{"AQS-950",14000,2,-2,false},
    {"AQS-1000",17000,1.5f,-.5f,false},{"AQS-3000XL",18000,2,0,false},{"EMV-12",10000,2,-3,false},
    {"EMV-1200",11000,3,-2.5f,false},{"EMV-II",9000,2,-4,true},{"EMV-III",18000,1,0,false},
    {"EMV-ESI32",16000,2,-1,false},{"EMV-ESI4000",18000,1,0,false},{"EMV-E4X",19000,1,.5f,false},
    {"EMV-E64",19000,.5f,.5f,false},{"PS-202",10000,1,-3,false},{"PS-303",12000,2,-3,false},
    {"PS-404",13000,2.5f,-1.5f,false},{"PS-404SX",14000,2,-1,false},{"PS-404MK2",16000,2,0,false},
    {"PS-505",14000,1.5f,-1,false},{"PS-555",15000,2,-.5f,false},{"PS-606",15000,1.5f,-.5f,false},
    {"PS-808",15000,1.5f,-.5f,false},{"RLD-50",12000,2,-3,false},{"RLD-550",13000,2.5f,-2.5f,false},
    {"RLD-770",18000,1.5f,0,false},{"RLD-DJ70",11000,1,-2,false},{"ENQ-Mirage",10000,2,-3.5f,false},
    {"ENQ-EPS",13000,1.5f,-1.5f,false},{"ENQ-EPS16",16000,1.5f,-1,false},{"ENQ-ASR10",16000,2,-.5f,false},
    {"KRG-DSS1",12000,1,-2.5f,false},{"CSO-RZ1",8000,1,-4,false},{"CSO-FZ1",15000,.5f,1.5f,false}
}};
} // namespace junky
