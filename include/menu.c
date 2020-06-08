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
void console_fullscreen(){
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
}