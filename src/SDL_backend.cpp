#include "SDL.h"
#include "SDL_backend.h"
#include "emulator_config.h"
#include <iostream>
int init_sdl(SDL& sdl)
{
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    sdl.window = SDL_CreateWindow("NESEMU", 256, 240, 256*3, 240*3, 0);
    if(!sdl.window)
    {
        std::cout << "COULD NOT MAKE WINDOW";
        return 3;
    }
    sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);
    if(!sdl.renderer)
    {
        std::cout << "COULD NOT MAKE RENDERER";
    }
    return 0;
}

bool handle_input(SDL& sdl)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                sdl.state = QUIT;        
                return true;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    if(sdl.state == RUNNING)
                    {
                        sdl.state = PAUSED;
                        std::cout << "==========PAUSED==========\n";
                    }
                    else sdl.state = RUNNING;
                    return true;
                }
            case SDL_KEYUP:
                return false;
            default: 
                return false;
        }
    }
    SDL_SetRenderDrawColor(sdl.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(sdl.renderer);
    SDL_RenderPresent(sdl.renderer);
    return true;
}

void destroy_sdl(SDL& sdl)
{
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}