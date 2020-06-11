/**
 * @file buffer.c
 * @author Lothar Gomoluch, Oliver Röckener und Niko Tepe
 * @brief 
 * @date 05.06.2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "buffer.h"

/**
 * @brief
 * 
 * @param gridsize 
 * @return int 
 */
int calc_buffer_size(COORD gridsize){
    return sizeof(char) * gridsize.X * gridsize.Y * 2;
}

/**
 * @brief 
 * 
 * @param gridsize 
 * @param x_pos 
 * @param y_pos 
 * @return int 
 */
int calc_position_in_buffer(COORD gridsize, const int x_pos, const int y_pos){
    return  ( y_pos * gridsize.X + x_pos ) * 2;
}

/**
 * @brief 
 * 
 * @param buffer 
 * @param gridsize 
 */
void alloc_buffer(char **buffer, COORD gridsize){
    *buffer = (char*)malloc( calc_buffer_size(gridsize) );
}

/**
 * @brief 
 * 
 * @param buffer 
 */
void dealloc_buffer(char **buffer){
    free(buffer);
}

/**
 * @brief 
 * 
 * @param buffer 
 */
void print_buffer(char *buffer){
    set_cursor(0,0);
    printf("%s\n", buffer);
    set_cursor(0,0);
}

/**
 * @brief 
 * 
 * @param buffer 
 * @param gridsize 
 * @param symbolAlive 
 * @param symbolDead 
 */
void initialize_buffer(char *buffer, COORD gridsize, char symbolAlive, char symbolDead){

    int buffer_size = calc_buffer_size(gridsize);
    int written_chars = 0;

    for(int y = 0; y < gridsize.Y; y++){
        for(int x = 0; x < gridsize.X; x++){

            //Wenn am letzten Zeichen der Reihe,...
            if(x == gridsize.X - 1){
                //... dann speichere ohne Leerzeichen
               written_chars += snprintf(buffer + written_chars, buffer_size,"%c", symbolDead);
            } else {
                // sonst mit Leerzeichen
               written_chars += snprintf(buffer + written_chars, buffer_size,"%c ", symbolDead);
            }

        }
        // als letztes Zeichen jeder Reihe im Buffer wird ein Umbruch gesetzt
        written_chars += snprintf(buffer + written_chars, buffer_size,"\n");
    }

}

/**
 * @brief Sets the char in buffer at coord to symbolAlive
 * 
 * @param buffer 
 * @param gamesettings 
 * @param x_pos 
 * @param y_pos 
 */
void revive_buffer_at_coord(char *buffer, struct settings gamesettings, int x_pos, int y_pos){
    buffer[calc_position_in_buffer(gamesettings.gridsize, x_pos, y_pos)] = gamesettings.symbolAlive;
}

/**
 * @brief Sets the char in buffer at coord to symbolDead
 * 
 * @param buffer 
 * @param gamesettings 
 * @param x_pos 
 * @param y_pos 
 */
void kill_buffer_at_coord(char *buffer, struct settings gamesettings, int x_pos, int y_pos){
    buffer[calc_position_in_buffer(gamesettings.gridsize, x_pos, y_pos)] = gamesettings.symbolDead;
}

/**
 * @brief 
 * 
 * @param gamesettings 
 * @param aliveCells 
 * @param currentGeneration 
 */
void draw_hud(struct settings gamesettings, int aliveCells, int currentGeneration, int generationsToCalc){
    set_cursor(gamesettings.hud_currentGeneration_pos.X, gamesettings.hud_currentGeneration_pos.Y);
    printf("generation: %d of %d", currentGeneration, generationsToCalc);

    set_cursor(gamesettings.hud_aliveCells_pos.X, gamesettings.hud_aliveCells_pos.Y);
    printf("cells alive: %d of %d  ", aliveCells, gamesettings.gridsize.X*gamesettings.gridsize.Y);
    
    set_cursor(gamesettings.hud_gridSize_pos.X, gamesettings.hud_gridSize_pos.Y);
    printf("grid size: %dx%d", gamesettings.gridsize.X, gamesettings.gridsize.Y);

    set_cursor(gamesettings.hud_iterationsPerSecond_pos.X, gamesettings.hud_iterationsPerSecond_pos.Y);
    printf("iterationsPerSecond: %d", gamesettings.iterationsPerSecond);

    set_cursor(gamesettings.hud_shortcutInfo_pos.X, gamesettings.hud_shortcutInfo_pos.Y);
    printf("|\t'ESC' to end game\t|\t'SPACE' to pause game\t|\t'S' to save currrent state as preset\t|");
}