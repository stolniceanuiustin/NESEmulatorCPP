#ifndef SDL_HEADERGUARD
#define SDL_HEADERGUARD
#include "SDL.h"
#include "virtual_screen.h"
#include <SDL_ttf.h>
#include <../include/bus.h>
#include <string>
typedef enum
{
    QUIT,
    RUNNING,
    PAUSED
} STATE;
class SDL
{
public:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Renderer* debugRenderer;
    SDL_Window* debugWindow;
    BUS& bus;
    static STATE state;
    Screen &screen;
    bool tick;
    bool render_one_frame = false;
    SDL(Screen &screen, BUS& bus) : screen(screen), bus(bus) {};
    void render_frame();
};
int init_sdl(SDL &sdl);
bool handle_input(SDL &sdl);
void destroy_sdl(SDL &sdl);
void render_text(SDL& sdl, const std::string& text);
#endif