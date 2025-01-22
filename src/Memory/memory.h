#ifndef MEMORY_HEADERGUARD
#define MEMORY_HEADERGUARD
#include <stdint.h>
#include <cstring>
#include <iostream>

typedef uint8_t byte;

class Memory
{
private:
    byte ram[65536];
public:
    Memory()
    {
        memset(ram, 0, 65536);
    }
    byte& operator[](uint16_t address)
    {
        if(address >= 0 && address <= 0xFFFF)
        {
            return ram[address];
        }
        else{
            std::cerr << "Out of bounds index for memory read\n";
            exit(-1);
        }
    }
    void write(uint16_t address, byte src)
    {
        if(address >= 0 && address <= 0xFFFF)
        {
            ram[address] = src;
        }
        else
        {
            std::cerr << "Out of bounds index for memory write\n";
            exit(-1);
        }
    }

};







#endif