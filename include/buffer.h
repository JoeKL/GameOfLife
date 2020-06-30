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

/**
 * @brief aloziiert den Buffer
 * 
 * @param buffer Verweis auf den Buffer
 * @param gridsize Größe des Spielfeldes
 */
void alloc_buffer(char **buffer, COORD gridsize);

/**
 * @brief dealoziiert den Buffer
 * 
 * @param buffer Verweis auf den Buffer
 */
void dealloc_buffer(char *buffer);

/**
 * @brief initialisiert den Buffer
 * 
 * @param buffer Verweis auf den Buffer
 * @param gridsize Größe des Spielfeldes
 * @param symbolAlive Symbol für lebende Zellen
 * @param symbolDead Symbol für tote Zellen
 */
void initialize_buffer(
                char *buffer, 
                COORD gridsize, 
                char symbolAlive, 
                char symbolDead
);

/**
 * @brief Diese Funktion belebt eine Zelle an bestimmter Stelle im Buffer wieder
 * 
 * @param buffer Verweis auf den Buffer
 * @param gamesettings Gamesettings Struct
 * @param x_pos Position X auf dem Spielfeld
 * @param y_pos Position Y auf dem Spielfeld
 */
void revive_buffer_at_coord(
                    char *buffer, 
                    struct settings gamesettings, 
                    int x_pos, 
                    int y_pos
);

/**
 * @brief Diese Funktion tötet eine Zelle an bestimmter Stelle im Buffer
 * 
 * @param buffer Verweis auf den Buffer
 * @param gamesettings Gamesettings Struct
 * @param x_pos Position X auf dem Spielfeld
 * @param y_pos Position Y auf dem Spielfeld
 */
void kill_buffer_at_coord(
                    char *buffer, 
                    struct settings gamesettings, 
                    int x_pos, 
                    int y_pos
);

/**
 * @brief Berechnet die Speichergröße des Buffers
 * 
 * @param gridsize Größe des Spielfeldes
 * @return int Gibt die Speichergröße zurück
 */
int calc_buffer_size(COORD gridsize);

/**
 * @brief Berechnet die Position einer Zelle im Buffer anhand der Position auf dem Spielfeld
 * 
 * @param gridsize Größe des Spielfeldes
 * @param x_pos Position X auf dem Spielfeld
 * @param y_pos Position Y auf dem Spielfeld
 * @return int Gibt die Position im Buffer zurück
 */
int calc_position_in_buffer( 
                        COORD gridsize, 
                        const int x_pos, 
                        const int y_pos
);

/**
 * @brief Zeichnet das Head-Up-Display im Spiel
 * 
 * @param gamesettings  Größe des Spielfeldes
 * @param aliveCells Anzahl lebender Zellen
 * @param currentGeneration Nummer der aktuellen Generation
 * @param generationsToCalc Noch zu berechnende Generationen 
 */
void draw_hud(
        struct settings gamesettings, 
        int aliveCells, 
        int currentGeneration, 
        int generationsToCalc
);

/**
 * @brief Gibt den Buffer aus
 * 
 * @param buffer Verweis auf den Buffer
 */
void print_buffer(char *buffer);

#endif