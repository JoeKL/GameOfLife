/**
 * @file menu.h
 * @author Lothar Gomoluch, Oliver Röckener und Niko Tepe
 * @brief
 * @date 05.06.2020
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef MENU_H
#define MENU_H

#include <windows.h>
#include <string.h>
#include "game.h"

struct menu_button
{
    char label[20];
    COORD pos;
};

void print_logo(int x, int y);

void set_cursor(int x, int y);
void set_fontsize(int size);

void draw_menu(struct menu_button Menu_Button[3], int array_length);

void draw_settings_menu_values(
                struct menu_button Menu_Button[3],
                int array_length,
                struct settings gamesettings
);

void draw_rule_menu_values(
                struct menu_button Menu_Button[3],
                int array_length,
                struct rule gamerules
);

int edit_setting_value(
        struct settings *gamesettings,
        struct menu_button Menu_Button[5],
        int cursor_pos
);

int edit_rule_value(
        struct rule *gamerules,
        struct menu_button Menu_Button[5],
        int cursor_pos
);

void set_value_cursor(struct menu_button Menu_Button[5], int cursor_pos);

void set_menucursor(struct menu_button Menu_Button[3], int array_length, int position);

void erase_menu_cursors();
void draw_cursor(COORD cords);
void erase_cursor(COORD cords);

COORD get_console_window_size(HANDLE hConsoleOutput);
void set_console_fullscreen();

#endif
