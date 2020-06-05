#ifndef MENU_H
#define MENU_H

#include <windows.h>
#include <cstdio>
#include <string.h>

typedef struct menu_button
{
    char label[20];
    COORD pos;
};

void set_cursor(int x, int y);
void set_fontsize(int size);
void draw_menu(struct menu_button Menu_Button[3], int array_length);
void set_menucursor(struct menu_button Menu_Button[3], int array_length, int position);

void erase_menu_cursors();
void draw_cursor(COORD cords);
void erase_cursor(COORD cords);

COORD get_console_window_size(HANDLE hConsoleOutput);
void console_fullscreen();

#endif