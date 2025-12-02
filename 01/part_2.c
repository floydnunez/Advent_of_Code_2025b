#include <stdio.h>
#include <stdlib.h>

typedef struct _data {
    char direction;
    int value;
} data;

data array[10000];

void add_data(char dir, int val, int pos) {
    array[pos] = (data){dir, val};
}

int main(int argc, char **argv){
    FILE *f;
    int bufferLength = 255;
    char buffer[bufferLength];

    f = fopen("input/input.txt", "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    data end = (data){'N', 0};
    for (int i = 0; i < 10000; i++ ){
        array[i] = end;
    }

    int ii = 0;
    while(fgets(buffer, bufferLength, f)) {
        char dir;
        int val;
        sscanf(buffer, "%c%d", &dir, &val);
        add_data(dir, val, ii++);
    }
    for (int i = 0; i < 10000; i++ ){
        if (array[i].direction == 'N') {
            break;
        }
    }

    int position = 50;
    int zeros = 0;
    int additions = 0;
    for (int i = 0; i < 10000; i++ ){
        data row = array[i];
        if (row.direction == 'N') {
            break;
        }
        int diff = row.value * (row.direction == 'R'? 1: -1);
        printf("data: %c = %d position %d\n", array[i].direction, array[i].value, position + diff);
        // fuck it, just move the wheel one by one
        for (int jj = 0; jj < row.value; jj++) {
            position += (row.direction == 'R'? 1: -1);
            if (position < 0) {
                position += 100;
            }
            if (position >= 100) {
                position -= 100;
            }
            if (position == 0) {
                zeros++;
            }
            additions++;
        }
        printf("\tpos: %d   \tzeros: %d  \t new position: %d\n\n", position, zeros, position);
    }
    printf("final pos: %d\n", position);
    printf("password: %d\n", zeros);
    printf("additions: %d\n", additions);
    //5632 too low

}