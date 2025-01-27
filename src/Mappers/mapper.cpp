#include "../include/mapper.h"
#include <iostream>

Nametable_Map nametablee;
void set_mapping(uint16_t top_left, uint16_t top_right, uint16_t bottom_left, uint16_t bottom_right)
{
    nametablee.map[0] = top_left;
    nametablee.map[1] = top_right;
    nametablee.map[2] = bottom_left;
    nametablee.map[3] = bottom_right;
}

bool mapper(Config &config, Memory& ram, Memory& ppu_ram)
{
    std::cout << "RAM ADDRESS IN mapper:" << &ram << '\n';
    std::ifstream rom(config.rom_name, std::ios::binary);
    if (!rom)
    {
        std::cerr << "Could not open ROM FILE\n";
        return false;
    }
    NESHeader header;
    rom.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (header.magic[0] != 'N' || header.magic[1] != 'E' || header.magic[2] != 'S' || header.magic[3] != 0x1A)
    {
        std::cerr << "Invalid NES file: Missing iNES header.\n";
        return false;
    }
    
    byte mapper_type = (header.flags6 && 0xF0 >> 4) | (header.flags7 && 0xF0);
    byte nametable_type = (header.flags6 & 1);
    if(nametable_type == 0)
    {
        //Vertical mirroring
        std::cout << "Using vertical mirroring\n";
        set_mapping(0, 0x400, 0, 0x400);
    }
    else if(nametable_type == 1)
    {
        std::cout << "Using horizontal mirroring\n";
        set_mapping(0, 0, 0x400, 0x400);
    }
    config.nametable_arrangement = (nametable_type == 1) ? 1 : 0;
    if(mapper_type == 0)
    {
        mapper0(config, ram, ppu_ram, header, rom);
    }
    return true;
}
