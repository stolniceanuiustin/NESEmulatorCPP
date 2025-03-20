#include "SDL.h"
#include "../include/SDL_backend.h"
#include "../include/emulator_config.h"
#include <iostream>

/*
TODO:
Store logs in a stack or queue type of shi. you know 

*/
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
        return 3;
    }

    sdl.debugWindow = SDL_CreateWindow("Debug Panel", 256, 50, 256*3, 50*3, 0);
    if(!sdl.debugWindow)
    {
        std::cout << "COULD NOT MAKE WINDOW";
        return 3;
    }
    sdl.debugRenderer = SDL_CreateRenderer(sdl.debugWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!sdl.debugRenderer)
    {
        std::cout << "COULD NOT MAKE RENDERER";
        return 3;
    }

    if(TTF_Init() == -1)
    {
        std::cout << "COULD NOT INITIALIZE TTF: " << TTF_GetError() << '\n';
        return 3;
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
    // SDL_SetRenderDrawColor(sdl.renderer, 0x00, 0x00, 0x00, 0x00);
    // SDL_RenderClear(sdl.renderer);
    // SDL_RenderPresent(sdl.renderer);
    return true;
}

void destroy_sdl(SDL& sdl)
{
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyRenderer(sdl.debugRenderer);
    SDL_DestroyWindow(sdl.window);
    SDL_DestroyWindow(sdl.debugWindow);
    TTF_Quit();
    SDL_Quit();
}

void SDL::render_frame()
{
    SDL_Texture* frame = SDL_CreateTexture(renderer, 
                                        SDL_PIXELFORMAT_RGBA8888, 
                                        SDL_TEXTUREACCESS_STREAMING, 
                                        256, 240);  
    SDL_UpdateTexture(frame, nullptr, screen.pixels, 256*sizeof(uint32_t));
    SDL_Rect destRect = {0, 0, 256*3, 256*3};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, frame, nullptr, &destRect);
    SDL_RenderPresent(renderer);
    // for(int i=0; i<256*240; i++)
    // {
    //     std::cout << screen.pixels[i];
    // }
    SDL_DestroyTexture(frame);
} 

void render_text(SDL& sdl, const std::string& text)
{
    static TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/Arial.ttf", 24);
    if(!font)
    {
        std::cout <<"CANT LOAD FONT" << TTF_GetError() << '\n';
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(sdl.debugRenderer, textSurface);

    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_SetRenderDrawColor(sdl.debugRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl.debugRenderer);
    SDL_RenderCopy(sdl.debugRenderer, textTexture, nullptr, &textRect);
    SDL_RenderPresent(sdl.debugRenderer);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}