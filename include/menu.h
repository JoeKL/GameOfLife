#ifndef MENU_H
#define MENU_H

#include <windows.h>
#include <cstdio>
#include <string.h>

struct menu_button
{
    char label[20];
    COORD pos;
};

int set_cursor(int x, int y);
void draw_main_menu(struct menu_button mainMenu_Button[3], int array_length);

void erase_menu_cursors();
void draw_cursor(COORD cords);
void erase_cursor(COORD cords);

COORD get_console_window_size(HANDLE hConsoleOutput);
void console_fullscreen();

#endif