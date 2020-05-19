#include <stdio.h>
#include <stdlib.h>

#define X_value 5
#define Y_value 5

struct cell{

    int alive;
    struct cell *neighborCell1;
    struct cell *neighborCell2;
    struct cell *neighborCell3;
    struct cell *neighborCell4;
    struct cell *neighborCell5;
    struct cell *neighborCell6;
    struct cell *neighborCell7;
    struct cell *neighborCell8;

};

struct cell gamefield[X_value][Y_value];

void initialize_game(){

    int x, y;

    //erzeugen des leeren Feldes
    for(y = 0; y < Y_value; y++){
        for(x = 0; x < X_value; x++){
            gamefield[x][y].alive = 0;
        }
    }

    define_neighborhood();

    //preset laden
}

void define_neighborhood(){

    /*
        Nachbar wird wie folgt definiert:

                    1|2|3
                    4| |5
                    6|7|8
    */

    //nachbarn eintragen
    int x, y;
    for(y = 0; y < Y_value; y++){
        for(x = 0; x < X_value; x++){
            gamefield[x][y].neighborCell1 = &gamefield[x-1][y-1];
            gamefield[x][y].neighborCell2 = &gamefield[x][y-1];

            gamefield[x][y].neighborCell3 = &gamefield[x+1][y-1];
            gamefield[x][y].neighborCell4 = &gamefield[x-1][y];

            gamefield[x][y].neighborCell5 = &gamefield[x+1][y];
            gamefield[x][y].neighborCell6 = &gamefield[x-1][y+1];

            gamefield[x][y].neighborCell7 = &gamefield[x][y+1];
            gamefield[x][y].neighborCell8 = &gamefield[x+1][y+1];

        }
    }
}

void print_gamestate(){
    int x, y;

    for(y = 0; y < Y_value; y++){
        for(x = 0; x < X_value; x++){
            printf("%i ",gamefield[x][y].alive);
        }
        printf("\n");
    }
}

void save_preset(){
    FILE *fp;
    fp = fopen("preset.txt", "w");

    if(fp == NULL) {
        printf("Datei konnte nicht geoeffnet werden.\n");
    }else {
        int x;
        int y;

        for(y = 0; y < Y_value; y++){
            for(x = 0; x < X_value; x++){
                fprintf(fp,"%i ",gamefield[x][y].alive);
            }
            fprintf(fp,"\n");
        }

        fclose(fp);
    }
}

void load_preset(){
    FILE *fp;
    fp = fopen("preset.txt", "r");
    int tempalive;
    int x, y;

    if(fp == NULL) {
        printf("Datei konnte nicht geoeffnet werden.\n");
    }else {
        for(y = 0; y < Y_value; y++){
            for(x = 0; x < X_value; x++){
                fscanf(fp, "%d ", &tempalive);
                gamefield[x][y].alive = tempalive;
            }
        }
	fclose(fp);
    }
}



int main()
{
    initialize_game();
    //save_preset();
    load_preset();

    //print_gamestate();

    printf("\nnachbar 1: %d", gamefield[2][2].neighborCell2 -> neighborCell1 -> alive);

    return 0;
}
