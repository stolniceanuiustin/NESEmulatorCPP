
#include "mapper.h"
bool mapper(Config &config, Memory& ram)
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
    
    byte mapper = (header.flags6 && 0xF0 >> 4) | (header.flags7 && 0xF0);
    byte nametable = (header.flags6 & 1);
    config.nametable_arrangement = (nametable == 1) ? 1 : 0;
    if(mapper == 0)
    {
        mapper0(config, ram, header, rom);
    }
    return true;
}