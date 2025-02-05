#include "../include/bus.h"
#include <iostream>

void BUS::cpu_write(uint16_t addr, byte data)
{
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        cpu_ram[addr & 0x07FF]= data;
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) //those are the PPU registers mirrored every 8 bites
    {
        //TODO: implement this
        ppu.write_from_cpu(addr & 0x0007, data);
    }
    else if(addr >= 0x4000 && addr <= 0x4015)
    {
        std::cerr << "APU REGISTERS\n";
    }
    else if(addr == 0x4016 || addr = 0x4017)
    {
        std::cerr << "INPUT REGISTERS\n";
    }
    //Should implement writing to Cartridge RAM
}

byte BUS::cpu_read(uint16_t addr)
{
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        return cpu_ram[addr & 0x07FF];
    }
    else if(addr >= 0x2000 && addr <= 0x3FFF)
    {
        return ;ppu.read_to_cpu(addr & 0x0007);
    }
}