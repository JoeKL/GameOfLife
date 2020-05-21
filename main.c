#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdint.h>
#include <conio.h>

#define X_Size 100
#define Y_Size 50

//structs
struct cell{
    int alive;
    struct cell *neighborCell[8];
};

struct settings{

    char symbolTrue;
    char symbolFalse;

    int gameTime;
    int iterationsPerSecond;

    int generation_pos_y;
    int generation_pos_x;
    
    int gameFieldSize_pos_y;
    int gameFieldSize_pos_x;

    int gameTime_pos_y;
    int gameTime_pos_x;

    int iterationsPerSecond_pos_y;
    int iterationsPerSecond_pos_x;

} gamesettings;

//Variablen
//seed für Zufallsgenerator
uint64_t x_msws = 0, w_msws = 0, s_msws = 0xb5ad4eceda1ce2a9;
struct cell gamefield[X_Size][Y_Size];
struct cell gamefieldcopy[X_Size][Y_Size];
int generation = 0;


//Funktionen
int CountLivingNeighbors(int x, int y);
void define_neighborhood(struct cell gamefield_ptr[X_Size][Y_Size]);

void initialize_game();
void generate_random_gamefield();
void load_preset();
void save_preset();

void run_game(int gameLengthInSenconds, int ticksPerSecond);
void tick();

void print_gamestate();
void draw_hud();
int set_cursor(int x, int y);
unsigned int generate_random_int_msws();


int main(){
    //setze den rand() seed auf Sekunden seit Epoche
    srand(time(NULL));

    //system("chcp 437");

    gamesettings.generation_pos_x = 10;
    gamesettings.generation_pos_y = 51;
    gamesettings.gameFieldSize_pos_x = 10;
    gamesettings.gameFieldSize_pos_y = 52;
    gamesettings.gameTime_pos_x = 50;
    gamesettings.gameTime_pos_y = 51;
    gamesettings.iterationsPerSecond_pos_x = 50;
    gamesettings.iterationsPerSecond_pos_y = 52;

    gamesettings.symbolTrue = '#';
    gamesettings.symbolFalse = '-';
    gamesettings.iterationsPerSecond = 60;
    gamesettings.gameTime = 5;


    initialize_game();

    generate_random_gamefield();
    //save_preset();
    //load_preset();    


    print_gamestate();

    run_game(gamesettings.gameTime, gamesettings.iterationsPerSecond);

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

    char buffer[sizeof(char)*X_Size*Y_Size*2+Y_Size];

    for(int y = 0; y < Y_Size; y++){
        for(int x = 0; x < X_Size; x++){
            if (gamefield[x][y].alive == 1) {

                if(x == 0 && y == 0){
                    snprintf(buffer, sizeof(buffer),"%c ", gamesettings.symbolTrue);
                } else {
                    snprintf(buffer + strlen(buffer), sizeof(buffer),"%c ", gamesettings.symbolTrue);
                }

            }else {

                if(x == 0 && y == 0){
                    snprintf(buffer, sizeof(buffer),"%c ", gamesettings.symbolFalse);
                } else {
                    snprintf(buffer + strlen(buffer), sizeof(buffer),"%c ", gamesettings.symbolFalse);
                }
            }
        }
        snprintf(buffer + strlen(buffer), sizeof(buffer),"\n");
    }
    set_cursor(0,0);
    printf("%s\n", buffer);
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
        draw_hud();

        // if ( _kbhit() ){
        //     char key_code = _getch();
        //     if(key_code == ' '){
        //        return;
        //     }
        // }
    }
}

void draw_hud(){
    
    set_cursor(gamesettings.generation_pos_x, gamesettings.generation_pos_y);
    printf("generation: %d", generation);
    
    set_cursor(gamesettings.gameFieldSize_pos_x, gamesettings.gameFieldSize_pos_y);
    printf("gamefield size: %dx%d", X_Size, Y_Size);

    set_cursor(gamesettings.gameTime_pos_x, gamesettings.gameTime_pos_y);
    printf("gametime: %ds", gamesettings.gameTime);

    set_cursor(gamesettings.iterationsPerSecond_pos_x, gamesettings.iterationsPerSecond_pos_y);
    printf("iterationsPerSecond: %d", gamesettings.iterationsPerSecond);


    set_cursor(0,0);
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

unsigned int generate_random_int_msws(){
    //Middle Square Weyl Sequence PRNG
    x_msws *= x_msws;
    x_msws += (w_msws += s_msws);
    //  >>    Bitmanipulation rechts
    //  <<    Bitmanipulation links
    //  |     Bitweises OR
    return (unsigned) (x_msws = (x_msws>>32) | (x_msws<<32));
}

void generate_random_gamefield(){
    int x, y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            gamefield[x][y].alive = generate_random_int_msws()*rand() % 2;
        }
    }
}
