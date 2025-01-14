#ifndef EMULATOR_CONFIG
#define EMULATOR_CONFIG
#include <stdint.h>

typedef uint8_t byte;


#define ENTRY_POINT 0x8000
#define MAX_ROM_SIZE 0xFFFF + 1

class Config 
{
public:
    char* rom_name;
    int max_rom_size; //0x8000;
	uint16_t code_segment;
	Config(char* rom)
	{
		rom_name = rom;
		max_rom_size = MAX_ROM_SIZE;
	}
};

struct NESHeader
{
    char magic[4];
    byte prg_size; //program size in 16 kb units
    byte chr_size;
    byte flags6;
    byte flags7;
    byte unused[8];
};

#endif