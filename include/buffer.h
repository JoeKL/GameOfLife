#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "game.h"

void alloc_buffer(char **buffer, COORD gridsize);
void dealloc_buffer(char **buffer);

void init_buffer(char *buffer, COORD gridsize, char symbolAlive, char symbolDead);
void update_buffer_at_coord(char *buffer, COORD gridsize, char symbolAlive, char symbolDead, int x_pos, int y_pos, int alive);

int calc_buffersize(COORD gridsize);
int calc_positionInBuffer( COORD gridsize, const int x_pos, const int y_pos);

void print_buffer(char *buffer);

#endif