#include "../include/cartridge.h"
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

bool CARTRIDGE::read_file()
{
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
    std::cout << "Mapper_type: " << (int)mapper_type << '\n';
    byte nametable_type = (header.flags6 & 1);
    if(nametable_type == 0)
    {
        //Vertical mirroring
        std::cout << "Using vertical mirroring\n";
        mirroring = VERTICAL;
        set_mapping(0, 0x400, 0, 0x400);
    }
    else if(nametable_type == 1)
    {
        mirroring = HORIZONTAL;
        std::cout << "Using horizontal mirroring\n";
        set_mapping(0, 0, 0x400, 0x400);
    }
    config.nametable_arrangement = (nametable_type == 1) ? 1 : 0;
    if(mapper_type == 0)
    {
        //TODO CHECK THIS ASAP
        std::cout << "Using MAPPERTYPE = 0" << std::endl;
        p_mapper = std::make_shared<Mapper0>(header.prg_size, header.chr_size);
        mapper0(config, *this, rom);
    }
    return true;
}

byte CARTRIDGE::cpu_read(uint16_t addr)
{
    uint16_t mapped_addr = p_mapper->cpu_map_read(addr);
    return PRGrom[mapped_addr];
}
void CARTRIDGE::cpu_write(uint16_t addr, byte data)
{
    uint16_t mapped_addr = p_mapper->cpu_map_write(addr);
    PRGrom[mapped_addr] = data;
}

byte CARTRIDGE::ppu_read(uint16_t addr)
{
    uint16_t mapped_addr = p_mapper->ppu_map_read(addr);
    return CHRrom[mapped_addr];
}

void CARTRIDGE::ppu_write(uint16_t addr, byte data)
{
    uint16_t mapped_addr = p_mapper->ppu_map_write(addr);
    CHRrom[mapped_addr] = addr;
}