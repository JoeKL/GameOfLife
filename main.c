/**
 * @file main.c
 * @author Lothar Gomoluch, Oliver Röckener und Niko Tepe
 * @brief 
 * @date 05.06.2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */

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

struct menu_button mainMenu_Button[4];
struct menu_button startMenu_Button[2];
struct menu_button settingsMenu_Button[6];

int aliveCells = 0;
int aliveCellsPrevGen = 0;
int currentGeneration = 0;
int iterationsSinceLastChange = 0;
int runtime_start = 0;

void run(int periodInSeconds, int ticksPerSecond);
void tick(int *end_game);

void draw_hud();
void settings_menu();
void main_menu();
void init_settings();

int main(){
    // set_fontsize(1);

    // SetConsoleTitle((LPCTSTR) "Game of Life");
    
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
    gamesettings.hud_aliveCells_pos.X = 50;
    gamesettings.hud_aliveCells_pos.Y = 57;
    gamesettings.hud_periodInSeconds_pos.X = 100;
    gamesettings.hud_periodInSeconds_pos.Y = 57;
    gamesettings.hud_iterationsPerSecond_pos.X = 150;
    gamesettings.hud_iterationsPerSecond_pos.Y = 57;
    gamesettings.hud_gridSize_pos.X = 200;
    gamesettings.hud_gridSize_pos.Y = 57;
    gamesettings.hud_shortcutInfo_pos.X = 0;
    gamesettings.hud_shortcutInfo_pos.Y = 59;

    //setze Main Menu Buttons
    strcpy(mainMenu_Button[0].label, "start");
    mainMenu_Button[0].pos.X = 110;
    mainMenu_Button[0].pos.Y = 20;
    
    strcpy(mainMenu_Button[1].label, "settings");
    mainMenu_Button[1].pos.X = 110;
    mainMenu_Button[1].pos.Y = 22;

    strcpy(mainMenu_Button[2].label, "rules");
    mainMenu_Button[2].pos.X = 110;
    mainMenu_Button[2].pos.Y = 24;

    strcpy(mainMenu_Button[3].label, "exit");
    mainMenu_Button[3].pos.X = 110;
    mainMenu_Button[3].pos.Y = 26;

    //setze start menu buttons
    strcpy(startMenu_Button[0].label, "random");
    startMenu_Button[0].pos.X = 124;
    startMenu_Button[0].pos.Y = 20;

    strcpy(startMenu_Button[1].label, "preset");
    startMenu_Button[1].pos.X = 124;
    startMenu_Button[1].pos.Y = 22;

    //setze Settings Menu Buttons
    strcpy(settingsMenu_Button[0].label, "gridSizeX");
    settingsMenu_Button[0].pos.X = 124;
    settingsMenu_Button[0].pos.Y = 20;

    strcpy(settingsMenu_Button[1].label, "gridSizeY");
    settingsMenu_Button[1].pos.X = 124;
    settingsMenu_Button[1].pos.Y = 22;

    strcpy(settingsMenu_Button[2].label, "periodInSeconds");
    settingsMenu_Button[2].pos.X = 124;
    settingsMenu_Button[2].pos.Y = 24;

    strcpy(settingsMenu_Button[3].label, "iterationsPerSecond");
    settingsMenu_Button[3].pos.X = 124;
    settingsMenu_Button[3].pos.Y = 26;

    strcpy(settingsMenu_Button[4].label, "symbolAlive");
    settingsMenu_Button[4].pos.X = 124;
    settingsMenu_Button[4].pos.Y = 28;
    
    strcpy(settingsMenu_Button[5].label, "symbolDead");
    settingsMenu_Button[5].pos.X = 124;
    settingsMenu_Button[5].pos.Y = 30;


    //setze Symbole
    gamesettings.symbolAlive = '#';
    gamesettings.symbolDead = '-';

    //setze base values
    gamesettings.iterationsPerSecond = 2000;
    gamesettings.periodInSeconds = 10;

    //setze grid size
    gamesettings.gridsize.X = 117;
    gamesettings.gridsize.Y = 57;
}

void run(int periodInSeconds, int ticksPerSecond){

    runtime_start = time(0);

    // Variable wird in der Funktion tick() auf 1 gesetzt um das aktuelle Spiel zu beenden
    int end_game = 0;

    while(get_time_since_start_value(runtime_start) < periodInSeconds) {
        double cpu_time_used;
        clock_t start, end;

        start = clock();
        do {
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        } while(cpu_time_used < (double) 1/ticksPerSecond);
        
        tick(&end_game);

        // wenn end_game == 1 dann breche aus der While-Schleife
        if(end_game) {
            break;
        }
    }
}

void tick(int *end_game){

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
                    revive_buffer_at_coord(buffer, gamesettings, x, y);
                } else
                // Lebende Zellen mit weniger als zwei lebenden Nachbarn sterben in der Folgegeneration an Einsamkeit.
                if (gridcopy[x][y].alive == 1 && gridcopy[x][y].livingNeighbors < 2) {
                    grid[x][y].alive = 0;
                    sub_neighborhood(grid[x][y]); 
                    kill_buffer_at_coord(buffer, gamesettings, x, y);
                } else
                // Lebende Zellen mit mehr als drei lebenden Nachbarn sterben in der Folgegeneration an  Überbevölkerung.
                if (gridcopy[x][y].alive == 1 && gridcopy[x][y].livingNeighbors > 3) {
                    grid[x][y].alive = 0;
                    sub_neighborhood(grid[x][y]); 
                    kill_buffer_at_coord(buffer, gamesettings, x, y);
                }
            }
        }
    }

    draw_hud();
    print_buffer(buffer);

    // +++++++ KeyStonks +++++++
        //Keystrokes in neuem Thread abfangen?

    if (kbhit()){
        int ch = _getch();
        
        if(ch == 27){

            //if *ESC* then end game
            *end_game = 1;

        } else if(ch == 32){

            //if *space* then pause game
            
        } else if(ch == 115){

            //if *s* then save game as preset
            save_preset_from_grid(grid, gamesettings.gridsize);
            set_cursor(0, 58);
            printf("generation %i has been saved as 'preset.txt'", currentGeneration);

        }
        
    }

    // ------- KeyStonks -------

    // ++++++++ EndCondition ++++++++
    currentGeneration++;
    if (aliveCellsPrevGen == aliveCells) {
        iterationsSinceLastChange++;
    } else {
        iterationsSinceLastChange = 0;
    }

    aliveCellsPrevGen = aliveCells;
    
    if (iterationsSinceLastChange >= 20){
       *end_game = 1;
    }
    // -------- EndCondition --------
}

void draw_hud(){
    set_cursor(gamesettings.hud_currentGeneration_pos.X, gamesettings.hud_currentGeneration_pos.Y);
    printf("generation: %d", currentGeneration);

    set_cursor(gamesettings.hud_aliveCells_pos.X, gamesettings.hud_aliveCells_pos.Y);
    printf("cells alive: %d of %d  ", aliveCells, gamesettings.gridsize.X*gamesettings.gridsize.Y);
    
    set_cursor(gamesettings.hud_gridSize_pos.X, gamesettings.hud_gridSize_pos.Y);
    printf("grid size: %dx%d", gamesettings.gridsize.X, gamesettings.gridsize.Y);

    set_cursor(gamesettings.hud_periodInSeconds_pos.X, gamesettings.hud_periodInSeconds_pos.Y);
    printf("time: %ds of %ds", get_time_since_start_value(runtime_start), gamesettings.periodInSeconds);

    set_cursor(gamesettings.hud_iterationsPerSecond_pos.X, gamesettings.hud_iterationsPerSecond_pos.Y);
    printf("iterationsPerSecond: %d", gamesettings.iterationsPerSecond);

    set_cursor(gamesettings.hud_shortcutInfo_pos.X, gamesettings.hud_shortcutInfo_pos.Y);
    printf("|\t'ESC' to end game\t|\t'SPACE' to pause game\t|\t'S' to save currrent state as preset\t|");
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

    return NULL; 
}

void *start_preset_game(void *vargp){

    currentGeneration = 0;

    alloc_grid(&grid, gamesettings.gridsize);
    alloc_grid(&gridcopy, gamesettings.gridsize);
    alloc_buffer(&buffer, gamesettings.gridsize);
    initialize_buffer(buffer, gamesettings.gridsize, gamesettings.symbolAlive, gamesettings.symbolDead);

    initialize_empty_grid(grid, gamesettings.gridsize);
    
    load_preset_to_grid(grid, gamesettings.gridsize);
    define_neighborhood(grid, gamesettings.gridsize);
    define_neighborhood(gridcopy, gamesettings.gridsize);
    calc_all_neighbors(grid, gamesettings.gridsize);

    run(gamesettings.periodInSeconds, gamesettings.iterationsPerSecond);
    
    dealloc_grid(&grid, gamesettings.gridsize.X);
    dealloc_grid(&gridcopy, gamesettings.gridsize.X);
    dealloc_buffer(&buffer);

    return NULL; 
}

void settings_menu(){
    int settings_menu_cursor_position = 0;

    int refresh_menu = 1;
    while (refresh_menu == 1)
    {
        draw_menu(settingsMenu_Button, sizeof(settingsMenu_Button)/sizeof(settingsMenu_Button[0]));
        draw_menu_values(settingsMenu_Button, sizeof(settingsMenu_Button)/sizeof(settingsMenu_Button[0]), gamesettings);
        
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
                            //%hu == short unsigned
                            edit_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
                            break;
                        
                        case 1:
                            //%hu == short unsigned
                            edit_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);    
                            break;
                            
                        case 2:
                            edit_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
                            break;

                        case 3:
                            edit_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
                            break;

                        case 4:
                            edit_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
                            break;

                        case 5:
                            edit_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
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

void start_menu(){

    int start_menu_cursor_position = 0;

    int refresh_menu = 1;
    while (refresh_menu == 1)
    {
        draw_menu(startMenu_Button, sizeof(startMenu_Button)/sizeof(startMenu_Button[0]));

        set_menucursor(startMenu_Button, sizeof(startMenu_Button)/sizeof(startMenu_Button[0]), start_menu_cursor_position);

        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            switch (_getch())
            {
                //UP
                case 72:
                        start_menu_cursor_position--;
                    break;

                //DOWN
                case 80:
                        start_menu_cursor_position++;
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
                    
                    switch (start_menu_cursor_position)
                    {
                        case 0:
                            system("cls");
                            pthread_t thread_0;
                            pthread_create(&thread_0, NULL, start_random_game, NULL); 
                            pthread_join(thread_0, NULL);   
                            system("cls");  

                            refresh_menu = 0;
                            break;
                        
                        case 1:
                            system("cls");
                            pthread_t thread_1;
                            pthread_create(&thread_1, NULL, start_preset_game, NULL); 
                            pthread_join(thread_1, NULL);   
                            system("cls");  

                            refresh_menu = 0;
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

        
        if(start_menu_cursor_position < 0){
            start_menu_cursor_position = 0;
        }

        if(start_menu_cursor_position > sizeof(startMenu_Button)/sizeof(startMenu_Button[0]) - 1){
            start_menu_cursor_position = sizeof(startMenu_Button)/sizeof(startMenu_Button[0]) - 1;
        }

    }

}

void main_menu(){

    int main_menu_cursor_position = 0;
   
    while (1)
    {
        print_logo(86, 10);
        draw_menu(mainMenu_Button, sizeof(mainMenu_Button)/sizeof(mainMenu_Button[0]));

        set_menucursor(mainMenu_Button, sizeof(mainMenu_Button)/sizeof(mainMenu_Button[0]), main_menu_cursor_position);

        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            switch (_getch())
            {
                //UP
                case 72:
                        main_menu_cursor_position--;
                    break;

                //DOWN
                case 80:
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
                            start_menu();
                            break;
                        
                        case 1:
                            settings_menu();
                            break;
                            
                        case 2:
                            break;
                            
                        case 3:
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
