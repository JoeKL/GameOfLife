#include "menu.h"

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

void draw_main_menu(){
    set_cursor(10,10);
    printf("Start");
    set_cursor(10,20);
    printf("Settings");
    set_cursor(10,30);
    printf("Exit");
    set_cursor(0,0);
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

void console_fullscreen(){
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleDisplayMode(consoleHandle,CONSOLE_FULLSCREEN_MODE,0);
    COORD consoleSize = get_console_window_size(consoleHandle);
    SetConsoleScreenBufferSize(consoleHandle, consoleSize);     
}
