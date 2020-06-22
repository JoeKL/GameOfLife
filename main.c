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

#include "include/menu.h"
#include "include/game.h"
#include "include/buffer.h"

struct settings gamesettings;
struct rule gamerules;

struct cell **grid;
struct cell **gridcopy;
char *buffer;

struct menu_button mainMenu_Button[4];
struct menu_button startMenu_Button[2];
struct menu_button settingsMenu_Button[6];
struct menu_button ruleMenu_Button[3];

int aliveCells = 0;
int aliveCellsPrevGen = 0;
int currentGeneration = 0;
int iterationsSinceLastChange = 0;
int runtime_start = 0;

void run(int generationsToCalc, int ticksPerSecond);
void tick(int *end_game, int *pause_game);

void settings_menu();
void main_menu();
void init_settings();

int main(){

    SetConsoleTitle((LPCTSTR) "Game of Life");

    set_console_fullscreen();
    init_settings();
    main_menu();

    return 0;
}

/**
 * @brief initialisiert Grundwerte für die Programmausführung
 *
 */
void init_settings(){
    //setze den rand() seed auf Sekunden seit Epoche
    srand(time(NULL));

    //setze hud koordinaten
    gamesettings.hud_currentGeneration_pos.X = 10;
    gamesettings.hud_currentGeneration_pos.Y = 57;
    gamesettings.hud_aliveCells_pos.X = 50;
    gamesettings.hud_aliveCells_pos.Y = 57;
    gamesettings.hud_generationsToCalc_pos.X = 100;
    gamesettings.hud_generationsToCalc_pos.Y = 57;
    gamesettings.hud_iterationsPerSecond_pos.X = 150;
    gamesettings.hud_iterationsPerSecond_pos.Y = 57;
    gamesettings.hud_gridSize_pos.X = 200;
    gamesettings.hud_gridSize_pos.Y = 57;
    gamesettings.hud_shortcutInfo_pos.X = 25;
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

    strcpy(settingsMenu_Button[2].label, "generationsToCalc");
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

    //setze Rule Menu Buttons
    strcpy(ruleMenu_Button[0].label, "rebornRule");
    ruleMenu_Button[0].pos.X = 124;
    ruleMenu_Button[0].pos.Y = 20;

    strcpy(ruleMenu_Button[1].label, "lonelinessRule");
    ruleMenu_Button[1].pos.X = 124;
    ruleMenu_Button[1].pos.Y = 22;

    strcpy(ruleMenu_Button[2].label, "overpopulationRule");
    ruleMenu_Button[2].pos.X = 124;
    ruleMenu_Button[2].pos.Y = 24;

    //setze Symbole
    gamesettings.symbolAlive = '#';
    gamesettings.symbolDead = '-';

    //setze base values
    gamesettings.iterationsPerSecond = 30;
    gamesettings.generationsToCalc = 100;

    //setze grid size
    gamesettings.gridsize.X = 117;
    gamesettings.gridsize.Y = 57;

    //setze gamerules
    gamerules.rebornRule = 3;
    gamerules.lonelinessRule = 2;
    gamerules.overpopulationRule = 3;
}

/**
 * @brief
 *
 * @param generationsToCalc
 * @param ticksPerSecond
 */
void run(int generationsToCalc, int ticksPerSecond){

    int calculated_ticks = 0;

    // Variable wird in der Funktion tick() auf 1 gesetzt um das aktuelle Spiel zu beenden
    int end_game = 0;
    int pause_game = 0;

    aliveCells = 0;
    aliveCellsPrevGen = 0;
    currentGeneration = 0;
    iterationsSinceLastChange = 0;

    //Simulation läuft solange bis solange bis periodInTicks erreicht wurde
    while(calculated_ticks <= generationsToCalc) {

        //Schlafe in jedem Tick für 1000ms/ticksPerSecond -> 1000/30 = 33.3ms
        Sleep((DWORD) 1000/ticksPerSecond);

        if(!pause_game){
            tick(&end_game, &pause_game);
            calculated_ticks++;
        }

        draw_hud(gamesettings, aliveCells, currentGeneration, generationsToCalc);

        // +++++++ KeyStonks +++++++
        if (kbhit()){
            int ch = _getch();

            if(ch == 27){

                //wenn *ESC* dann end game
                end_game = 1;

            } else if(ch == 32){

                //wenn *space* dann pause game
                if(pause_game == 0){
                    pause_game = 1;
                } else {
                    pause_game = 0;
                }

            } else if(ch == 115){

                //wenn *s* dann save gamestate in preset
                save_preset_from_grid(grid, gamesettings.gridsize);
                set_cursor(0, 58);
                printf("generation %i has been saved as 'preset.txt'", currentGeneration);

            } else if(ch == 13 && pause_game == 1){
                tick(&end_game, &pause_game);
                calculated_ticks++;
            }
        }

        // wenn in tick() end_game > 0 gesetzt wurde, dann breche aus der While-Schleife
        if(end_game) {
            break;
        }
    }
}

/**
 * @brief Diese Funktion berechnet die nächste Iteration des GOL
 *
 * @param end_game
 * @param pause_game
 */
void tick(int *end_game, int *pause_game){

    aliveCells = 0;

    // kopiere das aktuelle grid
    copy_grid(gridcopy, grid, gamesettings.gridsize);

    for(int y = 0; y < gamesettings.gridsize.Y; y++){
        for(int x = 0; x < gamesettings.gridsize.X; x++){

            // zähle lebende Zellen beim durchgehen
            aliveCells += grid[x][y].alive;

            //abfragen ob es überhaupt lebende Nachbarn gibt
            if (gridcopy[x][y].livingNeighbors){

                //Eine tote Zelle mit genau drei lebenden Nachbarn wird in der Folgegeneration neu geboren.
                if(gridcopy[x][y].livingNeighbors == gamerules.rebornRule && gridcopy[x][y].alive == 0){
                    grid[x][y].alive = 1;
                    add_neighborhood(grid[x][y]);
                    revive_buffer_at_coord(buffer, gamesettings, x, y);
                } else
                // Lebende Zellen mit weniger als zwei lebenden Nachbarn sterben in der Folgegeneration an Einsamkeit.
                if (gridcopy[x][y].livingNeighbors < gamerules.lonelinessRule && gridcopy[x][y].alive == 1) {
                    grid[x][y].alive = 0;
                    sub_neighborhood(grid[x][y]);
                    kill_buffer_at_coord(buffer, gamesettings, x, y);
                } else
                // Lebende Zellen mit mehr als drei lebenden Nachbarn sterben in der Folgegeneration an  Überbevölkerung.
                if (gridcopy[x][y].livingNeighbors > gamerules.overpopulationRule && gridcopy[x][y].alive == 1) {
                    grid[x][y].alive = 0;
                    sub_neighborhood(grid[x][y]);
                    kill_buffer_at_coord(buffer, gamesettings, x, y);
                }
            }
        }
    }

    print_buffer(buffer);

    // ++++++++ EndCondition ++++++++
    currentGeneration++;

    if (aliveCellsPrevGen == aliveCells) {
        iterationsSinceLastChange++;
    } else {
        iterationsSinceLastChange = 0;
    }

    aliveCellsPrevGen = aliveCells;

    //spiel wird anch 20 iterationen mit der gleichen anzahl lebender Zellen abgebrochen
    if (iterationsSinceLastChange >= 20){
       *end_game = 1;
    }
    // -------- EndCondition --------
}

/**
 * @brief diese Funktion startet ein Spiel
 *
 * @param is_random legt fest ob das Spiel zufällig generiert sein soll
 */
void start_game(int is_random){

    alloc_grid(&grid, gamesettings.gridsize);
    alloc_grid(&gridcopy, gamesettings.gridsize);
    alloc_buffer(&buffer, gamesettings.gridsize);

    initialize_buffer(
                buffer,
                gamesettings.gridsize,
                gamesettings.symbolAlive,
                gamesettings.symbolDead
    );

    initialize_empty_grid(grid, gamesettings.gridsize);

    if(is_random == 1){
        generate_random_grid(grid, gamesettings.gridsize);
    } else {
        load_preset_to_grid(grid, gamesettings.gridsize);
    }

    define_neighborhood(grid, gamesettings.gridsize);
    define_neighborhood(gridcopy, gamesettings.gridsize);

    calc_all_neighbors(grid, gamesettings.gridsize);

    run(gamesettings.generationsToCalc, gamesettings.iterationsPerSecond);

    dealloc_grid(&grid, gamesettings.gridsize.X);
    dealloc_grid(&gridcopy, gamesettings.gridsize.X);
    dealloc_buffer(&buffer);
}

/**
 * @brief generiert ein Untermenü: Settingsmenu
 *
 */
void settings_menu(){
    int settings_menu_cursor_position = 0;

    int refresh_menu = 1;
    while (refresh_menu == 1)
    {
        draw_menu(
                settingsMenu_Button,
                sizeof(settingsMenu_Button)/sizeof(settingsMenu_Button[0])
        );

        draw_settings_menu_values(
                settingsMenu_Button,
                sizeof(settingsMenu_Button)/sizeof(settingsMenu_Button[0]),
                gamesettings
        );

        set_menucursor(
                settingsMenu_Button,
                sizeof(settingsMenu_Button)/sizeof(settingsMenu_Button[0]),
                settings_menu_cursor_position
        );

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
                            edit_setting_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
                            break;

                        case 1:
                            //%hu == short unsigned
                            edit_setting_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
                            break;

                        case 2:
                            edit_setting_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
                            break;

                        case 3:
                            edit_setting_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
                            break;

                        case 4:
                            edit_setting_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
                            break;

                        case 5:
                            edit_setting_value(&gamesettings, settingsMenu_Button, settings_menu_cursor_position);
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

/**
 * @brief generiert ein Untermenü: startmenu
 *
 */
void start_menu(){

    int start_menu_cursor_position = 0;

    int refresh_menu = 1;
    while (refresh_menu == 1)
    {
        draw_menu(
                startMenu_Button,
                sizeof(startMenu_Button)/sizeof(startMenu_Button[0])
        );

        set_menucursor(
                startMenu_Button,
                sizeof(startMenu_Button)/sizeof(startMenu_Button[0]),
                start_menu_cursor_position
        );

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
                            start_game(1);
                            system("cls");

                            refresh_menu = 0;
                            break;

                        case 1:
                            system("cls");
                            start_game(0);
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

/**
 * @brief generiert ein Untermenü: Rulemenu
 * 
 */
void rule_menu(){
    int rule_menu_cursor_position = 0;

    int refresh_menu = 1;
    while (refresh_menu == 1)
    {
        draw_menu(
                ruleMenu_Button,
                sizeof(ruleMenu_Button)/sizeof(ruleMenu_Button[0])
        );

        draw_rule_menu_values(
                ruleMenu_Button,
                sizeof(ruleMenu_Button)/sizeof(ruleMenu_Button[0]),
                gamerules
        );

        set_menucursor(
                ruleMenu_Button,
                sizeof(ruleMenu_Button)/sizeof(ruleMenu_Button[0]),
                rule_menu_cursor_position
        );

        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            switch (_getch())
            {
                //UP
                case 72:
                        set_cursor(0,0);
                        rule_menu_cursor_position--;
                    break;

                //DOWN
                case 80:
                        set_cursor(0,0);
                        rule_menu_cursor_position++;
                    break;

            }
        } else {
            switch (ch)
            {
                //ENTER
                case 13:
                    switch (rule_menu_cursor_position)
                    {
                        case 0:
                            edit_rule_value(&gamerules, ruleMenu_Button, rule_menu_cursor_position);
                            break;

                        case 1:
                            edit_rule_value(&gamerules, ruleMenu_Button, rule_menu_cursor_position);
                            break;

                        case 2:
                            edit_rule_value(&gamerules, ruleMenu_Button, rule_menu_cursor_position);
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

        if(rule_menu_cursor_position < 0){
            rule_menu_cursor_position = 0;
        }

        if(rule_menu_cursor_position > sizeof(ruleMenu_Button)/sizeof(ruleMenu_Button[0]) - 1){
            rule_menu_cursor_position = sizeof(ruleMenu_Button)/sizeof(ruleMenu_Button[0]) - 1;
        }

    }

}

/**
 * @brief generiert das Hauptmenü
 *
 */
void main_menu(){

    int main_menu_cursor_position = 0;

    while (1)
    {
        print_logo(86, 10);

        draw_menu(
                mainMenu_Button,
                sizeof(mainMenu_Button)/sizeof(mainMenu_Button[0])
        );

        set_menucursor(
                mainMenu_Button,
                sizeof(mainMenu_Button)/sizeof(mainMenu_Button[0]),
                main_menu_cursor_position
        );

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
                            rule_menu();
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
