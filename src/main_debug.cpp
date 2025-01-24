#include <iostream>
#include "emulator_config.h"
// #include "SDL.h"
#include <unistd.h>
#include "CPU/cpu_header.h"
#include "CPU/unittest.h"
#include "PPU/ppu.h"
#include "Mappers/mapper.h"
using std::cout;

int main(int argc, char *argv[])
{
    // SDL_Window *window;
    // SDL_Renderer *renderer;
    // SDL_Surface *surface;
    // SDL_Event event;

    // if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    //     return 3;
    // }

    // if (SDL_CreateWindowAndRenderer(320, 240, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
    //     return 3;
    // }

    // while (1) {
    //     SDL_PollEvent(&event);
    //     if (event.type == SDL_QUIT) {
    //         break;
    //     }
    //     SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    //     SDL_RenderClear(renderer);
    //     SDL_RenderPresent(renderer);
    // }
    // if(argc < 2)
    // {
    //     std::cerr << "Usage: " << argv[0] << "rom_name\n";
    //     return -1;
    // }

    Memory ram;
    Config config = Config("../roms/donkeykong.nes");
    mapper(config, ram);
    ram[0] = 0xFF;
    CPU cpu = CPU(ram);
    cpu.reset();
    cpu.init();
    PPU ppu(ram);
    config.code_segment = cpu.read_abs_address(0xFFFC);

    std::cout << "RAM ADDRESS IN main:" << &ram << '\n';
    cpu.ram.hexdump("CPU_ram");
    ram.hexdump("RAM");
    int t = 0;
    //cpu.cycles += 7;
    bool run = true;
    while (run)
    {
        for (int i = 0; i < 35000; i++)
        {
            cpu.execute();
            usleep(0.558659218);
        }
        run = false;
    }
    // ram.hexdump();

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    // Assert assert;
    // assert.unit_test();
}