#include <iostream>
#include "emulator_config.h"
//#include "SDL.h"
#include <unistd.h>
#include "cpu_header.h"
#include "unittest.h"
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

    

    

    CPU cpu;
    cpu.reset();
    // if(argc < 2)
    // {
    //     std::cerr << "Usage: " << argv[0] << "rom_name\n";
    //     return -1;
    // }

    // Config config = Config(argv[1]);
    Config config = Config("../roms/nestest.nes");
    config.code_segment = 0xC000;
    if(!cpu.init(config))
    {
        return -1;
    }
    int t = 0;

    while(true)
    {
        cpu.execute();
        usleep(0.558659218);
    }

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
    // Assert assert;
    // assert.unit_test();
}