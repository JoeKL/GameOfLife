/**
 * Game of Life
 * Erstellt am: 18.05.2020
 * Game of Life gemaeß Conway
 * 
 * Autoren: Gummiloch, TingelTangelTepe, Rueckener
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <pthread.h> 

#include "include/menu.h"
#include "include/game.h"
#include "include/buffer.h"

struct settings gamesettings;

struct cell **grid;
struct cell **gridcopy;
char *buffer;

struct menu_button mainMenu_Button[3];
struct menu_button settingsMenu_Button[5];

int aliveCells = 0;
int aliveCellsPrevGen = 0;
int currentGeneration = 0;
int iterationsSinceLastChange = 0;
int runtime = 0;

void run(int periodInSeconds, int ticksPerSecond);
void tick();

void draw_hud();
void settings_menu();
void main_menu();
void init_settings();

int main(){
    // set_fontsize(1);
    
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

    //setze Settings Menu Buttons
    strcpy(settingsMenu_Button[0].label, "currentGeneration");
    settingsMenu_Button[0].pos.X = 24;
    settingsMenu_Button[0].pos.Y = 10;
    
    strcpy(settingsMenu_Button[1].label, "aliveCells");
    settingsMenu_Button[1].pos.X = 24;
    settingsMenu_Button[1].pos.Y = 12;

    strcpy(settingsMenu_Button[2].label, "periodInSeconds");
    settingsMenu_Button[2].pos.X = 24;
    settingsMenu_Button[2].pos.Y = 14;

    strcpy(settingsMenu_Button[3].label, "iterationsPerSecond");
    settingsMenu_Button[3].pos.X = 24;
    settingsMenu_Button[3].pos.Y = 16;

    strcpy(settingsMenu_Button[4].label, "gridSize");
    settingsMenu_Button[4].pos.X = 24;
    settingsMenu_Button[4].pos.Y = 18;

    //setze Symbole
    gamesettings.symbolAlive = '#';
    gamesettings.symbolDead = '-';

    //setze base values
    gamesettings.iterationsPerSecond = 60;
    gamesettings.periodInSeconds = 3;

    //setze grid size
    gamesettings.gridsize.X = 117;
    gamesettings.gridsize.Y = 57;
}

void run(int periodInSeconds, int ticksPerSecond){

    runtime = time(0);

    while((time(0) - runtime) < periodInSeconds) {
        double cpu_time_used;
        clock_t start, end;

        start = clock();
        do {
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        } while(cpu_time_used < (double) 1/ticksPerSecond);
        
        tick();
        set_cursor(0, 57);
        printf("%i", time(0) - runtime);
    }
}

void tick(){

    aliveCells = 0;

    // kopiere das aktuelle grid
    copy_grid(gridcopy, grid, gamesettings.gridsize);

    int x;
    int y;

    for(y = 0; y < gamesettings.gridsize.Y; y++){
        for(x = 0; x < gamesettings.gridsize.X; x++){

            // zähle lebende Zellen beim durchgehen
            aliveCells += grid[x][y].alive;

            //abfragen ob es überhaupt lebende Nachbarn gibt
            if (gridcopy[x][y].livingNeighbors){
                
                //Eine tote Zelle mit genau drei lebenden Nachbarn wird in der Folgegeneration neu geboren.
                if(gridcopy[x][y].livingNeighbors == 3 && gridcopy[x][y].alive == 0){
                    grid[x][y].alive = 1;
                    add_neighborhood(grid[x][y]); 
                    update_buffer_at_coord(buffer, gamesettings.gridsize, gamesettings.symbolAlive, gamesettings.symbolDead, x, y, grid[x][y].alive);
                } else
                // Lebende Zellen mit weniger als zwei lebenden Nachbarn sterben in der Folgegeneration an Einsamkeit.
                if (gridcopy[x][y].alive == 1 && gridcopy[x][y].livingNeighbors < 2) {
                    grid[x][y].alive = 0;
                    sub_neighborhood(grid[x][y]); 
                    update_buffer_at_coord(buffer, gamesettings.gridsize, gamesettings.symbolAlive, gamesettings.symbolDead, x, y, grid[x][y].alive);
                } else
                // Lebende Zellen mit mehr als drei lebenden Nachbarn sterben in der Folgegeneration an  Überbevölkerung.
                if (gridcopy[x][y].alive == 1 && gridcopy[x][y].livingNeighbors > 3) {
                    grid[x][y].alive = 0;
                    sub_neighborhood(grid[x][y]); 
                    update_buffer_at_coord(buffer, gamesettings.gridsize, gamesettings.symbolAlive, gamesettings.symbolDead, x, y, grid[x][y].alive);

                }
            }
        }
    }

    // draw_hud();
    print_buffer(buffer);

    currentGeneration++;
    if (aliveCellsPrevGen == aliveCells) {
        iterationsSinceLastChange++;
    } else {
        iterationsSinceLastChange = 0;
    }
    

    aliveCellsPrevGen = aliveCells;
    
    // if (iterationsSinceLastChange >= 20){
    //     printf("asdada\n");
    //     system("pause");
    // }
}

void draw_hud(){
    
    set_cursor(gamesettings.hud_currentGeneration_pos.X, gamesettings.hud_currentGeneration_pos.Y);
    printf("generation: %d of %d", currentGeneration, gamesettings.iterationsPerSecond*gamesettings.periodInSeconds);

    set_cursor(gamesettings.hud_aliveCells_pos.X, gamesettings.hud_aliveCells_pos.Y);
    printf("cells alive: %d of %d  ", aliveCells, gamesettings.gridsize.X*gamesettings.gridsize.Y);
    
    set_cursor(gamesettings.hud_gridSize_pos.X, gamesettings.hud_gridSize_pos.Y);
    printf("grid size: %dx%d", gamesettings.gridsize);

    set_cursor(gamesettings.hud_periodInSeconds_pos.X, gamesettings.hud_periodInSeconds_pos.Y);
    printf("periodInSeconds: %ds", gamesettings.periodInSeconds);

    set_cursor(gamesettings.hud_iterationsPerSecond_pos.X, gamesettings.hud_iterationsPerSecond_pos.Y);
    printf("iterationsPerSecond: %d", gamesettings.iterationsPerSecond);

    set_cursor(0,0);
}

void *start_random_game(void *vargp){

    currentGeneration = 0;

    alloc_grid(&grid, gamesettings.gridsize);
    alloc_grid(&gridcopy, gamesettings.gridsize);
    alloc_buffer(&buffer, gamesettings.gridsize);
    initialize_buffer(buffer, gamesettings.gridsize, gamesettings.symbolAlive, gamesettings.symbolDead);

    initialize_empty_grid(grid, gamesettings.gridsize);
    // save_preset_from_grid(grid, gamesettings.gridsize);
    // load_preset_to_grid(grid, gamesettings.gridsize);
    generate_random_grid(grid, gamesettings.gridsize);

    define_neighborhood(grid, gamesettings.gridsize);
    define_neighborhood(gridcopy, gamesettings.gridsize);

    calc_all_neighbors(grid, gamesettings.gridsize);


    // print_grid(grid, gamesettings.gridsize, gamesettings);

    run(gamesettings.periodInSeconds, gamesettings.iterationsPerSecond);

    dealloc_grid(&grid, gamesettings.gridsize.X);
    dealloc_grid(&gridcopy, gamesettings.gridsize.X);
    dealloc_buffer(&buffer);

    system("cls");

    return NULL; 
}

void settings_menu(){
    int settings_menu_cursor_position = 0;

    int refresh_menu = 1;
    while (refresh_menu == 1)
    {
        draw_menu(settingsMenu_Button, sizeof(settingsMenu_Button)/sizeof(settingsMenu_Button[0]));
        set_menucursor(settingsMenu_Button, sizeof(settingsMenu_Button)/sizeof(settingsMenu_Button[0]), settings_menu_cursor_position);

        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            switch (_getch())
            {
                //UP
                case 72:
                        set_cursor(0,0);
                        settings_menu_cursor_position--;
                    break;

                //DOWN
                case 80:
                        set_cursor(0,0);
                        settings_menu_cursor_position++;
                    break;

            }
        } else {
            switch (ch)
            {
                //ENTER
                case 13:
                    switch (settings_menu_cursor_position)
                    {
                        case 0:
                            
                            break;
                        
                        case 1:
                            
                            break;
                            
                        case 2:
                            
                            break;
                    }

                    break;
                //ESC
                case 27:
                    refresh_menu = 0;
                    system("cls");
                    break;
            }
        }
        
        if(settings_menu_cursor_position < 0){
            settings_menu_cursor_position = 0;
        }

        if(settings_menu_cursor_position > sizeof(settingsMenu_Button)/sizeof(settingsMenu_Button[0]) - 1){
            settings_menu_cursor_position = sizeof(settingsMenu_Button)/sizeof(settingsMenu_Button[0]) - 1;
        }
        
    }

}

void main_menu(){

    int main_menu_cursor_position = 0;

    while (1)
    {
        
        draw_menu(mainMenu_Button, sizeof(mainMenu_Button)/sizeof(mainMenu_Button[0]));

        set_menucursor(mainMenu_Button, sizeof(mainMenu_Button)/sizeof(mainMenu_Button[0]), main_menu_cursor_position);

        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            switch (_getch())
            {
                //UP
                case 72:
                        set_cursor(0,0);
                        main_menu_cursor_position--;
                    break;

                //DOWN
                case 80:
                        set_cursor(0,0);
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
                            system("cls");
                            pthread_t thread_id;
                            pthread_create(&thread_id, NULL, start_random_game, NULL); 
                            pthread_join(thread_id, NULL);   
                            system("cls");                
                            break;
                        
                        case 1:
                            settings_menu();
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
            main_menu_cursor_position = 0;
        }

        if(main_menu_cursor_position > sizeof(mainMenu_Button)/sizeof(mainMenu_Button[0]) - 1){
            main_menu_cursor_position = sizeof(mainMenu_Button)/sizeof(mainMenu_Button[0]) - 1;
        }
 
    }
    
}
