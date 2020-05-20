#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#define X_Size 100
#define Y_Size 50

struct cell{
    int alive;
    struct cell *neighborCell[8];
};

char symbolTrue, symbolFalse;
struct cell gamefield[X_Size][Y_Size];
struct cell gamefieldcopy[X_Size][Y_Size];
int generation = 1;


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

            gamefield[x][y].neighborCell[0] = &gamefield[x-1][y-1];
            gamefield[x][y].neighborCell[1] = &gamefield[x][y-1];

            gamefield[x][y].neighborCell[2] = &gamefield[x+1][y-1];
            gamefield[x][y].neighborCell[3] = &gamefield[x-1][y];

            gamefield[x][y].neighborCell[4] = &gamefield[x+1][y];
            gamefield[x][y].neighborCell[5] = &gamefield[x-1][y+1];

            gamefield[x][y].neighborCell[6] = &gamefield[x][y+1];
            gamefield[x][y].neighborCell[7] = &gamefield[x+1][y+1];

            if (x == 0) {
                gamefield[x][y].neighborCell[0] = &gamefield[X_Size-1][y-1];
                gamefield[x][y].neighborCell[3] = &gamefield[X_Size-1][y];
                gamefield[x][y].neighborCell[5] = &gamefield[X_Size-1][y+1];
            }
            if (y == 0) {
                gamefield[x][y].neighborCell[0] = &gamefield[x-1][Y_Size-1];
                gamefield[x][y].neighborCell[1] = &gamefield[x][Y_Size-1];
                gamefield[x][y].neighborCell[2] = &gamefield[x+1][Y_Size-1];
            }
            if (x == X_Size-1) {
                gamefield[x][y].neighborCell[2] = &gamefield[0][y-1];
                gamefield[x][y].neighborCell[4] = &gamefield[0][y];
                gamefield[x][y].neighborCell[7] = &gamefield[0][y+1];
            }
            if (y == Y_Size-1) {
                gamefield[x][y].neighborCell[5] = &gamefield[x-1][0];
                gamefield[x][y].neighborCell[6] = &gamefield[x][0];
                gamefield[x][y].neighborCell[7] = &gamefield[x+1][0];

            }

            if (x == 0 && y == 0){
                gamefield[x][y].neighborCell[0] = &gamefield[X_Size-1][Y_Size-1];
            }

            if (x == 0 && y == Y_Size-1){
                gamefield[x][y].neighborCell[5] = &gamefield[X_Size-1][0];
            }

            if (x == X_Size-1 && y == 0){
                gamefield[x][y].neighborCell[2] = &gamefield[0][Y_Size-1];
            }

            if (x == X_Size-1 && y == Y_Size-1){
                gamefield[x][y].neighborCell[7] = &gamefield[0][0];
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

            gamefieldcopy[x][y].neighborCell[0] = &gamefieldcopy[x-1][y-1];
            gamefieldcopy[x][y].neighborCell[1] = &gamefieldcopy[x][y-1];

            gamefieldcopy[x][y].neighborCell[2] = &gamefieldcopy[x+1][y-1];
            gamefieldcopy[x][y].neighborCell[3] = &gamefieldcopy[x-1][y];

            gamefieldcopy[x][y].neighborCell[4] = &gamefieldcopy[x+1][y];
            gamefieldcopy[x][y].neighborCell[5] = &gamefieldcopy[x-1][y+1];

            gamefieldcopy[x][y].neighborCell[6] = &gamefieldcopy[x][y+1];
            gamefieldcopy[x][y].neighborCell[7] = &gamefieldcopy[x+1][y+1];

            if (x == 0) {
                gamefieldcopy[x][y].neighborCell[0] = &gamefieldcopy[X_Size-1][y-1];
                gamefieldcopy[x][y].neighborCell[3] = &gamefieldcopy[X_Size-1][y];
                gamefieldcopy[x][y].neighborCell[5] = &gamefieldcopy[X_Size-1][y+1];
            }
            if (y == 0) {
                gamefieldcopy[x][y].neighborCell[0] = &gamefieldcopy[x-1][Y_Size-1];
                gamefieldcopy[x][y].neighborCell[1] = &gamefieldcopy[x][Y_Size-1];
                gamefieldcopy[x][y].neighborCell[2] = &gamefieldcopy[x+1][Y_Size-1];
            }
            if (x == X_Size-1) {
                gamefieldcopy[x][y].neighborCell[2] = &gamefieldcopy[0][y-1];
                gamefieldcopy[x][y].neighborCell[4] = &gamefieldcopy[0][y];
                gamefieldcopy[x][y].neighborCell[7] = &gamefieldcopy[0][y+1];
            }
            if (y == Y_Size-1) {
                gamefieldcopy[x][y].neighborCell[5] = &gamefieldcopy[x-1][0];
                gamefieldcopy[x][y].neighborCell[6] = &gamefieldcopy[x][0];
                gamefieldcopy[x][y].neighborCell[7] = &gamefieldcopy[x+1][0];

            }

            if (x == 0 && y == 0){
                gamefieldcopy[x][y].neighborCell[0] = &gamefieldcopy[X_Size-1][Y_Size-1];
            }

            if (x == 0 && y == Y_Size-1){
                gamefieldcopy[x][y].neighborCell[5] = &gamefieldcopy[X_Size-1][0];
            }

            if (x == X_Size-1 && y == 0){
                gamefieldcopy[x][y].neighborCell[2] = &gamefieldcopy[0][Y_Size-1];
            }

            if (x == X_Size-1 && y == Y_Size-1){
                gamefieldcopy[x][y].neighborCell[7] = &gamefieldcopy[0][0];
            }
        }
    }
}


void print_gamestate(){
    set_cursor(0,0);
    char buffer[sizeof(char)*X_Size*Y_Size*2+Y_Size];
    int x, y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            if (gamefield[x][y].alive == 1) {

                if(x == 0 && y == 0){
                    snprintf(buffer, sizeof(buffer),"%c ", symbolTrue);
                } else {
                    snprintf(buffer + strlen(buffer), sizeof(buffer),"%c ", symbolTrue);
                }

            }else {

                if(x == 0 && y == 0){
                    snprintf(buffer, sizeof(buffer),"%c ", symbolFalse);
                } else {
                    snprintf(buffer + strlen(buffer), sizeof(buffer),"%c ", symbolFalse);
                }
            }
        }
        snprintf(buffer + strlen(buffer), sizeof(buffer),"\n");
    }
    printf("%s\n", buffer);
    printf("generation: %d", generation);
    set_cursor(0,0);
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

int CountLivingNeighbors(int x, int y){
    int count = 0;
    int i = 0;
    for(i; i<8; i++){
        if(gamefieldcopy[x][y].neighborCell[i] -> alive == 1){
            count++;
        }
    }

    return count;
}

void tick(){
    memcpy(&gamefieldcopy, &gamefield, sizeof(gamefield));
    define_neighborhoodcopy();

    int x;
    int y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){

            int LivingNeighbors = CountLivingNeighbors(x, y);

            //Eine tote Zelle mit genau drei lebenden Nachbarn wird in der Folgegeneration neu geboren.
            if(LivingNeighbors == 3 && gamefieldcopy[x][y].alive == 0){
                gamefield[x][y].alive = 1;
            }

            //Lebende Zellen mit weniger als zwei lebenden Nachbarn sterben in der Folgegeneration an Einsamkeit.
            if (gamefieldcopy[x][y].alive == 1 && LivingNeighbors < 2) {
                    gamefield[x][y].alive = 0;
            }
            //Eine lebende Zelle mit zwei oder drei lebenden Nachbarn bleibt in der Folgegeneration lebend.
            if (gamefieldcopy[x][y].alive == 1 && (LivingNeighbors == 2 || LivingNeighbors == 3)) {
                    gamefield[x][y].alive = 1;
            }
            //Lebende Zellen mit mehr als drei lebenden Nachbarn sterben in der Folgegeneration an  Überbevölkerung.
            if (gamefieldcopy[x][y].alive == 1 && LivingNeighbors > 3) {
                    gamefield[x][y].alive = 0;
            }
        }
    }

    print_gamestate();

    //printf("generation: %d", generation);
    generation++;
}

int set_cursor(int x, int y)
{
    COORD koordinaten;
    koordinaten.X= x;
    koordinaten.Y= y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), koordinaten);
    return 0;
}

int main(){
    symbolTrue = '#';
    symbolFalse = ' ';

    int iterationsPerSecond = 60;
    int gameTime = 500;


    initialize_game();
    //save_preset();
    load_preset();


    print_gamestate();

    run_game(gameTime, iterationsPerSecond);

    return 0;
}
