/**
 * @file buffer.h
 * @author  Lothar Gomoluch, Oliver Röckener und Niko Tepe ()
 * @brief 
 * @date 05.06.2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "game.h"

void alloc_buffer(char **buffer, COORD gridsize);
void dealloc_buffer(char **buffer);

void initialize_buffer(char *buffer, COORD gridsize, char symbolAlive, char symbolDead);

void revive_buffer_at_coord(char *buffer, struct settings gamesettings, int x_pos, int y_pos);
void kill_buffer_at_coord(char *buffer, struct settings gamesettings, int x_pos, int y_pos);

int calc_buffer_size(COORD gridsize);
int calc_position_in_buffer( COORD gridsize, const int x_pos, const int y_pos);

void draw_hud(struct settings gamesettings, int aliveCells, int currentGeneration, int generationsToCalc);

void print_buffer(char *buffer);

#endif