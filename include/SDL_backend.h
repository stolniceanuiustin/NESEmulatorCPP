#ifndef SDL_HEADERGUARD
#define SDL_HEADERGUARD
#include "SDL.h"
#include "virtual_screen.h"
#include <SDL_ttf.h>
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
    static STATE state;
    Screen &screen;
    bool tick;
    SDL(Screen &screen) : screen(screen) {};
    void render_frame();
};
int init_sdl(SDL &sdl);
bool handle_input(SDL &sdl);
void destroy_sdl(SDL &sdl);
void render_text(SDL& sdl, const std::string& text);
#endif