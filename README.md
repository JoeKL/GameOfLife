# Game Of Life - C Implementierung
##### von Lothar Gomoluch, Oliver Röckener und Niko Tepe


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

    int periodInSeconds;
    int iterationsPerSecond;

    COORD hud_currentGeneration_pos;  
    COORD hud_gridSize_pos;
    COORD hud_periodInSeconds_pos;
    COORD hud_iterationsPerSecond_pos;
    COORD hud_aliveCells_pos;
    
};
```

### Funktionen

[test]: (https://raw.githubusercontent.com/JoeKL/GameOfLife/master/doc/struct%20cell.png) "Test"