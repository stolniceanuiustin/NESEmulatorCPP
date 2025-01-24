#ifndef mapper_hg
#define mapper_hg
#include <stdint.h>
#include <fstream>
#include <iostream>
#include "../emulator_config.h"
#include "../Memory/memory.h"

bool mapper(Config &config, Memory& ram);
bool mapper0(Config &config, Memory& ram, NESHeader header, std::ifstream &rom);
#endif