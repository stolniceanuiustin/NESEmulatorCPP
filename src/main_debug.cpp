#include <iostream>
#include <unistd.h>
#include "../include/emulator_config.h"
#include "SDL.h"
#include "../include/SDL_backend.h"
#include "../include/cpu_header.h"
#include "../include/unittest.h"
#include "../include/ppu.h"
#include "../include/mapper.h"
#include "../include/bus.h"
#include "../include/cartridge.h"

using std::cout;

int main(int argc, char *argv[])
{
    // if(argc < 2)
    // {
    //     std::cerr << "Usage: " << argv[0] << "rom_name\n";
    //     return -1;
    // }
    Screen screen;
    SDL sdl(screen);
    init_sdl(sdl);
    sdl.state = PAUSED;

    //Config config = Config("../roms/donkeykong.nes");
    Config config = Config("../roms/nestest.nes");
    CARTRIDGE cartridge(config);
    cartridge.read_file();

    std::cout << "READ FILE SUCCESFULLY\n";
    CPU cpu;
    PPU ppu(cpu, screen);
    BUS bus(cpu, ppu, cartridge);
    cpu.connect_bus(&bus);
    ppu.connect_bus(&bus);
    
    cpu.reset();
    cpu.init();
    bus.hexdump();
    

    config.code_segment = cpu.read_abs_address(0xFFFC);
    cpu.PC = 0xC000;
    while (1)
    {
        handle_input(sdl);
        if (sdl.state == RUNNING)
        {
            for (int i = 0; i <= 10000; i++)
            {
                int old_cycles = cpu.get_cycles();
                cpu.execute();
                int new_cycles = cpu.get_cycles();
                int cycles_elapsed = new_cycles - old_cycles;
                // CHECK HOW MANY CYCLES THEN CATCH UP THE PPU
                for (int i = 0; i < cycles_elapsed; i++)
                {
                    ppu.execute();
                    ppu.execute();
                    ppu.execute();
                }

                //render_frmae
                if(screen.RENDER_ENABLED == true)
                {   sdl.render_frame();
                    screen.RENDER_ENABLED = false;
                }
                usleep(0.558659218 /*-time(cpu) - 3*time(ppu)*/);
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