/**
 * @file game.h
 * @author Lothar Gomoluch, Oliver Röckener und Niko Tepe
 * @brief 
 * @date 05.06.2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */

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

    int generationsToCalc;
    int iterationsPerSecond;

    COORD hud_currentGeneration_pos;  
    COORD hud_gridSize_pos;
    COORD hud_generationsToCalc_pos;
    COORD hud_iterationsPerSecond_pos;
    COORD hud_aliveCells_pos;
    COORD hud_shortcutInfo_pos;
};


struct rule{
    int rebornRule;
    int lonelinessRule;
    int overpopulationRule;
};

/**
 * @brief Generiert eine Middle Square Weyl Sequence um daraus eine Randomzahl zu generieren.
 * https://pthree.org/2018/07/30/middle-square-weyl-sequence-prng/
 * Diese Funktion wird verwendet, da so eine bessere Zufallszahlgenerierung gewährleistet wird.
 * 
 * @return uint32_t gibt einen unsigned 32bit Integer zurück
 */
uint32_t generate_random_int_msws();

/**
 * @brief alloc_grid reserviert den Speicher des Feldes
 * struct cell ***grid_ptr -> deklariere "grid" als pointer to pointer to pointer to struct cell 
 * 
 * @param grid_ptr Verweis auf das Grid
 * @param gridsize Größe des Spielfeldes
 */
void alloc_grid(struct cell *** grid_ptr, COORD gridsize);

/**
 * @brief dealloc_grid gibt den Speicher des Feldes wieder frei
 * struct cell ***grid_ptr -> deklariere "grid" als pointer to pointer to pointer to struct cell 
 * 
 * @param grid_ptr Verweis auf das Grid
 * @param x_size Größe der X-Achse des Spielfeldes
 */
void dealloc_grid(struct cell *** grid_ptr, const int x_size);

/**
 * @brief copy_grid kopiert alle Werte eines Feldes in ein neues Feld
 * 
 * @param grid_ptr_dest Verweis auf das Ziel Grid
 * @param grid_ptr_src  Verweis auf das Grid von dem kopiert werden soll
 * @param gridsize Größe des Spielfeldes
 */
void copy_grid(
            struct cell ** grid_ptr_dest, 
            struct cell ** grid_ptr_src, 
            COORD gridsize
);

/**
 * @brief load_preset_to_grid laed ein Preset aus einer Textdatei.
 * 
 * @param grid_ptr Verweis auf das Grid
 * @param gridsize Größe des Spielfeldes
 */
void load_preset_to_grid(struct cell ** grid_ptr, COORD gridsize);

/**
 * @brief save_preset_to_grid speichert den Stand des Feldes als Preset in eine Textdatei.
 * 
 * @param grid_ptr Verweis auf das Grid
 * @param gridsize Größe des Spielfeldes
 */
void save_preset_from_grid(struct cell ** grid_ptr, COORD gridsize);


/**
 * @brief count_living_neighbors zählt die lebenden umliegenden Nachbarn.
 * diese Funktion ist möglichst effizient geschrieben, da sie beim initialisieren eines 1000x1000 Feldes bis zu 1.000.000x aufgerufen wird.
 * 
 * @param cell Verweis auf eine Zelle im Spielfeld
 * @return int Anzahl der lebenden Nachbarn
 */
int count_living_neighbors(struct cell grid);

/**
 * @brief define_neighborhood definiert alle anliegenden Nachbarzellen für jede Zelle eines Feldes
 * 
 * @param grid_ptr Verweis auf das Grid
 * @param gridsize Größe des Spielfeldes
 */
void define_neighborhood(struct cell ** grid_ptr, COORD gridsize);

/**
 * @brief add_neighborhood informiert jede anliegende Nachbarzelle über den aktuellen Stand der Zelle
 * das passiert durch das Hochzählen von livingNeighbors in jeder anliegenden Zelle
 * diese Funktion ist möglichst effizient geschrieben,
 * da sie bei einem 1000x1000 Feld bis zu 1.000.000x pro Tick aufgrufen werden kann.
 * 
 * @param cell Verweis auf eine Zelle im Spielfeld
 */
void add_neighborhood(struct cell grid);

/**
 * @brief sub_neighborhood informiert jede anliegende Nachbarzelle über den aktuellen Stand der Zelle
 * das passiert durch das Runterzählen von livingNeighbors in jeder anliegenden Zelle
 * diese Funktion ist möglichst effizient geschrieben, 
 * da sie bei einem 1000x1000 Feld bis zu 1.000.000x pro Tick aufgrufen werden kann.
 * 
 * @param cell Verweis auf eine Zelle im Spielfeld 
 */
void sub_neighborhood(struct cell grid);

/**
 * @brief initialize_empty_grid initialisiert ein leeres grid
 * 
 * @param grid_ptr Verweis auf das Grid
 * @param gridsize Größe des Spielfeldes
 */
void initialize_empty_grid(struct cell ** grid_ptr, COORD gridsize);

/**
 * @brief calc_all_neighbors initialisiert die livingNeighbors für jede Zelle.
 * 
 * @param grid_ptr Verweis auf das Grid
 * @param gridsize Größe des Spielfeldes
 */
void calc_all_neighbors(struct cell ** grid_ptr, COORD gridsize);

/**
 * @brief 
 * generate_random_grid initialisiert das übergebene Feld mit dem Modulo 2 (%2)
 * aus einer zufälligen Zahl für jede Zelle eines Feldes
 * 
 * @param grid_ptr Verweis auf das Grid
 * @param gridsize Größe des Spielfeldes
 */
void generate_random_grid(struct cell **grid_ptr, COORD gridsize);

#endif