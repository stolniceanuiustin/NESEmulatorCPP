#ifndef SDL_HEADERGUARD
#define SDL_HEADERGUARD
#include "SDL.h"
#include "virtual_screen.h"
#include <SDL_ttf.h>
#include <../include/bus.h>
#include <string>
#include <thread>
#include <atomic>
typedef enum
{
    QUIT,
    RUNNING,
    PAUSED
} STATE;
class SDL
{
public:
    std::atomic<STATE> atomic_state;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Renderer* debugRenderer;
    SDL_Window* debugWindow;
    Screen &screen;
    BUS& bus;
    static STATE state;
    bool tick;
    bool render_one_frame = false;
    SDL(Screen &screen, BUS& bus) : screen(screen), bus(bus) {};
    void render_frame();
    std::atomic<byte> controller_input_buffer{0};
};
int init_sdl(SDL &sdl);
bool handle_input(SDL &sdl);
void destroy_sdl(SDL &sdl);
void render_text(SDL& sdl, const std::string& text);
#endif