// External
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "SDL.h"


//Internal
#include "structs_and_definitions.h"
#include "sand_logic.h"


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
char current_particle=1;

float func_result;

// 0 is air, 1 is regular sand. The rest are special behaviours
// Fuck it, we ball.
char sim[WIDTH][WIDTH];
char aux_sim[WIDTH][WIDTH];

//colors
char colors[256][3] = {
    {0, 0, 0},
    {128,128,128}, //Basic Sand
    {10,128,40}, // Inert gas
    {0, 0, 128} // Water
};

float grid_size = RESOLUTION/WIDTH;

// Frame Management

int last_frame_time = 0;
float delta_time = 0; //Difference between last frame and current frame

// Delta time is used to apply force and movement on a per frame basis.
// This is so that there is so that more powerful computers run
// the game just as fast as slower ones if they both reach the
// FPS limit

void setup(){
    srand(time(NULL));
    for(int i =0; i<WIDTH; i++){
    for(int j =0 ; j<=WIDTH; j++){
        sim[i][j]=0;
        aux_sim[i][j]=0;
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
                if (event.key.keysym.sym == SDLK_0) {
                    current_particle = 0;
                }
                if (event.key.keysym.sym == SDLK_1) {
                    current_particle = 1;
                }
                if (event.key.keysym.sym == SDLK_2) {
                    current_particle = 2;
                }
                if (event.key.keysym.sym == SDLK_3) {
                    current_particle = 3;
                }
                if (event.key.keysym.sym == SDLK_r) {
                    for(int i=0; i<WIDTH - 1; i++){
                    for(int j=0; j<WIDTH - 1; j++){
                        sim[i][j]=0;
                    }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_down = 1;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_down = 0;
                }
                break;
        }
    }

    if (left_button_down) {          
        SDL_GetMouseState(&current_mouse.x, &current_mouse.y);
    
        
        // Mouse Grid indices
        int grid_x = current_mouse.x / grid_size;
        int grid_y = current_mouse.y / grid_size;
        
        
        
        if (grid_x >= 0 && grid_x <= WIDTH && grid_y >= 0 && grid_y <= WIDTH) {
            for(int i=0;i<30;i++){
                sim[grid_x][grid_y+i] = current_particle;
                sim[grid_x+1][grid_y+i] = current_particle;
            }
            
        }

        
    }
}


/// @brief Initializes SDL, window and renderer
/// @return 1 if successful, otherwise 0
int init_window(){
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        printf("SDL failed to initialize, %s", SDL_GetError());
    }

    world.window = SDL_CreateWindow(
        "C Falling Sand", 
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

    // Auxiliary matrix, to update the sim frame by frame instead of everything at once.
    // memory expensive but who cares
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < WIDTH; j++){
            aux_sim[i][j] = sim[i][j];
        }
    }

    // Main logic
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < WIDTH; j++){

        // Type of sand checker
        if(sim[i][j]==1){
            regular_sand(sim, aux_sim, i, j);
        }

        else if(sim[i][j]==2){
            inert_gas(sim, aux_sim, i, j);
        }

        else if(sim[i][j]==3){
            basic_water(sim, aux_sim, i, j);
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
    SDL_SetRenderDrawColor(world.renderer, 10, 10, 15, 255);
    SDL_RenderClear(world.renderer);

    // Render Gridx
    for(int i =0; i<WIDTH; i++){
    for(int j =0 ; j<WIDTH; j++){
        
        if(sim[i][j]>=1){

        Uint8 r = colors[(int)sim[i][j]][0];
        Uint8 g = colors[(int)sim[i][j]][1];
        Uint8 b = colors[(int)sim[i][j]][2];
        SDL_SetRenderDrawColor(world.renderer, r, g, b, 255);
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