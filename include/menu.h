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

/**
 * @brief gibt das Spiel-Logo an Position x,y aus
 * 
 * @param x X-Position im Consolenbuffer
 * @param y Y-Position im Consolenbuffer
 */
void print_logo(int x, int y);

/**
 * @brief Setzt den Consolecursor an eine Position im Consolenbuffer
 * 
 * @param x X-Position im Consolenbuffer
 * @param y Y-Position im Consolenbuffer
 */
void set_cursor(int x, int y);

/**
 * @brief Setzt die Consolenfontsize
 * 
 * @param size Schriftgröße
 */
void set_fontsize(int size);

/**
 * @brief Zeichnet ein Menu anhand von Buttons
 * 
 * @param Menu_Button Array mit Menubuttons
 * @param array_length Arraylänge
 */
void draw_menu(struct menu_button Menu_Button[3], int array_length);

/**
 * @brief Zeichnet die Werte im Settingsmenü
 * 
 * @param Menu_Button Array mit Menubuttons
 * @param array_length Arraylänge
 * @param gamesettings Gamesettings Struct
 */
void draw_settings_menu_values(
                struct menu_button Menu_Button[3],
                int array_length,
                struct settings gamesettings
);

/**
 * @brief Zeichnet die Werte im Rulemenü
 * 
 * @param Menu_Button Array mit Menubuttons
 * @param array_length Arraylänge
 * @param gamerules Gamerule Struct
 */
void draw_rule_menu_values(
                struct menu_button Menu_Button[3],
                int array_length,
                struct rule gamerules
);

/**
 * @brief Initialisiert eine Scanf-Sequenz im Settingesmenü um einen Wert anzupassen
 * 
 * @param gamesettings Gamesettings Struct
 * @param Menu_Button Array mit Menubuttons
 * @param cursor_pos Position des Cursors
 */
void edit_setting_value(
        struct settings *gamesettings,
        struct menu_button Menu_Button[5],
        int cursor_pos
);

/**
 * @brief Initialisiert eine Scanf-Sequenz im Rulesmenü um einen Wert anzupassen
 * 
 * @param gamerules Gamesrules Struct
 * @param Menu_Button Array mit Menubuttons
 * @param cursor_pos Position des Cursors
 */
void edit_rule_value(
        struct rule *gamerules,
        struct menu_button Menu_Button[5],
        int cursor_pos
);

/**
 * @brief Bewegt den Cursor im Settingsmenü an eine übergebene Stelle
 * 
 * @param Menu_Button Array mit Menubuttons
 * @param cursor_pos Position des Cursors
 */
void set_value_cursor(struct menu_button Menu_Button[5], int cursor_pos);

/**
 * @brief Bewegt den Cursor im Mainmenü an eine übergebene Stelle
 * 
 * @param Menu_Button Array mit Menubuttons
 * @param array_length Arraylänge
 * @param position Position des Cursors
 */
void set_menucursor(struct menu_button Menu_Button[3], int array_length, int position);

/**
 * @brief Zeichnet den Menücursor an einer bestimmte Stelle
 * 
 * @param cords Coords der Position
 */
void draw_cursor(COORD cords);

/**
 * @brief Löscht den Menücursor an einer bestimmte Stelle
 * 
 * @param cords Coords der Position
 */
void erase_cursor(COORD cords);

/**
 * @brief Get the console window size object
 * 
 * @param hConsoleOutput Console Handle
 * @return COORD Größe der Console als Coord-Objekt
 */
COORD get_console_window_size(HANDLE hConsoleOutput);

/**
 * @brief Setzt die Console auf Vollbild (so wie alt+enter)
 * 
 */
void set_console_fullscreen();

#endif
