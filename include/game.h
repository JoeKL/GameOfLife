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

    char symbolAlive;
    char symbolDead;

    int periodInSeconds;
    int iterationsPerSecond;

    COORD hud_currentGeneration_pos;  
    COORD hud_gridSize_pos;
    COORD hud_periodInSeconds_pos;
    COORD hud_iterationsPerSecond_pos;
    COORD hud_aliveCells_pos;

    COORD main_menu_start_pos;
    COORD main_menu_settings_pos;
    COORD main_menu_exit_pos;

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