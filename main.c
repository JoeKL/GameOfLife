#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define X_Size 20
#define Y_Size 20

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


char symbolTrue, symbolFalse;
struct cell gamefield[X_Size][Y_Size];
struct cell gamefieldcopy[X_Size][Y_Size];

void initialize_game(){

    int x, y;

    //erzeugen des leeren Feldes
    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
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
    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){

            gamefield[x][y].neighborCell1 = &gamefield[x-1][y-1];
            gamefield[x][y].neighborCell2 = &gamefield[x][y-1];

            gamefield[x][y].neighborCell3 = &gamefield[x+1][y-1];
            gamefield[x][y].neighborCell4 = &gamefield[x-1][y];

            gamefield[x][y].neighborCell5 = &gamefield[x+1][y];
            gamefield[x][y].neighborCell6 = &gamefield[x-1][y+1];

            gamefield[x][y].neighborCell7 = &gamefield[x][y+1];
            gamefield[x][y].neighborCell8 = &gamefield[x+1][y+1];

            if (x == 0) {
                gamefield[x][y].neighborCell1 = &gamefield[X_Size-1][y-1];
                gamefield[x][y].neighborCell4 = &gamefield[X_Size-1][y];
                gamefield[x][y].neighborCell6 = &gamefield[X_Size-1][y+1];
            }
            if (y == 0) {
                gamefield[x][y].neighborCell1 = &gamefield[x-1][Y_Size-1];
                gamefield[x][y].neighborCell2 = &gamefield[x][Y_Size-1];
                gamefield[x][y].neighborCell3 = &gamefield[x+1][Y_Size-1];
            }
            if (x == X_Size-1) {
                gamefield[x][y].neighborCell3 = &gamefield[0][y-1];
                gamefield[x][y].neighborCell5 = &gamefield[0][y];
                gamefield[x][y].neighborCell8 = &gamefield[0][y+1];
            }
            if (y == Y_Size-1) {
                gamefield[x][y].neighborCell6 = &gamefield[x-1][0];
                gamefield[x][y].neighborCell7 = &gamefield[x][0];
                gamefield[x][y].neighborCell8 = &gamefield[x+1][0];

            }

            if (x == 0 && y == 0){
                gamefield[x][y].neighborCell1 = &gamefield[X_Size-1][Y_Size-1];
            }

            if (x == 0 && y == Y_Size-1){
                gamefield[x][y].neighborCell6 = &gamefield[X_Size-1][0];
            }

            if (x == X_Size-1 && y == 0){
                gamefield[x][y].neighborCell3 = &gamefield[0][Y_Size-1];
            }

            if (x == X_Size-1 && y == Y_Size-1){
                gamefield[x][y].neighborCell8 = &gamefield[0][0];
            }
        }
    }
}


void define_neighborhoodcopy(){

    /*
        Nachbar wird wie folgt definiert:

                    1|2|3
                    4| |5
                    6|7|8
    */

    //nachbarn eintragen

    int x, y;
    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){

            gamefieldcopy[x][y].neighborCell1 = &gamefieldcopy[x-1][y-1];
            gamefieldcopy[x][y].neighborCell2 = &gamefieldcopy[x][y-1];

            gamefieldcopy[x][y].neighborCell3 = &gamefieldcopy[x+1][y-1];
            gamefieldcopy[x][y].neighborCell4 = &gamefieldcopy[x-1][y];

            gamefieldcopy[x][y].neighborCell5 = &gamefieldcopy[x+1][y];
            gamefieldcopy[x][y].neighborCell6 = &gamefieldcopy[x-1][y+1];

            gamefieldcopy[x][y].neighborCell7 = &gamefieldcopy[x][y+1];
            gamefieldcopy[x][y].neighborCell8 = &gamefieldcopy[x+1][y+1];

            if (x == 0) {
                gamefieldcopy[x][y].neighborCell1 = &gamefieldcopy[X_Size-1][y-1];
                gamefieldcopy[x][y].neighborCell4 = &gamefieldcopy[X_Size-1][y];
                gamefieldcopy[x][y].neighborCell6 = &gamefieldcopy[X_Size-1][y+1];
            }
            if (y == 0) {
                gamefieldcopy[x][y].neighborCell1 = &gamefieldcopy[x-1][Y_Size-1];
                gamefieldcopy[x][y].neighborCell2 = &gamefieldcopy[x][Y_Size-1];
                gamefieldcopy[x][y].neighborCell3 = &gamefieldcopy[x+1][Y_Size-1];
            }
            if (x == X_Size-1) {
                gamefieldcopy[x][y].neighborCell3 = &gamefieldcopy[0][y-1];
                gamefieldcopy[x][y].neighborCell5 = &gamefieldcopy[0][y];
                gamefieldcopy[x][y].neighborCell8 = &gamefieldcopy[0][y+1];
            }
            if (y == Y_Size-1) {
                gamefieldcopy[x][y].neighborCell6 = &gamefieldcopy[x-1][0];
                gamefieldcopy[x][y].neighborCell7 = &gamefieldcopy[x][0];
                gamefieldcopy[x][y].neighborCell8 = &gamefieldcopy[x+1][0];

            }

            if (x == 0 && y == 0){
                gamefieldcopy[x][y].neighborCell1 = &gamefieldcopy[X_Size-1][Y_Size-1];
            }

            if (x == 0 && y == Y_Size-1){
                gamefieldcopy[x][y].neighborCell6 = &gamefieldcopy[X_Size-1][0];
            }

            if (x == X_Size-1 && y == 0){
                gamefieldcopy[x][y].neighborCell3 = &gamefieldcopy[0][Y_Size-1];
            }

            if (x == X_Size-1 && y == Y_Size-1){
                gamefieldcopy[x][y].neighborCell8 = &gamefieldcopy[0][0];
            }
        }
    }
}


void print_gamestate(){
    int x, y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            if (gamefield[x][y].alive == 1) {
                printf("%c ", symbolTrue);
            }else {
                printf("%c ", symbolFalse);
            }
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

        for(y = 0; y < Y_Size; y++){
            for(x = 0; x < X_Size; x++){
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
        for(y = 0; y < Y_Size; y++){
            for(x = 0; x < X_Size; x++){
                fscanf(fp, "%d ", &tempalive);
                gamefield[x][y].alive = tempalive;
            }
        }
	fclose(fp);
    }
}

void run_game(int gameLengthInSenconds, int ticksPerSecond){

    int tickCounter = 0;

    while(tickCounter < ticksPerSecond*gameLengthInSenconds) {
        double cpu_time_used;
        clock_t start, end;
        tickCounter++;

        start = clock();
        do {
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        } while(cpu_time_used < (double) 1/ticksPerSecond);
        tick();
    }
}

void tick(){
    memcpy(&gamefieldcopy, &gamefield, sizeof(gamefield));
    define_neighborhoodcopy();

    int x;
    int y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            //CountLivingNeighbors();
            int CountLivingNeighbors = 0;
            if(gamefieldcopy[x][y].neighborCell1 -> alive == 1){
                CountLivingNeighbors++;
            }
            if(gamefieldcopy[x][y].neighborCell2 -> alive == 1) {
                CountLivingNeighbors++;
            }
            if(gamefieldcopy[x][y].neighborCell3 -> alive == 1) {
                CountLivingNeighbors++;
            }
            if(gamefieldcopy[x][y].neighborCell4 -> alive == 1) {
                CountLivingNeighbors++;
            }
            if(gamefieldcopy[x][y].neighborCell5 -> alive == 1) {
                CountLivingNeighbors++;
            }
            if(gamefieldcopy[x][y].neighborCell6 -> alive == 1) {
                CountLivingNeighbors++;
            }
            if(gamefieldcopy[x][y].neighborCell7 -> alive == 1) {
                CountLivingNeighbors++;
            }
            if(gamefieldcopy[x][y].neighborCell8 -> alive == 1) {
                CountLivingNeighbors++;
            }

            //Eine tote Zelle mit genau drei lebenden Nachbarn wird in der Folgegeneration neu geboren.
            if(CountLivingNeighbors == 3 && gamefieldcopy[x][y].alive == 0){
                gamefield[x][y].alive = 1;
            }

            //Lebende Zellen mit weniger als zwei lebenden Nachbarn sterben in der Folgegeneration an Einsamkeit.
            if (gamefieldcopy[x][y].alive == 1 && CountLivingNeighbors < 2) {
                    gamefield[x][y].alive = 0;
            }
            //Eine lebende Zelle mit zwei oder drei lebenden Nachbarn bleibt in der Folgegeneration lebend.
            if (gamefieldcopy[x][y].alive == 1 && (CountLivingNeighbors == 2 || CountLivingNeighbors == 3)) {
                    gamefield[x][y].alive = 1;
            }
            //Lebende Zellen mit mehr als drei lebenden Nachbarn sterben in der Folgegeneration an  Überbevölkerung.
            if (gamefieldcopy[x][y].alive == 1 && CountLivingNeighbors > 3) {
                    gamefield[x][y].alive = 0;
            }
        }
    }
    system("cls");
    print_gamestate();
}

int main(){
    symbolTrue = 'X';
    symbolFalse = '-';

    int iterationsPerSecond = 10;
    int gameTime = 10;

    initialize_game();
    //save_preset();
    load_preset();

    print_gamestate();

    run_game(gameTime, iterationsPerSecond);


    return 0;
}
