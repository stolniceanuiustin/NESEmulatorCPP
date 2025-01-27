#include <iostream>
#include <unistd.h>
#include "../include/emulator_config.h"
#include "SDL.h"
#include "../include/SDL_backend.h"
#include "../include/cpu_header.h"
#include "../include/unittest.h"
#include "../include/ppu.h"
#include "../include/mapper.h"

using std::cout;

int main(int argc, char *argv[])
{
    // if(argc < 2)
    // {
    //     std::cerr << "Usage: " << argv[0] << "rom_name\n";
    //     return -1;
    // }
    SDL sdl;
    init_sdl(sdl);
    sdl.state = PAUSED;

    //TODO: COMMON BUS, maybe move the registers from the ppu to the common bus, idk if its wortH?
    Memory ram(0xFFFF);
    Memory ppu_ram(0x3FFF);
    Config config = Config("../roms/donkeykong.nes");
    mapper(config, ram, ppu_ram);
    ram[0] = 0xFF;
    CPU cpu = CPU(ram);
    cpu.reset();
    cpu.init();
    PPU ppu(ram, ppu_ram, cpu);
    config.code_segment = cpu.read_abs_address(0xFFFC);

    std::cout << "RAM ADDRESS IN main:" << &ram << '\n';
    cpu.ram.hexdump("CPU_ram", 0xFFFF);
    ram.hexdump("PPU_RAM", 0x3FFF);

    while (1)
    {
        handle_input(sdl);

        if (sdl.state == RUNNING)
        {
            for (int i = 0; i <= 100000; i++)
            {
                cpu.execute();
                //CHECK HOW MANY CYCLES THEN CATCH UP THE PPU!
                ppu.execute();
                ppu.execute();
                ppu.execute();
                usleep(0.558659218);
            }
            goto ENDLOOP;
        }
        else if (sdl.state == PAUSED)
        {
            continue;
        }
        else
            goto ENDLOOP;
    }
ENDLOOP:
    destroy_sdl(sdl);
    // ram.hexdump();

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    // Assert assert;
    // assert.unit_test();
}