#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define X_Size 117
#define Y_Size 57

struct cell{
    int alive;
    struct cell *neighborCell[8];
};

struct settings{

    char symbolAlive;
    char symbolDead;

    int periodInSeconds;
    int iterationsPerSecond;

    int generation_pos_x;
    int generation_pos_y;
    
    int gridSize_pos_x;
    int gridSize_pos_y;

    int periodInSeconds_pos_x;
    int periodInSeconds_pos_y;

    int iterationsPerSecond_pos_x;
    int iterationsPerSecond_pos_y;

    int aliveCells_pos_x;
    int aliveCells_pos_y;

};

unsigned int generate_random_int_msws();
void sleep_ms(int milliseconds);

void load_preset(struct cell grid_ptr[X_Size][Y_Size]);
void save_preset(struct cell grid_ptr[X_Size][Y_Size]);

int count_living_neighbors(struct cell grid_ptr[X_Size][Y_Size], int x, int y);
void define_neighborhood(struct cell grid_ptr[X_Size][Y_Size]);

void initialize_grid(struct cell grid_ptr[X_Size][Y_Size]);
void print_gamestate(struct cell grid_ptr[X_Size][Y_Size], struct settings gamesettings);


#endif