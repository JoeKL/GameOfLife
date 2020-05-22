#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <pthread.h> 
#include <unistd.h>

#include "include/menu.h"
#include "include/game.h"

struct settings gamesettings;
struct cell grid[X_Size][Y_Size];
struct cell gridcopy[X_Size][Y_Size];

struct menu_button mainMenu_Button[3];

int aliveCells = 0;
int currentGeneration = 0;

void run_ticks(int periodInSeconds, int ticksPerSecond);
void tick();

void generate_random_grid(struct cell grid_ptr[X_Size][Y_Size]);

void draw_hud();
void main_menu();
void init_settings();

int main(){

    console_fullscreen();
    init_settings();
    main_menu();
    
    system("pause");
    return 0;
}

void init_settings(){
    //setze den rand() seed auf Sekunden seit Epoche
    srand(time(NULL));

    //setze hud koordinaten
    gamesettings.hud_currentGeneration_pos.X = 10;
    gamesettings.hud_currentGeneration_pos.Y = 57;
    gamesettings.hud_aliveCells_pos.X = 10;
    gamesettings.hud_aliveCells_pos.Y = 58;
    gamesettings.hud_periodInSeconds_pos.X = 50;
    gamesettings.hud_periodInSeconds_pos.Y = 57;
    gamesettings.hud_iterationsPerSecond_pos.X = 50;
    gamesettings.hud_iterationsPerSecond_pos.Y = 58;
    gamesettings.hud_gridSize_pos.X = 50;
    gamesettings.hud_gridSize_pos.Y = 59;

    //setze Main Menu Buttons
    strcpy(mainMenu_Button[0].label, "start");
    mainMenu_Button[0].pos.X = 10;
    mainMenu_Button[0].pos.Y = 10;
    
    strcpy(mainMenu_Button[1].label, "settings");
    mainMenu_Button[1].pos.X = 10;
    mainMenu_Button[1].pos.Y = 12;

    strcpy(mainMenu_Button[2].label, "exit");
    mainMenu_Button[2].pos.X = 10;
    mainMenu_Button[2].pos.Y = 14;

    //setze Symbol
    gamesettings.symbolAlive = '#';
    gamesettings.symbolDead = '-';

    //setze base values
    gamesettings.iterationsPerSecond = 60;
    gamesettings.periodInSeconds = 10;
}

void run_ticks(int periodInSeconds, int ticksPerSecond){

    int tickCounter = 0;

    while(tickCounter < ticksPerSecond*periodInSeconds) {
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
    draw_hud();

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
    currentGeneration++;
}

void draw_hud(){
    
    set_cursor(gamesettings.hud_currentGeneration_pos.X, gamesettings.hud_currentGeneration_pos.Y);
    printf("generation: %d of %d", currentGeneration, gamesettings.iterationsPerSecond*gamesettings.periodInSeconds);

    set_cursor(gamesettings.hud_aliveCells_pos.X, gamesettings.hud_aliveCells_pos.Y);
    printf("cells alive: %d of %d", aliveCells, X_Size*Y_Size);
    
    set_cursor(gamesettings.hud_gridSize_pos.X, gamesettings.hud_gridSize_pos.Y);
    printf("grid size: %dx%d", X_Size, Y_Size);

    set_cursor(gamesettings.hud_periodInSeconds_pos.X, gamesettings.hud_periodInSeconds_pos.Y);
    printf("periodInSeconds: %ds", gamesettings.periodInSeconds);

    set_cursor(gamesettings.hud_iterationsPerSecond_pos.X, gamesettings.hud_iterationsPerSecond_pos.Y);
    printf("iterationsPerSecond: %d", gamesettings.iterationsPerSecond);

    set_cursor(0,0);
}

void *start_random_game(void *vargp){

    currentGeneration = 0;
    initialize_grid(grid);
    generate_random_grid(grid);
    run_ticks(gamesettings.periodInSeconds, gamesettings.iterationsPerSecond);
    return NULL; 
}

void main_menu(){

    int main_menu_cursor_position;

    main_menu_cursor_position = 0;

    int run = 1;
    while (run == 1)
    {
        
        draw_main_menu(mainMenu_Button, sizeof(mainMenu_Button));

        switch (main_menu_cursor_position)
        {

            case 0:
                draw_cursor(mainMenu_Button[0].pos);
                erase_cursor(mainMenu_Button[1].pos);
                erase_cursor(mainMenu_Button[2].pos);
                break;

            case 1:
                erase_cursor(mainMenu_Button[0].pos);
                draw_cursor(mainMenu_Button[1].pos);
                erase_cursor(mainMenu_Button[2].pos);
                break;
            
            case 2:
                erase_cursor(mainMenu_Button[0].pos);
                erase_cursor(mainMenu_Button[1].pos);
                draw_cursor(mainMenu_Button[2].pos);
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
                        // printf("u %d", main_menu_cursor_position);
                        main_menu_cursor_position--;
                    break;

                //DOWN
                case 80:
                        set_cursor(0,0);
                        // printf("d %d", main_menu_cursor_position);
                        main_menu_cursor_position++;
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

                    switch (main_menu_cursor_position)
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
        
        if(main_menu_cursor_position < 0){
            main_menu_cursor_position = 2;
        }

        if(main_menu_cursor_position > 2){
            main_menu_cursor_position = 0;
        }
        
    }
    

}


void generate_random_grid(struct cell grid_ptr[X_Size][Y_Size]){
    int x, y;

    for(y = 0; y < Y_Size; y++){
        for(x = 0; x < X_Size; x++){
            grid_ptr[x][y].alive = generate_random_int_msws()*rand() % 2;
        }
    }
}

