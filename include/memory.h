#ifndef MEMORY_HEADERGUARD
#define MEMORY_HEADERGUARD
#include <stdint.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
typedef uint8_t byte;

class Memory
{
private:
    byte *memory;
    int size;
public:
    Memory(int size) : size(size)
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
            if (address >= 0 && address < size)
            {
                return memory[address];
            }
            else
            {
                std::cerr << "Out of bounds index for memory read\n";
                exit(-1);
            }
    }

    void reset()
    {
        std::memset(memory, 0, size);
    }


    void hexdump(char *filename, uint16_t size)
    {
        FILE *file = fopen(filename, "wb");
        if(file)
        {
            fwrite(memory, sizeof(byte), size, file);
            fclose(file);
        }
        else
        {
            std::cout << "Could not open file succesfuly for hexdump\n" << std::flush;
        }
    }
};

#endif