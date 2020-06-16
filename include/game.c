/**
 * @file game.c
 * @author Lothar Gomoluch, Oliver Röckener und Niko Tepe
 * @brief 
 * @date 05.06.2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "game.h"

uint64_t x_msws = 0, w_msws = 0;
// Must be odd (least significant bit is "1"), and upper 64-bits non-zero
uint64_t seed_msws = 0xb5ad4eceda1ce2a9; // qualifying seed

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t generate_random_int_msws() {
    // https://pthree.org/2018/07/30/middle-square-weyl-sequence-prng/
    x_msws *= x_msws; // square the number
    w_msws += seed_msws; // the weyl sequence
    x_msws += w_msws; // apply to x
    return x_msws = (x_msws>>32) | (x_msws<<32); // return the middle 32-bits
}

/**
 * @brief alloc_grid reserviert den Speicher des Feldes
 * struct cell ***grid_ptr -> deklariere "grid" als pointer to pointer to pointer to struct cell 
 * 
 * @param grid_ptr 
 * @param gridsize 
 */
void alloc_grid(struct cell ***grid_ptr, COORD gridsize){

    *grid_ptr = (struct cell **)malloc(gridsize.X * sizeof(struct cell *));
    for (int i = 0; i < gridsize.X; i++)
    {   
        (*grid_ptr)[i] = (struct cell *)malloc(gridsize.Y * sizeof(struct cell));
    }
}

/**
 * @brief dealloc_grid gibt den Speicher des Feldes wieder frei
 * struct cell ***grid_ptr -> deklariere "grid" als pointer to pointer to pointer to struct cell 
 * 
 * @param grid_ptr 
 * @param x_size 
 */
void dealloc_grid(struct cell *** grid_ptr, const int x_size){
    for (int i = 0; i < x_size; i++)
    {
        free((*grid_ptr)[i]);
    }
    free(*grid_ptr);
}

/**
 * @brief load_preset_to_grid speichert den Stand des Feldes als Preset in eine Textdatei.
 * 
 * @param grid_ptr 
 * @param gridsize 
 */
void save_preset_from_grid(struct cell ** grid_ptr, COORD gridsize){
    FILE *fp;
    fp = fopen("preset.txt", "w");

    if(fp == NULL) {
        printf("Datei konnte nicht geoeffnet werden.\n");
    }else {
        int x;
        int y;

        for(y = 0; y < gridsize.Y; y++){
            for(x = 0; x < gridsize.X; x++){
                fprintf(fp,"%i ",grid_ptr[x][y].alive);
            }
            fprintf(fp,"\n");
        }

        fclose(fp);
    }
}

/**
 * @brief load_preset_to_grid laed ein Preset aus einer Textdatei.
 * 
 * @param grid_ptr 
 * @param gridsize 
 */
void load_preset_to_grid(struct cell ** grid_ptr, COORD gridsize){
    FILE *fp;
    fp = fopen("preset.txt", "r");
    int tempalive;
    int x, y;

    if(fp == NULL) {
        save_preset_from_grid(grid_ptr, gridsize);
    }
    for(y = 0; y < gridsize.Y; y++){
        for(x = 0; x < gridsize.X; x++){
            fscanf(fp, "%d ", &tempalive);
            grid_ptr[x][y].alive = tempalive;
        }
    }
	fclose(fp);
}

/**
 * @brief copy_grid kopiert alle Werte eines Feldes in ein neues Feld
 * 
 * @param grid_ptr_dest 
 * @param grid_ptr_src 
 * @param gridsize 
 */
void copy_grid(struct cell ** grid_ptr_dest, struct cell ** grid_ptr_src, COORD gridsize){
    for(int j=0; j < gridsize.X; j++) {
        memcpy(grid_ptr_dest[j], grid_ptr_src[j], gridsize.Y * sizeof(struct cell));
   }

}

/**
 * @brief define_neighborhood definiert alle anliegenden Nachbarzellen für jede Zelle eines Feldes
 * 
 * @param grid_ptr 
 * @param gridsize 
 */
void define_neighborhood(struct cell ** grid_ptr, COORD gridsize){

    /*
        Nachbar wird wie folgt definiert:

                    1|2|3
                    4| |5
                    6|7|8
    */

    //Nachbaren eintragen
    int x, y;
    for(y = 0; y < gridsize.Y; y++){
        for(x = 0; x < gridsize.X; x++){

            grid_ptr[x][y].neighborCell[0] = &grid_ptr[x-1][y-1]; // Nachbar 1
            grid_ptr[x][y].neighborCell[1] = &grid_ptr[x][y-1]; // Nachbar 2

            grid_ptr[x][y].neighborCell[2] = &grid_ptr[x+1][y-1]; // Nachbar 3
            grid_ptr[x][y].neighborCell[3] = &grid_ptr[x-1][y]; // Nachbar 4

            grid_ptr[x][y].neighborCell[4] = &grid_ptr[x+1][y]; // Nachbar 5
            grid_ptr[x][y].neighborCell[5] = &grid_ptr[x-1][y+1]; // Nachbar 6

            grid_ptr[x][y].neighborCell[6] = &grid_ptr[x][y+1]; // Nachbar 7
            grid_ptr[x][y].neighborCell[7] = &grid_ptr[x+1][y+1]; // Nachbar 8

            //Unsere Zelle ein Linkes Kantenfeld ist
            if (x == 0) {
                grid_ptr[x][y].neighborCell[0] = &grid_ptr[gridsize.X-1][y-1]; 
                grid_ptr[x][y].neighborCell[3] = &grid_ptr[gridsize.X-1][y];
                grid_ptr[x][y].neighborCell[5] = &grid_ptr[gridsize.X-1][y+1];
            }
            
            //Unsere Zelle ein Rechtes Kantenfeld ist
            if (x == gridsize.X-1) {
                grid_ptr[x][y].neighborCell[2] = &grid_ptr[0][y-1];
                grid_ptr[x][y].neighborCell[4] = &grid_ptr[0][y];
                grid_ptr[x][y].neighborCell[7] = &grid_ptr[0][y+1];
            }
            
            //Unsere Zelle ein Oberes Kantenfeld ist
            if (y == 0) {
                grid_ptr[x][y].neighborCell[0] = &grid_ptr[x-1][gridsize.Y-1];
                grid_ptr[x][y].neighborCell[1] = &grid_ptr[x][gridsize.Y-1];
                grid_ptr[x][y].neighborCell[2] = &grid_ptr[x+1][gridsize.Y-1];
            }
            
            //Unsere Zelle ein Unteres Kantenfeld ist
            if (y == gridsize.Y-1) {
                grid_ptr[x][y].neighborCell[5] = &grid_ptr[x-1][0];
                grid_ptr[x][y].neighborCell[6] = &grid_ptr[x][0];
                grid_ptr[x][y].neighborCell[7] = &grid_ptr[x+1][0];

            }

            //Unsere Zelle ein in der Oberen Linken Ecke ist            
            if (x == 0 && y == 0){
                grid_ptr[x][y].neighborCell[0] = &grid_ptr[gridsize.X-1][gridsize.Y-1];
            }

            //Unsere Zelle ein in der Unteren Linken Ecke ist           
            if (x == 0 && y == gridsize.Y-1){
                grid_ptr[x][y].neighborCell[5] = &grid_ptr[gridsize.X-1][0];
            }

            //Unsere Zelle ein in der Oberen Rechten Ecke ist         
            if (x == gridsize.X-1 && y == 0){
                grid_ptr[x][y].neighborCell[2] = &grid_ptr[0][gridsize.Y-1];
            }

            //Unsere Zelle ein in der Unten Rechten Ecke ist      
            if (x == gridsize.X-1 && y == gridsize.Y-1){
                grid_ptr[x][y].neighborCell[7] = &grid_ptr[0][0];
            }
        }
    }
}

/**
 * @brief initialize_empty_grid initialisiert ein leeres grid
 * 
 * @param grid_ptr 
 * @param gridsize 
 */
void initialize_empty_grid(struct cell **grid_ptr, COORD gridsize){
    int x, y;
    for(y = 0; y < gridsize.Y; y++){
        for(x = 0; x < gridsize.X; x++){
            grid_ptr[x][y].alive = 0;
            grid_ptr[x][y].livingNeighbors = 0;
        }
    }
}

/**
 * @brief calc_all_neighbors initialisiert die livingNeighbors für jede Zelle.
 * 
 * @param grid_ptr 
 * @param gridsize 
 */
void calc_all_neighbors(struct cell **grid_ptr, COORD gridsize){
    int x, y;

    for(y = 0; y < gridsize.Y; y++){
        for(x = 0; x < gridsize.X; x++){
            grid_ptr[x][y].livingNeighbors = count_living_neighbors(grid_ptr[x][y]);
        }
    }
}

/**
 * @brief 
 * generate_random_grid initialisiert das übergebene Feld mit dem Modulo 2 (%2)
 * aus einer zufälligen Zahl für jede Zelle eines Feldes
 * 
 * @param cell **grid_ptr 
 * @param gridsize 
 */
void generate_random_grid(struct cell **grid_ptr, COORD gridsize){
    int x, y;

    for(y = 0; y < gridsize.Y; y++){
        for(x = 0; x < gridsize.X; x++){
            grid_ptr[x][y].alive = generate_random_int_msws() * rand() % 2;
        }
    }
}


/**
 * @brief 
 * count_living_neighbors zählt die lebenden umliegenden Nachbarn.
 * diese Funktion ist möglichst effizient geschrieben, da sie beim initialisieren eines 1000x1000 Feldes
 * 1.000.000x aufgerufen wird.
 * 
 * @param cell grid 
 * @return int 
 */
int count_living_neighbors(struct cell grid){
    return  grid.neighborCell[0] -> alive 
        +   grid.neighborCell[1] -> alive 
        +   grid.neighborCell[2] -> alive 
        +   grid.neighborCell[3] -> alive 
        +   grid.neighborCell[4] -> alive 
        +   grid.neighborCell[5] -> alive 
        +   grid.neighborCell[6] -> alive
        +   grid.neighborCell[7] -> alive;
}


/**
 * @brief add_neighborhood informiert jede anliegende Nachbarzelle über den aktuellen Stand der Zelle
 * das passiert durch das Hochzählen von livingNeighbors in jeder anliegenden Zelle
 * diese Funktion ist möglichst effizient geschrieben,
 * da sie bei einem 1000x1000 Feld bis zu 1.000.000x pro Tick aufgrufen werden kann.
 * 
 * @param cell grid 
 */
void add_neighborhood(struct cell grid){
    grid.neighborCell[0] -> livingNeighbors++;
    grid.neighborCell[1] -> livingNeighbors++;
    grid.neighborCell[2] -> livingNeighbors++;
    grid.neighborCell[3] -> livingNeighbors++;
    grid.neighborCell[4] -> livingNeighbors++;
    grid.neighborCell[5] -> livingNeighbors++;
    grid.neighborCell[6] -> livingNeighbors++;
    grid.neighborCell[7] -> livingNeighbors++;
}

/**
 * @brief sub_neighborhood informiert jede anliegende Nachbarzelle über den aktuellen Stand der Zelle
 * das passiert durch das Runterzählen von livingNeighbors in jeder anliegenden Zelle
 * diese Funktion ist möglichst effizient geschrieben, 
 * da sie bei einem 1000x1000 Feld bis zu 1.000.000x pro Tick aufgrufen werden kann.
 * 
 * @param struct cell grid 
 */
void sub_neighborhood(struct cell grid){
    grid.neighborCell[0] -> livingNeighbors--;
    grid.neighborCell[1] -> livingNeighbors--;
    grid.neighborCell[2] -> livingNeighbors--;
    grid.neighborCell[3] -> livingNeighbors--;
    grid.neighborCell[4] -> livingNeighbors--;
    grid.neighborCell[5] -> livingNeighbors--;
    grid.neighborCell[6] -> livingNeighbors--;
    grid.neighborCell[7] -> livingNeighbors--;
}


