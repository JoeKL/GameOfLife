#include "buffer.h"

int calc_buffersize(COORD gridsize){
    return sizeof(char) * gridsize.X * gridsize.Y * 2;
}

int calc_positionInBuffer(COORD gridsize, const int x_pos, const int y_pos){
    return  ( y_pos * gridsize.X + x_pos ) * 2;
}

void alloc_buffer(char **buffer, COORD gridsize){
    *buffer = (char*)malloc( calc_buffersize(gridsize) );
}

void dealloc_buffer(char **buffer){
    free(buffer);
}

void print_buffer(char *buffer){
    set_cursor(0,0);
    printf("%s\n", buffer);
    set_cursor(0,0);
}

void init_buffer(char *buffer, COORD gridsize, char symbolAlive, char symbolDead){

    int buffer_size = calc_buffersize(gridsize);
    int written_chars = 0;

    for(int y = 0; y < gridsize.Y; y++){
        for(int x = 0; x < gridsize.X; x++){

            if(x == gridsize.X - 1){
               written_chars += snprintf(buffer + written_chars, buffer_size,"%c", symbolDead);
            } else {
               written_chars += snprintf(buffer + written_chars, buffer_size,"%c ", symbolDead);
            }

        }
        written_chars += snprintf(buffer + written_chars, buffer_size,"\n");
    }

}

// void update_buffer(char *buffer, struct cell **grid_ptr, struct cell **gridcopy_ptr, const int x_size, const int y_size, char symbolAlive, char symbolDead){

//     for(int y = 0; y < y_size; y++){
//         for(int x = 0; x < x_size; x++){
//             if (grid_ptr[x][y].alive != gridcopy_ptr[x][y].alive) {

//                 if (grid_ptr[x][y].alive == 1) {
                    
//                     buffer[calc_positionInBuffer(x_size, y_size, x, y)] = symbolAlive;

//                 }else {

//                     buffer[calc_positionInBuffer(x_size, y_size, x, y)] = symbolDead;

//                 }

//             }
//         }
//     }
// }

void update_buffer_at_coord(char *buffer, COORD gridsize, char symbolAlive, char symbolDead, int x_pos, int y_pos, int alive){
    if (alive) {
        
        buffer[calc_positionInBuffer(gridsize, x_pos, y_pos)] = symbolAlive;

    }else {

        buffer[calc_positionInBuffer(gridsize, x_pos, y_pos)] = symbolDead;

    }
}
