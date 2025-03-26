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

const double CLK_TIME = 1.0/1789773.0;

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

    Config config = Config("../roms/donkeykong.nes");
    //Config config = Config("../roms/nestest.nes");
    CARTRIDGE cartridge(config);
    cartridge.read_file();

    std::cout << "READ FILE SUCCESFULLY\n";
    CPU cpu;
    PPU ppu(cpu, screen);
    BUS bus(cpu, ppu, cartridge);
    cpu.connect_bus(&bus);
    ppu.connect_bus(&bus);
    ppu.connect_cartridge(&cartridge);
    cpu.reset();
    cpu.init();
    

    config.code_segment = cpu.read_abs_address(0xFFFC);
    //cpu.PC = 0xC000;
    //TODO run for one frmae then sleep
    int frames = 0;
    while (1)
    {
        handle_input(sdl);
        if (sdl.state == RUNNING)
        {
            uint32_t start_time = SDL_GetTicks();
            uint32_t end_time = 0;
            int old_cycles = cpu.get_cycles();
            cpu.execute();
            int new_cycles = cpu.get_cycles();
            int cycles_elapsed = new_cycles - old_cycles;
            //Prevent cycle overflow
            if(new_cycles >= 100000)
                cpu.set_cycles(0);
            
            for (int i = 0; i < cycles_elapsed; i++)
            {
                ppu.execute();
                ppu.execute();
                ppu.execute();
            }

            if (screen.RENDER_ENABLED == true)
            {
                //cout << "=======RENDERING FRAME=======" << frames << '\n';
                sdl.render_frame();
                frames++;
                uint32_t end_time = SDL_GetTicks();
                screen.RENDER_ENABLED = false;
                double time_elapsed = (end_time - start_time)*1000;
                if(time_elapsed < 33333)
                {
                    timespec requested_time;
                    timespec remaining_time;
                    requested_time.tv_nsec = 33333 - time_elapsed;
                    nanosleep(&requested_time, &remaining_time);
                    //cout << end_time << " " << start_time << std::endl;
                }
            }
            if(frames == 30)
            {
                std::cout <<"ONE SECOND PASSED" << std::endl;
                frames = 0;
            }
            
        }
        else if (sdl.state == PAUSED)
        {
            if(sdl.tick == true)
            {
                int old_cycles = cpu.get_cycles();
                std::string debug_output_log = cpu.execute_debug();
                int new_cycles = cpu.get_cycles();
                int cycles_elapsed = new_cycles - old_cycles;
                // CHECK HOW MANY CYCLES THEN CATCH UP THE PPU
                for (int i = 0; i < cycles_elapsed; i++)
                {
                    ppu.execute();
                    ppu.execute();
                    ppu.execute();
                }
    
                // render_frmae
                if (screen.RENDER_ENABLED == true)
                {
                    sdl.render_frame();
                    screen.RENDER_ENABLED = false;
                }
                render_text(sdl, debug_output_log);
                sdl.tick = false;
            }
            continue;
        }
        else if(sdl.state == QUIT)
        {
            bus.hexdump();
            ppu.hexdump();
            goto ENDLOOP;
        }
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