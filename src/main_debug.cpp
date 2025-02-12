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
    

    // TODO: COMMON BUS, maybe move the registers from the ppu to the common bus, idk if its wortH?
    Memory ram(0xFFFF);
    Memory ppu_ram(0x3FFF);
    
    Screen screen;

    SDL sdl(screen);
    init_sdl(sdl);
    sdl.state = PAUSED;

    //Config config = Config("../roms/donkeykong.nes");
    Config config = Config("../roms/nestest.nes");
    CARTRIDGE cartridge(config);
    cartridge.read_file();

    CPU cpu = CPU(ram);
    cpu.reset();
    cpu.init();

    PPU ppu(ram, ppu_ram, cpu, screen);
    BUS bus(cpu, ppu, cartridge);
    cpu.connect_bus(&bus);
    ppu.connect_bus(&bus);

    config.code_segment = cpu.read_abs_address(0xFFFC);
    // cpu.ram.hexdump("CPU_ram", 0xFFFF);
    // ram.hexdump("PPU_RAM", 0x3FFF);
    cpu.write(0x8001, 0xFF);
    cpu.write(0x800, 0xFF);
    cpu.write(0x2000, 0xFF);

    cout << (int)(*(cartridge.get_PRGrom() + 1)) << '\n';
    cout << (int)bus.cpu_ram[0x0000] << '\n';
    cout << (int)ppu.get_control() << '\n';
    cout << "===================\n" << std::endl;
    while (1)
    {
        handle_input(sdl);

        if (sdl.state == RUNNING)
        {
            // for (int i = 0; i <= 100000; i++)
            // {
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
            // }
            //goto ENDLOOP;
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