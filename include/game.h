#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//max = 117x57
#define X_Size 117
#define Y_Size 57

struct cell{
    int alive;
    struct cell *neighborCell[8];
};

struct settings{
    
    COORD gridsize;

    char symbolAlive;
    char symbolDead;

    int periodInSeconds;
    int iterationsPerSecond;

    COORD hud_currentGeneration_pos;  
    COORD hud_gridSize_pos;
    COORD hud_periodInSeconds_pos;
    COORD hud_iterationsPerSecond_pos;
    COORD hud_aliveCells_pos;
    
};

uint32_t generate_random_int_msws();

void alloc_grid(struct cell *** grid_ptr, const int x_size, const int y_size);
void dealloc_grid(struct cell *** grid_ptr, const int x_size);
void copy_grid(struct cell ** grid_ptr_dest, struct cell ** grid_ptr_src, const int x_size, const int y_size);

void load_preset(struct cell ** grid_ptr, const int x_size, const int y_size);
void save_preset(struct cell ** grid_ptr, const int x_size, const int y_size);

int count_living_neighbors(struct cell ** grid_ptr, int x, int y);
void define_neighborhood(struct cell ** grid_ptr, const int x_size, const int y_size);

void initialize_grid(struct cell ** grid_ptr, const int x_size, const int y_size);
void print_grid(struct cell ** grid_ptr, const int x_size, const int y_size, struct settings gamesettings);

void generate_random_grid(struct cell **grid_ptr, const int x_size, const int y_size);

#endif