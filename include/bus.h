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
    byte controller[2];
    byte controller_state[2];
    bool dma_transfer;
    byte oam_dma_page;
    byte oam_dma_addr;
    BUS(CPU &cpu, PPU &ppu, CARTRIDGE& cartridge) : cpu(cpu), ppu(ppu), cartridge(cartridge), cpu_ram(0x0800), ppu_ram(0x3FFF) {
        dma_transfer = false;
    };
    
    void cpu_write(uint16_t addr, byte data);
    void ppu_write(uint16_t addr, byte data);
    byte cpu_read(uint16_t addr);
    byte ppu_read(uint16_t addr);
    void reset();
    void hexdump();
};

#endif