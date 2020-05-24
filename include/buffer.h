#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "game.h"

void alloc_buffer(char **buffer, const int x_size, const int y_size);
void dealloc_buffer(char **buffer, const int x_size, const int y_size);

void init_buffer(char *buffer, const int x_size, const int y_size, char symbolAlive, char symbolDead);
// void update_buffer(char *buffer, struct cell **grid_ptr, struct cell **gridcopy_ptr, const int x_size, const int y_size, char symbolAlive, char symbolDead);
void update_buffer_at_coord(char *buffer, const int x_size, const int y_size, char symbolAlive, char symbolDead, int x_pos, int y_pos, int alive);

int calc_buffersize(const int x_size, const int y_size);
int calc_positionInBuffer( const int x_size, const int y_size, const int x_pos, const int y_pos);

void print_buffer(char *buffer);

#endif