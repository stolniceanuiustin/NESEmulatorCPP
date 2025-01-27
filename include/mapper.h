#ifndef mapper_hg
#define mapper_hg
#include <stdint.h>
#include <fstream>
#include <iostream>
#include "emulator_config.h"
#include "memory.h"

bool mapper(Config &config, Memory& ram, Memory& ppu_ram);
bool mapper0(Config &config, Memory& ram, Memory& ppu_ram, NESHeader header, std::ifstream &rom);
struct Nametable_Map
{
    uint16_t map[4];
};

#endif