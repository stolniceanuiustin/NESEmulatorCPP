#ifndef BUS_HEADERGUARD
#define BUS_HEADERGUARD
#include <stdint.h>
#include "memory.h"
typedef uint8_t byte;
class CPU;
class PPU;
class Memory;
class CARTRIDGE;

class BUS
{
public:
    CPU &cpu;
    PPU &ppu;
    CARTRIDGE& cartridge;
    Memory cpu_ram;
    Memory ppu_ram;
    BUS(CPU &cpu, PPU &ppu, CARTRIDGE& cartridge) : cpu(cpu), ppu(ppu), cartridge(cartridge), cpu_ram(0x0800), ppu_ram(0x3FFF) {};

    void cpu_write(uint16_t addr, byte data);
    void ppu_write(uint16_t addr, byte data);
    byte cpu_read(uint16_t addr);

    void hexdump();
};

#endif