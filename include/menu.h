#ifndef MENU_H
#define MENU_H

#include <windows.h>
#include <cstdio>

int set_cursor(int x, int y);
void draw_main_menu();
void erase_menu_cursors();
COORD get_console_window_size(HANDLE hConsoleOutput);
void console_fullscreen();

#endif