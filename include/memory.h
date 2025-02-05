#ifndef MEMORY_HEADERGUARD
#define MEMORY_HEADERGUARD
#include <stdint.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include "ppu.h"
typedef uint8_t byte;

class Memory
{
private:
    byte *memory;
    int size;
    PPU& ppu;
public:
    Memory(int size, PPU& ppu) : size(size), ppu(ppu);
    {
        if (size <= 65536)
        {
            memory = new byte[size];
        }
        else
        {
            std::cerr << "You are trying to allocate too much memory!\n";
        }
        memset(memory, 0, size);
    }
    Memory &operator=(const Memory &) = delete;
    byte &operator[](uint16_t address)
    {
        if (size == 65536)
        {
            // this means CPU memory space
            if (address >= 0 && address <= 0xFFFF)
            {
                return memory[address];
            }
            else
            {
                std::cerr << "Out of bounds index for memory read\n";
                exit(-1);
            }
        }
    }
    byte *get_address(uint16_t address)
    {
        return &memory[address];
    }
    void cpu_write(uint16_t address, byte src)
    {
        if (address >= 0 && address <= 0xFFFF)
        {
            memory[address] = src;
        }
        else
        {
            std::cerr << "Out of bounds index for memory write\n";
            exit(-1);
        }
    }
    void hexdump(char *filename, uint16_t size)
    {
        FILE *file = fopen(filename, "wb");
        fwrite(memory, sizeof(byte), size, file);
        fclose(file);
    }
};

#endif