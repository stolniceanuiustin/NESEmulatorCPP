#include <stdint.h>
#include <fstream>
#include <iostream>
#include "../include/emulator_config.h"
#include "../include/memory.h"
#include "../include/mapper.h"
#include "../include/cartridge.h"
bool mapper0(Config &config, CARTRIDGE& cartridge, std::ifstream &rom)
{
    int prg_size = cartridge.get_prg_size() * 16 * 1024;
    int chr_size = cartridge.get_chr_size() * 8 * 1024;
    std::cout << "READING FILE WITH MAPPER0\n" << std::flush;
    rom.read(reinterpret_cast<char *>(cartridge.get_PRGrom()), prg_size);
    if (prg_size == 0x4000)
    {
        std::memcpy(cartridge.get_PRGrom() + 0x4000, cartridge.get_PRGrom(), 0x4000);
    }
    else
    {
        rom.read(reinterpret_cast<char *>(cartridge.get_PRGrom()), 0x8000);
        rom.close();
    }
    if(chr_size > 0 && chr_size <= 0x3FFF)
    {
        rom.read(reinterpret_cast<char *>(cartridge.get_CHRrom()), chr_size);
    }
    else
    {
        std::cerr << "FAILED LOADING CHR ROM;\n";
    }
    return true;
}

uint16_t Mapper0::cpu_map_read(uint16_t addr)
{
    if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        return addr & (0x7FFF);
    }
    else return 0;
}
uint16_t Mapper0::cpu_map_write(uint16_t addr)
{
    if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        return addr & 0x7FFF;
    }
    else return 0;
}
uint16_t Mapper0::ppu_map_read(uint16_t addr)
{
    return addr;
}
//ONLY FOR games with CHRram, will have to implement later; TODO
uint16_t Mapper0::ppu_map_write(uint16_t addr) 
{
    return 0;
}