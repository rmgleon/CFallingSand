// External
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "SDL.h"

//Internal
#include "structs.h"

//Definiions
#define FPS 60
#define FRAME_TARGET_TIME (1000/FPS)
#define RESOLUTION 600
#define WIDTH 50

typedef struct mouse{
    int x;
    int y;
} mouse;

// Global variables

global_state world;
mouse current_mouse;

int game_is_running = 0;
int direction=1;
int left_button_down;

float func_result;

bool sim[WIDTH][WIDTH];
bool aux_sim[WIDTH][WIDTH];

float grid_size = RESOLUTION/WIDTH;

// Frame Management

int last_frame_time = 0;
float delta_time = 0; //Difference between last frame and current frame

// Delta time is used to apply force and movement on a per frame basis.
// This is so that there is so that more powerful computers run
// the game just as fast as slower ones if they both reach the
// FPS limit

void setup(){
    for(int i =0; i<WIDTH; i++){
    for(int j =0 ; j<WIDTH; j++){
        sim[i][j]=false;
        aux_sim[i][j]=false;
    }
    }
}

/// @brief Handles user input
void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_is_running = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_is_running = 0;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_down = true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_down = false;
                }
                break;
        }
    }

    if (left_button_down) {          
        SDL_GetMouseState(&current_mouse.x, &current_mouse.y);
        
        // Grid indices
        int grid_x = current_mouse.x / grid_size;
        int grid_y = current_mouse.y / grid_size;
        
        if (grid_x >= 0 && grid_x < WIDTH && grid_y >= 0 && grid_y < WIDTH) {
            sim[grid_x][grid_y] = true;
        }
    }
}


/// @brief Initializes SDL, window and renderer
/// @return True if successful, otherwise False
int init_window(){
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        printf("SDL failed to initialize, %s", SDL_GetError());
    }

    world.window = SDL_CreateWindow(
        "Hi", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        RESOLUTION, RESOLUTION,
        SDL_WINDOW_SHOWN
    );
    if(!world.window){
        printf("Error: Window is null");
        return 0;
    }

    world.renderer = SDL_CreateRenderer(world.window, -1, 0);
    if(!world.renderer){
        printf("Error: Renderer is null");
        return 0;
    }

    return 1;
}

// Runs every FRAME_TARGET_TIME ms. Or 16.6 ms if FPS is 60
void update() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

    int current_time = SDL_GetTicks();
    delta_time = (current_time - last_frame_time) / 1000.0f; // delta time in seconds
    last_frame_time = current_time;

    // Sand Grid logic
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < WIDTH; j++){
            aux_sim[i][j] = sim[i][j];
        }
    }
    for(int i = 0; i < WIDTH; i++){
        
        for(int j = 0; j < WIDTH - 1; j++){
        if(i==0){
            if(sim[i][j] == true && sim[i][j+1] == false){
                aux_sim[i][j] = false;
                aux_sim[i][j+1] = true;
            }
            else if(sim[i][j] == true && sim[i+1][j+1] == false){
                aux_sim[i][j] = false;
                aux_sim[i+1][j+1] = true;
            }
        }
        else if(i>WIDTH-2){
            if(sim[i][j] == true && sim[i][j+1] == false){
                aux_sim[i][j] = false;
                aux_sim[i][j+1] = true;
            }
            else if(sim[i][j] == true && sim[i-1][j+1] == false){
                aux_sim[i][j] = false;
                aux_sim[i-1][j+1] = true;
            }
        }
        else{
            if(sim[i][j] == true && sim[i][j+1] == false){
                aux_sim[i][j] = false;
                aux_sim[i][j+1] = true;
            }
            else if(sim[i][j] == true && sim[i+1][j+1] == false){
                aux_sim[i][j] = false;
                aux_sim[i+1][j+1] = true;
            }
            else if(sim[i][j] == true && sim[i-1][j+1] == false){
                aux_sim[i][j] = false;
                aux_sim[i-1][j+1] = true;
            }

        }
            
        }
    }
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < WIDTH; j++){
            sim[i][j] = aux_sim[i][j];
        }
    }
}


void render(){
    // Background
    SDL_SetRenderDrawColor(world.renderer, 128, 55, 55, 255);
    SDL_RenderClear(world.renderer);

    // Render Grid
    SDL_SetRenderDrawColor(world.renderer, 255, 255, 255, 255);
    for(int i =0; i<WIDTH; i++){
    for(int j =0 ; j<WIDTH; j++){
        if(sim[i][j]==true){
            SDL_Rect dot_rect = {
                i * grid_size,
                j * grid_size,
                grid_size,
                grid_size
            };
            SDL_RenderFillRect(world.renderer, &dot_rect);
        }
    }
    }

    

    // Buffer swap
    SDL_RenderPresent(world.renderer);
}

int main(){
    game_is_running = init_window();
    setup();

    while(game_is_running){
        process_input();
        
        update();
        render();
    }
}