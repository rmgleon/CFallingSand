#ifndef LOCAL_SDL_STRUCTS_H
#define LOCAL_SDL_STRUCTS_H
#include "SDL.h"

//Definiions
#define WIDTH 250
#define FPS 60
#define FRAME_TARGET_TIME (500/FPS)
#define RESOLUTION 1000


/// @brief Holds the global state of the app
typedef struct global_state{
    SDL_Window* window;
    SDL_Renderer* renderer;
} global_state;



#endif // LOCAL_SDL_STRUCTS_H