#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdint.h>
#include <conio.h>

#define X_Size 100
#define Y_Size 50

struct cell{
    int alive;
    struct cell *neighborCell[8];
};

//seed für Zufallsgenerator
uint64_t x_msws = 0, w_msws = 0, s_msws = 0xb5ad4eceda1ce2a9;
char symbolTrue, symbolFalse;
struct cell gamefield[X_Size][Y_Size];
struct cell gamefieldcopy[X_Size][Y_Size];
int generation = 1;


int CountLivingNeighbors(int x, int y);
void define_neighborhood(struct cell gamefield_ptr[X_Size][Y_Size]);


void initialize_game();
void random_gamefield();
void load_preset();
void save_preset();

void run_game(int gameLengthInSenconds, int ticksPerSecond);
void tick();

void print_gamestate();
int set_cursor(int x, int y);
int msws();


int main(){
    //setze den rand() seed auf Sekunden seit Epoche
    srand(time(NULL));

    //system("chcp 437");


    symbolTrue = '#';
    symbolFalse = '-';

    int iterationsPerSecond = 60;
    int gameTime = 5;


    initialize_game();

    random_gamefield();
    //save_preset();
    //load_preset();


    print_gamestate();

    run_game(gameTime, iterationsPerSecond);

    system("pause");
    return 0;
}


void initialize_game(){

    int x, y;

    //erzeugen des leeren Feldes
    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            gamefield[x][y].alive = 0;
        }
    }

    define_neighborhood(gamefield);
    //preset laden
}

void define_neighborhood(struct cell gamefield_ptr[X_Size][Y_Size]){

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

            gamefield_ptr[x][y].neighborCell[0] = &gamefield_ptr[x-1][y-1]; // Nachbar 1
            gamefield_ptr[x][y].neighborCell[1] = &gamefield_ptr[x][y-1]; // Nachbar 2

            gamefield_ptr[x][y].neighborCell[2] = &gamefield_ptr[x+1][y-1]; // Nachbar 3
            gamefield_ptr[x][y].neighborCell[3] = &gamefield_ptr[x-1][y]; // Nachbar 4

            gamefield_ptr[x][y].neighborCell[4] = &gamefield_ptr[x+1][y]; // Nachbar 5
            gamefield_ptr[x][y].neighborCell[5] = &gamefield_ptr[x-1][y+1]; // Nachbar 6

            gamefield_ptr[x][y].neighborCell[6] = &gamefield_ptr[x][y+1]; // Nachbar 7
            gamefield_ptr[x][y].neighborCell[7] = &gamefield_ptr[x+1][y+1]; // Nachbar 8

            //Unsere Zelle ein Linkes Kantenfeld ist
            if (x == 0) {
                gamefield_ptr[x][y].neighborCell[0] = &gamefield_ptr[X_Size-1][y-1]; 
                gamefield_ptr[x][y].neighborCell[3] = &gamefield_ptr[X_Size-1][y];
                gamefield_ptr[x][y].neighborCell[5] = &gamefield_ptr[X_Size-1][y+1];
            }
            
            //Unsere Zelle ein Rechtes Kantenfeld ist
            if (x == X_Size-1) {
                gamefield_ptr[x][y].neighborCell[2] = &gamefield_ptr[0][y-1];
                gamefield_ptr[x][y].neighborCell[4] = &gamefield_ptr[0][y];
                gamefield_ptr[x][y].neighborCell[7] = &gamefield_ptr[0][y+1];
            }
            
            //Unsere Zelle ein Oberes Kantenfeld ist
            if (y == 0) {
                gamefield_ptr[x][y].neighborCell[0] = &gamefield_ptr[x-1][Y_Size-1];
                gamefield_ptr[x][y].neighborCell[1] = &gamefield_ptr[x][Y_Size-1];
                gamefield_ptr[x][y].neighborCell[2] = &gamefield_ptr[x+1][Y_Size-1];
            }
            
            //Unsere Zelle ein Unteres Kantenfeld ist
            if (y == Y_Size-1) {
                gamefield_ptr[x][y].neighborCell[5] = &gamefield_ptr[x-1][0];
                gamefield_ptr[x][y].neighborCell[6] = &gamefield_ptr[x][0];
                gamefield_ptr[x][y].neighborCell[7] = &gamefield_ptr[x+1][0];

            }

            //Unsere Zelle ein in der Oberen Linken Ecke ist            
            if (x == 0 && y == 0){
                gamefield_ptr[x][y].neighborCell[0] = &gamefield_ptr[X_Size-1][Y_Size-1];
            }

            //Unsere Zelle ein in der Unteren Linken Ecke ist           
            if (x == 0 && y == Y_Size-1){
                gamefield_ptr[x][y].neighborCell[5] = &gamefield_ptr[X_Size-1][0];
            }

            //Unsere Zelle ein in der Oberen Rechten Ecke ist         
            if (x == X_Size-1 && y == 0){
                gamefield_ptr[x][y].neighborCell[2] = &gamefield_ptr[0][Y_Size-1];
            }

            //Unsere Zelle ein in der Unten Rechten Ecke ist      
            if (x == X_Size-1 && y == Y_Size-1){
                gamefield_ptr[x][y].neighborCell[7] = &gamefield_ptr[0][0];
            }
        }
    }
}

void print_gamestate(){
    set_cursor(0,0);
    char buffer[sizeof(char)*X_Size*Y_Size*2+Y_Size];

    for(int y = 0; y < Y_Size; y++){
        for(int x = 0; x < X_Size; x++){
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


void tick(){
    memcpy(&gamefieldcopy, &gamefield, sizeof(gamefield));
    define_neighborhood(gamefieldcopy);

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

        if ( _kbhit() ){
            char key_code = _getch();
            if(key_code == 'b'){
               return;
            }
        }
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

int set_cursor(int x, int y){
    // https://docs.microsoft.com/en-us/windows/console/console-functions
    COORD koordinaten;
    koordinaten.X= x;
    koordinaten.Y= y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), koordinaten);
    return 0;
}

int msws(){
    //Middle Square Weyl Sequence PRNG
    x_msws *= x_msws;
    x_msws += (w_msws += s_msws);
    return x_msws = (x_msws>>32) | (x_msws<<32);
}

void random_gamefield(){
    int x, y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            gamefield[x][y].alive = ((unsigned)msws())*rand() % 2;
        }
    }
}
