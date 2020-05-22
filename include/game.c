#include "game.h"
#include "menu.h"


uint64_t x_msws = 0, w_msws = 0, s_msws = 0xb5ad4eceda1ce2a9;

unsigned int generate_random_int_msws(){
    //Middle Square Weyl Sequence PRNG
    x_msws *= x_msws;
    x_msws += (w_msws += s_msws);
    //  >>    Bitmanipulation rechts
    //  <<    Bitmanipulation links
    //  |     Bitweises OR
    return (unsigned) (x_msws = (x_msws>>32) | (x_msws<<32));
}

int count_living_neighbors(struct cell grid_ptr[X_Size][Y_Size], int x, int y){
    int count = 0;
    int i = 0;
    for(i; i<8; i++){
        if(grid_ptr[x][y].neighborCell[i] -> alive == 1){
            count++;
        }
    }

    return count;
}

void save_preset(struct cell grid_ptr[X_Size][Y_Size]){
    FILE *fp;
    fp = fopen("preset.txt", "w");

    if(fp == NULL) {
        printf("Datei konnte nicht geoeffnet werden.\n");
    }else {
        int x;
        int y;

        for(y = 0; y < Y_Size; y++){
            for(x = 0; x < X_Size; x++){
                fprintf(fp,"%i ",grid_ptr[x][y].alive);
            }
            fprintf(fp,"\n");
        }

        fclose(fp);
    }
}

void load_preset(struct cell grid_ptr[X_Size][Y_Size]){
    FILE *fp;
    fp = fopen("preset.txt", "r");
    int tempalive;
    int x, y;

    if(fp == NULL) {
        printf("Datei konnte nicht geoeffnet werden.\n");
    }else {
        for(y = 0; y < Y_Size; y++){
            for(x = 0; x < X_Size; x++){
                fscanf(fp, "%d ", &tempalive);
                grid_ptr[x][y].alive = tempalive;
            }
        }
	fclose(fp);
    }
}

void define_neighborhood(struct cell grid_ptr[X_Size][Y_Size]){

    /*
        Nachbar wird wie folgt definiert:

                    1|2|3
                    4| |5
                    6|7|8
    */

    //Nachbaren eintragen
    int x, y;
    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){

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
                grid_ptr[x][y].neighborCell[0] = &grid_ptr[X_Size-1][y-1]; 
                grid_ptr[x][y].neighborCell[3] = &grid_ptr[X_Size-1][y];
                grid_ptr[x][y].neighborCell[5] = &grid_ptr[X_Size-1][y+1];
            }
            
            //Unsere Zelle ein Rechtes Kantenfeld ist
            if (x == X_Size-1) {
                grid_ptr[x][y].neighborCell[2] = &grid_ptr[0][y-1];
                grid_ptr[x][y].neighborCell[4] = &grid_ptr[0][y];
                grid_ptr[x][y].neighborCell[7] = &grid_ptr[0][y+1];
            }
            
            //Unsere Zelle ein Oberes Kantenfeld ist
            if (y == 0) {
                grid_ptr[x][y].neighborCell[0] = &grid_ptr[x-1][Y_Size-1];
                grid_ptr[x][y].neighborCell[1] = &grid_ptr[x][Y_Size-1];
                grid_ptr[x][y].neighborCell[2] = &grid_ptr[x+1][Y_Size-1];
            }
            
            //Unsere Zelle ein Unteres Kantenfeld ist
            if (y == Y_Size-1) {
                grid_ptr[x][y].neighborCell[5] = &grid_ptr[x-1][0];
                grid_ptr[x][y].neighborCell[6] = &grid_ptr[x][0];
                grid_ptr[x][y].neighborCell[7] = &grid_ptr[x+1][0];

            }

            //Unsere Zelle ein in der Oberen Linken Ecke ist            
            if (x == 0 && y == 0){
                grid_ptr[x][y].neighborCell[0] = &grid_ptr[X_Size-1][Y_Size-1];
            }

            //Unsere Zelle ein in der Unteren Linken Ecke ist           
            if (x == 0 && y == Y_Size-1){
                grid_ptr[x][y].neighborCell[5] = &grid_ptr[X_Size-1][0];
            }

            //Unsere Zelle ein in der Oberen Rechten Ecke ist         
            if (x == X_Size-1 && y == 0){
                grid_ptr[x][y].neighborCell[2] = &grid_ptr[0][Y_Size-1];
            }

            //Unsere Zelle ein in der Unten Rechten Ecke ist      
            if (x == X_Size-1 && y == Y_Size-1){
                grid_ptr[x][y].neighborCell[7] = &grid_ptr[0][0];
            }
        }
    }
}


void initialize_grid(struct cell grid_ptr[X_Size][Y_Size]){
    int x, y;

    //erzeugen des leeren Feldes
    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            grid_ptr[x][y].alive = 0;
        }
    }

    define_neighborhood(grid_ptr);
}


void print_gamestate(struct cell grid_ptr[X_Size][Y_Size], struct settings gamesettings){

    char buffer[sizeof(char)*X_Size*Y_Size*2+Y_Size];

    for(int y = 0; y < Y_Size; y++){
        for(int x = 0; x < X_Size; x++){
            if (grid_ptr[x][y].alive == 1) {

                if(x == 0 && y == 0){
                    snprintf(buffer, sizeof(buffer),"%c ", gamesettings.symbolAlive);
                } else {
                    snprintf(buffer + strlen(buffer), sizeof(buffer),"%c ", gamesettings.symbolAlive);
                }

            }else {

                if(x == 0 && y == 0){
                    snprintf(buffer, sizeof(buffer),"%c ", gamesettings.symbolDead);
                } else {
                    snprintf(buffer + strlen(buffer), sizeof(buffer),"%c ", gamesettings.symbolDead);
                }
            }
        }
        snprintf(buffer + strlen(buffer), sizeof(buffer),"\n");
    }
    set_cursor(0,0);
    printf("%s\n", buffer);
    set_cursor(0,0);
}