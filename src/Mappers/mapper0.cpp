#include <stdint.h>
#include <fstream>
#include <iostream>
#include "../emulator_config.h"
#include "../Memory/memory.h"
#include "mapper.h"
bool mapper0(Config &config, Memory& ram, Memory& ppu_ram, NESHeader header, std::ifstream &rom)
{
    std::cout << "RAM ADDRESS IN mapper0:" << &ram << '\n';
    int prg_size = header.prg_size * 16 * 1024;
    int chr_size = header.chr_size * 8 * 1024;

    rom.read(reinterpret_cast<char *>(&ram[0x8000]), prg_size);
    if (prg_size == 0x4000)
    {
        std::memcpy(&ram[0xC000], &ram[0x8000], 0x4000);
    }
    else
    {
        rom.read(reinterpret_cast<char *>(&ram[0x8000]), 0xFFFF);
        rom.close();
    }
    if(chr_size > 0 && chr_size <= 0x3FFF)
    {
        rom.read(reinterpret_cast<char *>(&ppu_ram[0x0000]), chr_size);
    }
    else
    {
        std::cerr << "FAILED LOADING CHR ROM;\n";
    }
    return true;
}