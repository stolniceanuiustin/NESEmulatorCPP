#ifndef SDL_HEADERGUARD
#define SDL_HEADERGUARD
#include "SDL.h"

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
};
int init_sdl(SDL& sdl);
bool handle_input(SDL& sdl);
void destroy_sdl(SDL& sdl);
#endif