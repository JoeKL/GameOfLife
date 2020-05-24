#include "game.h"

uint64_t x_msws = 0, w_msws = 0;
// Must be odd (least significant bit is "1"), and upper 64-bits non-zero
uint64_t seed_msws = 0xb5ad4eceda1ce2a9; // qualifying seed

// return 32-bit number
uint32_t generate_random_int_msws() {
    // https://pthree.org/2018/07/30/middle-square-weyl-sequence-prng/
    x_msws *= x_msws; // square the number
    w_msws += seed_msws; // the weyl sequence
    x_msws += w_msws; // apply to x
    return x_msws = (x_msws>>32) | (x_msws<<32); // return the middle 32-bits
}

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

// struct cell ***grid_ptr -> declare grid as pointer to pointer to pointer to struct cell
void alloc_grid(struct cell ***grid_ptr, COORD gridsize){

    *grid_ptr = (struct cell **)malloc(gridsize.X * sizeof(struct cell *));
    for (int i = 0; i < gridsize.X; i++)
    {   
        (*grid_ptr)[i] = (struct cell *)malloc(gridsize.Y * sizeof(struct cell));
    }
}

void dealloc_grid(struct cell *** grid_ptr, const int x_size){
    for (int i = 0; i < x_size; i++)
    {
        free((*grid_ptr)[i]);
    }
    free(*grid_ptr);
}


void save_preset(struct cell ** grid_ptr, COORD gridsize){
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

void copy_grid(struct cell ** grid_ptr_dest, struct cell ** grid_ptr_src, COORD gridsize){
    for(int j=0; j < gridsize.X; j++) {
        memcpy(grid_ptr_dest[j], grid_ptr_src[j], gridsize.Y * sizeof(struct cell));
   }

}

void load_preset(struct cell ** grid_ptr, COORD gridsize){
    FILE *fp;
    fp = fopen("preset.txt", "r");
    int tempalive;
    int x, y;

    if(fp == NULL) {
        printf("Datei konnte nicht geoeffnet werden.\n");
    }else {
        for(y = 0; y < gridsize.Y; y++){
            for(x = 0; x < gridsize.X; x++){
                fscanf(fp, "%d ", &tempalive);
                grid_ptr[x][y].alive = tempalive;
            }
        }
	fclose(fp);
    }
}

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


void initialize_grid(struct cell **grid_ptr, COORD gridsize){
    int x, y;
    //erzeugen des leeren Feldes
    for(y = 0; y < gridsize.Y; y++){
        for(x = 0; x < gridsize.X; x++){
            grid_ptr[x][y].alive = 0;
            grid_ptr[x][y].livingNeighbors = 0;
        }
    }
}


void initialize_neighbors(struct cell **grid_ptr, COORD gridsize){
    int x, y;

    for(y = 0; y < gridsize.Y; y++){
        for(x = 0; x < gridsize.X; x++){
            grid_ptr[x][y].livingNeighbors = count_living_neighbors(grid_ptr[x][y]);
        }
    }
}

void generate_random_grid(struct cell **grid_ptr, COORD gridsize){
    int x, y;

    for(y = 0; y < gridsize.Y; y++){
        for(x = 0; x < gridsize.X; x++){
            grid_ptr[x][y].alive = generate_random_int_msws() * rand() % 2;
        }
    }
}

void refresh_neighborhood(struct cell grid, int value){
    int i = 0;
    for(i; i<8; i++){
        if(grid.neighborCell[i] -> livingNeighbors != 0 || value > 0){
            grid.neighborCell[i] -> livingNeighbors += value;
        }
    }
}

