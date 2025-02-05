#ifndef CARTRIDGE_HEADERGUARD
#define CARTRIDGE_HEADERGUARD
#include <stdint.h>
#include "emulator_config.h"
#include "memory.h"
typedef uint8_t byte;

class Cartridge
{
private:
    Config config;
    byte PRGrom[0x8000];
    byte CHRrom[0x4000];
public: 
    Cartridge(Config config) : config(config)
    {};
    bool read_file(Config &config, Memory& ram, Memory& ppu_ram);

}



#endif