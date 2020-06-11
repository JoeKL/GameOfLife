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

### mermaid integration test

```mermaid
graph TD
A[Hard] -->|Text| B(Round)
B --> C{Decision}
C -->|One| D[Result 1]
C -->|Two| E[Result 2]
```

```mermaid
sequenceDiagram
Alice->>John: Hello John, how are you?
loop Healthcheck
    John->>John: Fight against hypochondria
end
Note right of John: Rational thoughts!
John-->>Alice: Great!
John->>Bob: How about you?
Bob-->>John: Jolly good!
```

```mermaid
gantt
section Section
Completed :done,    des1, 2014-01-06,2014-01-08
Active        :active,  des2, 2014-01-07, 3d
Parallel 1   :         des3, after des1, 1d
Parallel 2   :         des4, after des1, 1d
Parallel 3   :         des5, after des3, 1d
Parallel 4   :         des6, after des4, 1d
```

```mermaid
classDiagram
Class01 <|-- AveryLongClass : Cool
<<interface>> Class01
Class09 --> C2 : Where am i?
Class09 --* C3
Class09 --|> Class07
Class07 : equals()
Class07 : Object[] elementData
Class01 : size()
Class01 : int chimp
Class01 : int gorilla
class Class10 {
  <<service>>
  int id
  size()
}
```

```mermaid
stateDiagram
[*] --> Still
Still --> [*]
Still --> Moving
Moving --> Still
Moving --> Crash
Crash --> [*]
```

```mermaid
pie
"Dogs" : 386
"Cats" : 85
"Rats" : 15
```
