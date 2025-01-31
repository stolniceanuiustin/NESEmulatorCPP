#ifndef SDL_HEADERGUARD
#define SDL_HEADERGUARD
#include "SDL.h"
#include "virtual_screen.h"
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
    STATE state;
    Screen &screen;

    SDL(Screen &screen) : screen(screen) {};
    void render_frame();
};
int init_sdl(SDL &sdl);
bool handle_input(SDL &sdl);
void destroy_sdl(SDL &sdl);
#endif