#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "menu.h"

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

int get_time_since_start_value(int start_value);

void alloc_grid(struct cell *** grid_ptr, COORD gridsize);
void dealloc_grid(struct cell *** grid_ptr, const int x_size);
void copy_grid(struct cell ** grid_ptr_dest, struct cell ** grid_ptr_src, COORD gridsize);

void load_preset_to_grid(struct cell ** grid_ptr, COORD gridsize);
void save_preset_from_grid(struct cell ** grid_ptr, COORD gridsize);

int count_living_neighbors(struct cell grid);
void define_neighborhood(struct cell ** grid_ptr, COORD gridsize);

void add_neighborhood(struct cell grid);
void sub_neighborhood(struct cell grid);

void initialize_empty_grid(struct cell ** grid_ptr, COORD gridsize);
void calc_all_neighbors(struct cell ** grid_ptr, COORD gridsize);

void generate_random_grid(struct cell **grid_ptr, COORD gridsize);

#endif