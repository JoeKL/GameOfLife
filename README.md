# Game Of Life - C Implementierung
##### von Lothar Gomoluch, Oliver Röckener und Niko Tepe

(https://sourceforge.net/projects/mingw-w64/files/Toolchains targetting Win32/Personal Builds/mingw-builds/installer/mingw-w64-install.exe/download "MinGW Compiler used")

## Dokumentation

Hier entsteht eine Dokumentation.

### Abstrakte Datenstrukturen


### Strukturen

```c
struct cell{
    int alive;
    struct cell *neighborCell[8];
};
```

```c
struct settings{
    
    COORD gridsize;

    char symbolAlive;
    char symbolDead;

    int generationsToCalc;
    int iterationsPerSecond;

    COORD hud_currentGeneration_pos;  
    COORD hud_gridSize_pos;
    COORD hud_generationsToCalc_pos;
    COORD hud_iterationsPerSecond_pos;
    COORD hud_aliveCells_pos;
    COORD hud_shortcutInfo_pos;
};
```

#### void kill\_buffer\_at\_coord (char \* *buffer*, struct settings *gamesettings*, int *x\_pos*, int *y\_pos*)

Sets the char in buffer at coord to symbolDead.

##### Parameters
