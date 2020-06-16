/**
 * @file menu.c
 * @author Lothar Gomoluch, Oliver Röckener und Niko Tepe
 * @brief 
 * @date 05.06.2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "menu.h"

/**
 * @brief Set the cursor object
 * 
 * @param x 
 * @param y 
 */
void set_cursor(int x, int y){
    // https://docs.microsoft.com/en-us/windows/console/console-functions
    COORD koordinaten;
    koordinaten.X= x;
    koordinaten.Y= y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), koordinaten);
}

/**
 * @brief Get the console window size object
 * 
 * @param hConsoleOutput 
 * @return COORD 
 */
COORD get_console_window_size(HANDLE hConsoleOutput){
    COORD size;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    //x = 237
    //y = 60
    return size;
}

/**
 * @brief Set the fontsize object
 * 
 * @param size 
 */
void set_fontsize(int size){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.dwFontSize.Y = size;      
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

/**
 * @brief draws a menu
 * 
 * @param Menu_Button 
 * @param array_length 
 */
void draw_menu(struct menu_button Menu_Button[5], int array_length){

    for (int i = 0; i < array_length; i++){

        set_cursor(Menu_Button[i].pos.X,Menu_Button[i].pos.Y);

        printf("%s", Menu_Button[i].label);
    }

    set_cursor(0,0);
}

/**
 * @brief 
 * 
 * @param Menu_Button 
 * @param array_length 
 */
void draw_settings_menu_values(struct menu_button Menu_Button[5], int array_length, struct settings gamesettings){
 
    for (int i = 0; i < array_length; i++){

        set_cursor(Menu_Button[i].pos.X + sizeof(Menu_Button[i].label), Menu_Button[i].pos.Y);


        switch (i)
        {
            case 0:
                printf(" = %d            ", gamesettings.gridsize.X);
                break;
            
            case 1:
                printf(" = %d            ", gamesettings.gridsize.Y);
                break;

            case 2:
                printf(" = %d            ", gamesettings.generationsToCalc);
                break;

            case 3:
                printf(" = %d            ", gamesettings.iterationsPerSecond);
                break;            

            case 4:
                printf(" = '%c'          ", gamesettings.symbolAlive);
                break;

            case 5:
                printf(" = '%c'          ", gamesettings.symbolDead);
                break;
        }
    }

}

/**
 * @brief 
 * 
 * @param Menu_Button 
 * @param array_length 
 */
void draw_rule_menu_values(struct menu_button Menu_Button[5], int array_length, struct rule gamerules){
 
    for (int i = 0; i < array_length; i++){

        set_cursor(Menu_Button[i].pos.X + sizeof(Menu_Button[i].label), Menu_Button[i].pos.Y);


        switch (i)
        {
            case 0:
                printf(" = %d            ", gamerules.rebornRule);
                break;
            
            case 1:
                printf(" = %d            ", gamerules.lonelinessRule);
                break;

            case 2:
                printf(" = %d            ", gamerules.overpopulationRule);
                break;
        }
    }

}

/**
 * @brief 
 * 
 * @param cursor_pos 
 * @return int 
 */
int edit_setting_value(struct settings *gamesettings, struct menu_button Menu_Button[5], int cursor_pos){

        switch (cursor_pos)
        {
            case 0:    
                set_value_cursor(Menu_Button, cursor_pos);
                //%hu == short unsigned
                scanf("%hu", &gamesettings->gridsize.X);
                fflush(stdin);

                if(gamesettings->gridsize.X < 5){
                    gamesettings->gridsize.X = 5;
                }
                
                if(gamesettings->gridsize.X > 117){
                    gamesettings->gridsize.X = 117;
                }

                break;
            
            case 1:
                set_value_cursor(Menu_Button, cursor_pos);
                //%hu == short unsigned
                scanf("%hu", &gamesettings->gridsize.Y);
                fflush(stdin);

                
                if(gamesettings->gridsize.Y < 5){
                    gamesettings->gridsize.Y = 5;
                }
                
                if(gamesettings->gridsize.Y > 57){
                    gamesettings->gridsize.Y = 57;
                }

                break;

            case 2:
                set_value_cursor(Menu_Button, cursor_pos);
                scanf("%i", &gamesettings->generationsToCalc);
                fflush(stdin);

                if(gamesettings->generationsToCalc < 10){
                    gamesettings->generationsToCalc = 10;
                }
                
                if(gamesettings->generationsToCalc > 2147483647){
                    gamesettings->generationsToCalc = 2147483647;
                }


                break;

            case 3:
                set_value_cursor(Menu_Button, cursor_pos);
                scanf("%i", &gamesettings->iterationsPerSecond);
                fflush(stdin);

                if(gamesettings->generationsToCalc < 1){
                    gamesettings->generationsToCalc = 1;
                }
                
                if(gamesettings->generationsToCalc > 2147483647){
                    gamesettings->generationsToCalc = 2147483647;
                }

                break;            

            case 4:
                set_value_cursor(Menu_Button, cursor_pos);
                if(!scanf("%c", &gamesettings->symbolAlive)){
                    gamesettings->symbolAlive = '#';
                }

                if(gamesettings->symbolAlive == '\n'){
                    gamesettings->symbolAlive = '#';
                }

                fflush(stdin);
                break;

            case 5:
                set_value_cursor(Menu_Button, cursor_pos);
                if(!scanf("%c", &gamesettings->symbolDead)){
                    gamesettings->symbolDead = '-';
                }
                fflush(stdin);

                if(gamesettings->symbolDead == '\n'){
                    gamesettings->symbolDead = '-';
                }

                break;
        }


    return 0;
}

int edit_rule_value(struct rule *gamerules, struct menu_button Menu_Button[3], int cursor_pos){

        switch (cursor_pos)
        {
            case 0:    
                set_value_cursor(Menu_Button, cursor_pos);
                //%hu == short unsigned
                scanf("%hu", &gamerules->rebornRule);
                fflush(stdin);
                break;
            
            case 1:
                set_value_cursor(Menu_Button, cursor_pos);
                //%hu == short unsigned
                scanf("%hu", &gamerules->lonelinessRule);
                fflush(stdin);
                break;

            case 2:
                set_value_cursor(Menu_Button, cursor_pos);
                scanf("%i", &gamerules->overpopulationRule);
                fflush(stdin);
                break;
        }


    return 0;
}

void set_value_cursor(struct menu_button Menu_Button[5], int cursor_pos){
    set_cursor(Menu_Button[cursor_pos].pos.X + sizeof(Menu_Button[cursor_pos].label) + 2, Menu_Button[cursor_pos].pos.Y);
    printf("          ");
    set_cursor(Menu_Button[cursor_pos].pos.X + sizeof(Menu_Button[cursor_pos].label) + 3, Menu_Button[cursor_pos].pos.Y);
}


/**
 * @brief draws the cursor on a specific coord
 * 
 * @param cords
 */
void draw_cursor(COORD cords){
    char cursor[] = "-->";
    set_cursor( cords.X - sizeof(cursor) + 1, cords.Y);
    printf("%s", cursor);

    set_cursor(0,0);
}

/**
 * @brief eareses the cursor on a specific coord
 * 
 * @param cords 
 */
void erase_cursor(COORD cords){
    char cursor_eraser[] = "   ";
    set_cursor( cords.X - sizeof(cursor_eraser) + 1, cords.Y);
    printf("%s", cursor_eraser);
    set_cursor(0,0);
}

/**
 * @brief sets the console to fullscreen (same as alt+enter)
 * 
 */
void set_console_fullscreen(){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    // Consolewindow -> Alt+Enter
    SetConsoleDisplayMode(consoleHandle,CONSOLE_FULLSCREEN_MODE,0);
    COORD consoleSize = get_console_window_size(consoleHandle);
    SetConsoleScreenBufferSize(consoleHandle, consoleSize);     
}

/**
 * @brief Set the menucursor to specific menu point
 * 
 * @param Menu_Button 
 * @param array_length 
 * @param position 
 */
void set_menucursor(struct menu_button Menu_Button[3], int array_length, int position){

    for (int i = 0; i < array_length; i++)
    {
        if (i == position){
            draw_cursor(Menu_Button[i].pos);
        } else {
            erase_cursor(Menu_Button[i].pos);
        }
    }
}

/**
 * @brief gibt das Spiel-Logo an Position x,y aus
 * 
 * @param x 
 * @param y 
 */
void print_logo(int x, int y){

    set_cursor(x, y);
    printf("   ____                               __   _     _  __  ");

    set_cursor(x, y+1);
    printf("  / ___| __ _ _ __ ___   ___    ___  / _| | |   (_)/ _| ___ ");

    set_cursor(x, y+2);
    printf(" | |  _ / _` | '_ ` _ \\ / _ \\  / _ \\| |_  | |   | | |_ / _ \\ ");

    set_cursor(x, y+3);
    printf(" | |_| | (_| | | | | | |  __/ | (_) |  _| | |___| |  _|  __/");

    set_cursor(x, y+4);
    printf("  \\____|\\__,_|_| |_| |_|\\___|  \\___/|_|   |_____|_|_|  \\___|");

    set_cursor(x, y+5);
    printf("  by Lothar G., Oliver R. and Niko T.");
}