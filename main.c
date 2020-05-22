#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdint.h>
#include <conio.h>
#include <pthread.h> 

#include "include/test.h"

#define X_Size 118
#define Y_Size 57

COORD grid_size;

//structs
struct cell{
    int alive;
    struct cell *neighborCell[8];
};

struct settings{

    char symbolAlive;
    char symbolDead;

    int gameTime;
    int iterationsPerSecond;

    int generation_pos_x;
    int generation_pos_y;
    
    int gridSize_pos_x;
    int gridSize_pos_y;

    int gameTime_pos_x;
    int gameTime_pos_y;

    int iterationsPerSecond_pos_x;
    int iterationsPerSecond_pos_y;

    int aliveCells_pos_x;
    int aliveCells_pos_y;

} gamesettings;

//Variablen
//seed für Zufallsgenerator
uint64_t x_msws = 0, w_msws = 0, s_msws = 0xb5ad4eceda1ce2a9;
struct cell grid[X_Size][Y_Size];
struct cell gridcopy[X_Size][Y_Size];

//Merkt sich die Anzahl der lebenden Zellen der aktuellen und beider vorherigen Generationen
int aliveCells;
//Zählt hoch, welche Generation gerade durchlaufen wird
int generation = 0;


//Funktionen
int count_living_neighbors(int x, int y);
void define_neighborhood(struct cell grid_ptr[X_Size][Y_Size]);

void initialize_game();
void generate_random_grid();
void load_preset();
void save_preset();

void run_game(int gameLengthInSenconds, int ticksPerSecond);
void tick();

void print_gamestate();
void draw_hud();
int set_cursor(int x, int y);
unsigned int generate_random_int_msws();
COORD get_console_window_size(HANDLE hConsoleOutput);

void erase_menu_cursors();
void main_menu();


int main(){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleDisplayMode(consoleHandle,CONSOLE_FULLSCREEN_MODE,0);
    COORD consoleSize = get_console_window_size(consoleHandle);
    SetConsoleScreenBufferSize(consoleHandle, consoleSize);     

    //setze den rand() seed auf Sekunden seit Epoche
    srand(time(NULL));


    //system("chcp 437");

    gamesettings.generation_pos_x = 10;
    gamesettings.generation_pos_y = 57;
    gamesettings.aliveCells_pos_x = 10;
    gamesettings.aliveCells_pos_y = 58;
    gamesettings.gameTime_pos_x = 50;
    gamesettings.gameTime_pos_y = 57;
    gamesettings.iterationsPerSecond_pos_x = 50;
    gamesettings.iterationsPerSecond_pos_y = 58;
    gamesettings.gridSize_pos_x = 50;
    gamesettings.gridSize_pos_y = 59;

    gamesettings.symbolAlive = '#';
    gamesettings.symbolDead = '-';
    gamesettings.iterationsPerSecond = 60;
    gamesettings.gameTime = 10;
    

    main_menu();

    // initialize_game();
    // generate_random_grid();
    // // save_preset();
    // // load_preset();    
    // print_gamestate();
    // run_game(gamesettings.gameTime, gamesettings.iterationsPerSecond);

    
    system("pause");
    return 0;
}


void initialize_game(){
    int x, y;

    //erzeugen des leeren Feldes
    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            grid[x][y].alive = 0;
        }
    }

    define_neighborhood(grid);
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

void print_gamestate(){

    char buffer[sizeof(char)*X_Size*Y_Size*2+Y_Size];

    for(int y = 0; y < Y_Size; y++){
        for(int x = 0; x < X_Size; x++){
            if (grid[x][y].alive == 1) {

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
                fprintf(fp,"%i ",grid[x][y].alive);
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
                grid[x][y].alive = tempalive;
            }
        }
	fclose(fp);
    }
}

void tick(){
    aliveCells = 0;
    memcpy(&gridcopy, &grid, sizeof(grid));
    define_neighborhood(gridcopy);

    int x;
    int y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            if (grid[x][y].alive) aliveCells++;

            int LivingNeighbors = count_living_neighbors(x, y);

            //Eine tote Zelle mit genau drei lebenden Nachbarn wird in der Folgegeneration neu geboren.
            if(LivingNeighbors == 3 && gridcopy[x][y].alive == 0){
                grid[x][y].alive = 1;
            }

            //Lebende Zellen mit weniger als zwei lebenden Nachbarn sterben in der Folgegeneration an Einsamkeit.
            if (gridcopy[x][y].alive == 1 && LivingNeighbors < 2) {
                    grid[x][y].alive = 0;
            }
            //Eine lebende Zelle mit zwei oder drei lebenden Nachbarn bleibt in der Folgegeneration lebend.
            if (gridcopy[x][y].alive == 1 && (LivingNeighbors == 2 || LivingNeighbors == 3)) {
                    grid[x][y].alive = 1;
            }
            //Lebende Zellen mit mehr als drei lebenden Nachbarn sterben in der Folgegeneration an  Überbevölkerung.
            if (gridcopy[x][y].alive == 1 && LivingNeighbors > 3) {
                    grid[x][y].alive = 0;
            }
        }
    }

    print_gamestate();

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
    }
}

void draw_hud(){
    
    set_cursor(gamesettings.generation_pos_x, gamesettings.generation_pos_y);
    printf("generation: %d of %d", generation, gamesettings.iterationsPerSecond*gamesettings.gameTime);

    set_cursor(gamesettings.aliveCells_pos_x, gamesettings.aliveCells_pos_y);
    printf("cells alive: %d of %d", aliveCells, X_Size*Y_Size);
    
    set_cursor(gamesettings.gridSize_pos_x, gamesettings.gridSize_pos_y);
    printf("grid size: %dx%d", X_Size, Y_Size);

    set_cursor(gamesettings.gameTime_pos_x, gamesettings.gameTime_pos_y);
    printf("gametime: %ds", gamesettings.gameTime);

    set_cursor(gamesettings.iterationsPerSecond_pos_x, gamesettings.iterationsPerSecond_pos_y);
    printf("iterationsPerSecond: %d", gamesettings.iterationsPerSecond);


    set_cursor(0,0);
}

int count_living_neighbors(int x, int y){
    int count = 0;
    int i = 0;
    for(i; i<8; i++){
        if(gridcopy[x][y].neighborCell[i] -> alive == 1){
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


COORD get_console_window_size(HANDLE hConsoleOutput){
    COORD size;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return size;
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

void generate_random_grid(){
    int x, y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            grid[x][y].alive = generate_random_int_msws()*rand() % 2;
        }
    }
}

void *start_random_game(void *vargp){

    generation = 0;
    initialize_game();
    generate_random_grid();
    print_gamestate();       
    run_game(gamesettings.gameTime, gamesettings.iterationsPerSecond);
    return NULL; 
}

void draw_main_menu(){
    set_cursor(10,10);
    printf("Start");
    set_cursor(10,20);
    printf("Settings");
    set_cursor(10,30);
    printf("Exit");
    set_cursor(0,0);
}

void main_menu(){

    int main_menu_cursor;


    main_menu_cursor = 0;

    int run = 1;
    while (run == 1)
    {
        
                        draw_main_menu();

        switch (main_menu_cursor)
        {

            case 0:
                erase_menu_cursors();
                set_cursor(10-3,10);
                printf("-->");
                set_cursor(0,0);
                break;

            case 1:
                erase_menu_cursors();
                set_cursor(10-3,20);
                printf("-->");
                set_cursor(0,0);
                break;
            
            case 2:
                erase_menu_cursors();
                set_cursor(10-3,30);
                printf("-->");
                set_cursor(0,0);
                break;
            
        
        default:
            break;
        }

        // int ch;
        // while ((ch = _getch()) != 27) /* 27 = Esc key */
        // {
        //     printf("%d", ch);
        //     if (ch == 0 || ch == 224)
        //         printf (", %d", _getch ()); 
        //     printf("\n");
        // }

        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            switch (_getch())
            {
                //UP
                case 72:
                        set_cursor(0,0);
                        // printf("u %d", main_menu_cursor);
                        main_menu_cursor--;
                    break;

                //DOWN
                case 80:
                        set_cursor(0,0);
                        // printf("d %d", main_menu_cursor);
                        main_menu_cursor++;
                    break;

                //LEFT
                case 75:
                    break;
                                    
                //RIGHT
                case 77:
                    break;


            }
        } else {
            switch (ch)
            {
                //ENTER
                case 13:

                    switch (main_menu_cursor)
                    {
                    case 0:
                        // run = 0;
                        pthread_t thread_id;
                        pthread_create(&thread_id, NULL, start_random_game, NULL); 
                        pthread_join(thread_id, NULL);   
                        system("cls");                
                        break;
                    
                    case 1:
                        /* code */
                        break;
                        
                    case 2:
                        exit(0);
                        break;

                    default:
                        break;
                    }

                    break;
                //ESC
                case 27:
                    exit(0);
                    break;
            }
        }
        
        if(main_menu_cursor < 0){
            main_menu_cursor = 2;
        }

        if(main_menu_cursor > 2){
            main_menu_cursor = 0;
        }
        
    }
    

}

void erase_menu_cursors(){
    set_cursor(10-3,10);
                printf("   ");
    set_cursor(10-3,20);
                printf("   ");
    set_cursor(10-3,30);
                printf("   ");
    set_cursor(0,0);
}