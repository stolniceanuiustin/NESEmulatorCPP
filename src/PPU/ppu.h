#ifndef HEADERGUARD_PPU
#define headerguard

#include <stdint.h>
#include "../Memory/memory.h"

typedef uint8_t byte;

class PPU {
public:
    Memory &shared_ram;  //SHARED RAM
    PPU(Memory &ram) : shared_ram(ram) {}

    byte internal_ram[0x3FFF];
    byte OAM[256];
    
};

#endif