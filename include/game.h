#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "menu.h"

//max = 117x57
#define X_Size 117
#define Y_Size 57

struct cell{
    int alive;
    int livingNeighbors;
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

void alloc_grid(struct cell *** grid_ptr, COORD gridsize);
void dealloc_grid(struct cell *** grid_ptr, const int x_size);
void copy_grid(struct cell ** grid_ptr_dest, struct cell ** grid_ptr_src, COORD gridsize);

void load_preset(struct cell ** grid_ptr, COORD gridsize);
void save_preset(struct cell ** grid_ptr, COORD gridsize);

int count_living_neighbors(struct cell grid);
void define_neighborhood(struct cell ** grid_ptr, COORD gridsize);
void refresh_neighborhood(struct cell grid, int value);

void initialize_grid(struct cell ** grid_ptr, COORD gridsize);
void initialize_neighbors(struct cell ** grid_ptr, COORD gridsize);
void print_grid(struct cell ** grid_ptr, COORD gridsize, struct settings gamesettings);
void print_neighbors(struct cell ** grid_ptr, COORD gridsize, struct settings gamesettings);

void generate_random_grid(struct cell **grid_ptr, COORD gridsize);

#endif