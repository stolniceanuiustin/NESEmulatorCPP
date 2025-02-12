#ifndef mapper_hg
#define mapper_hg
#include <stdint.h>
#include <fstream>
#include <iostream>
#include "emulator_config.h"
#include "memory.h"

class CARTRIDGE;

bool mapper0(Config &config, CARTRIDGE &cartridge, std::ifstream &rom);
class Mapper
{
public:
    uint8_t prg_banks;
    uint8_t chr_banks;

public:
    Mapper(uint8_t prg_banks, uint8_t chr_banks) : prg_banks(prg_banks), chr_banks(chr_banks) {};
    virtual uint16_t cpu_map_read(uint16_t addr) = 0;
    virtual uint16_t cpu_map_write(uint16_t addr) = 0;
    virtual uint16_t ppu_map_read(uint16_t addr) = 0;
    virtual uint16_t ppu_map_write(uint16_t addr) = 0;
};

//SOME MAPPERS MIGHT DIFFER FROM CPU READ AND WRITE
class Mapper0 : public Mapper
{
public:
    Mapper0(uint8_t prg_banks, uint8_t chr_banks) : Mapper(prg_banks, chr_banks) {};
    uint16_t cpu_map_read(uint16_t addr) override;
    uint16_t cpu_map_write(uint16_t addr) override;
    uint16_t ppu_map_read(uint16_t addr) override;
    uint16_t ppu_map_write(uint16_t addr) override;
};

#endif