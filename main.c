#include <stdio.h>
#include <stdlib.h>

#define X_value 50
#define Y_value 50

struct cell{

    int alive;
    //struct cell &neigbor1;
    //struct cell &neigbor2;
    //struct cell &neigbor3;
    //struct cell &neigbor4;
    //struct cell &neigbor5;
    //struct cell &neigbor6;
    //struct cell &neigbor7;
    //struct cell &neigbor8;

};

int main()
{
    int x;
    int y;
    struct cell spielfeld[X_value][Y_value];

    for(y = 0; y < Y_value; y++){
        for(x = 0; x < X_value; x++){
            spielfeld[x][y].alive = 0;
        }
    }


    spielfeld[0][0].alive = 1;


    for(y = 0; y < Y_value; y++){
        for(x = 0; x < X_value; x++){
            printf("%i ",spielfeld[x][y].alive);
        }
        printf("\n");
    }

    return 0;
}
