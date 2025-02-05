#ifndef BUS_HEADERGUARD
#define BUS_HEADERGUARD

#include <stdint.h>
#include "cpu_header.h"
#include "ppu.h"

class BUS
{
public:
    CPU &cpu;
    PPU &ppu;
    byte cpu_ram[0x0800];
    BUS(CPU &cpu, PPU &ppu)
    cpu(cpu), ppu(ppu) {};
    ~BUS();

    void cpu_write(uint16_t addr, byte data);
    void ppu_write(uint16_t addr, byte data);
};

#endif