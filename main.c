#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <pthread.h> 

#include "include/menu.h"
#include "include/game.h"



//Variablen
//seed für Zufallsgenerator

struct settings gamesettings;
struct cell grid[X_Size][Y_Size];
struct cell gridcopy[X_Size][Y_Size];

//Merkt sich die Anzahl der lebenden Zellen der aktuellen und beider vorherigen Generationen
int aliveCells;
//Zählt hoch, welche Generation gerade durchlaufen wird
int generation = 0;


//Funktionen


void run_game(int gameLengthInSenconds, int ticksPerSecond);
void tick();

void generate_random_grid();
void draw_hud();

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

void tick(){
    aliveCells = 0;
    memcpy(&gridcopy, &grid, sizeof(grid));
    define_neighborhood(gridcopy);

    int x;
    int y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            if (grid[x][y].alive) aliveCells++;

            int LivingNeighbors = count_living_neighbors(gridcopy, x, y);

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
    print_gamestate(grid, gamesettings);
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

void *start_random_game(void *vargp){

    generation = 0;
    initialize_grid(grid);
    generate_random_grid();
    run_game(gamesettings.gameTime, gamesettings.iterationsPerSecond);
    return NULL; 
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



void generate_random_grid(){
    int x, y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            grid[x][y].alive = generate_random_int_msws()*rand() % 2;
        }
    }
}

