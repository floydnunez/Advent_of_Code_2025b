#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include <ctype.h>

char *FILENAME = "input/input.txt";

typedef struct _pair {
    int x, y;
} pair;

typedef struct _data {
    char name[10];
    char value;
    int surrounding;
    UT_hash_handle hh;
} data;

data *hasht = NULL;

void add_data(char *n, char v) {
    data *newval;
    newval = malloc(sizeof *newval);
    strcpy(newval->name, n);
    newval->value = v;
    newval->surrounding = 0;
    HASH_ADD_STR(hasht, name, newval);
}

data *dat(int row, int col) {
    char name[10];
    sprintf(name, "%d,%d", col, row);
    data *d;
    HASH_FIND_STR(hasht, name, d);
    return d;
}
char at(int row, int col) {
    data *d;
    d = dat(row, col);
    if (d == NULL) {
        return '-';
    }
    return d->value;
}

int printmap(int height, int width) {
    int total = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            data *d = dat(y, x);
            if (d->value == '@' && d->surrounding < 4) {
                total++;
                printf("x");
            } else {
                printf("%c", at(y, x));
            }
        }
        printf("\n");
    }
    return total;
}

void calculate(int len, int row) {
    pair eight[] = { (pair) { -1, -1 }, (pair) { 0, -1 }, (pair) { 1, -1},
        (pair) { -1, 0 }, (pair) { 1, 0},
        (pair) { -1, 1 }, (pair) { 0, 1 }, (pair) { 1, 1},
    };
    for (int x = 0; x < len; x++) {
        for (int y = 0; y < row; y++) {
            data *d = dat(y, x);
            d->surrounding = 0;
            if (d->value == '@') {
                for (int i = 0; i < 8; i++) {
                    char v = at(y+eight[i].y, x+eight[i].x);
                    if (v == '@') {
                        d->surrounding++;
                    }
                }
            }
        }
    }
}

int remove_rolls(int height, int width) {
    int total = 0;
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            data *d = dat(y, x);
            if (d->value == '@' && d->surrounding < 4) {
                d->value = '.';
                total++;
            }
        }
    }
    return total;
}

int main(int argc, char **argv){
    FILE *f;
    int bufferLength = 255;
    char buffer[bufferLength];

    f = fopen(FILENAME, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    int row = 0;
    int len = 0;
    while(fgets(buffer, bufferLength, f)) {
        printf("%s", buffer);
        char name[10];
        char value;
        len = strlen(buffer);
        char line[1000];
        for (int col = 0; col < len && !isspace(buffer[col]); col++) {
            sprintf(name, "%d,%d", col, row);
            add_data(name, buffer[col]);
        }
        row++;
    }

    pair eight[] = { (pair) { -1, -1 }, (pair) { 0, -1 }, (pair) { 1, -1},
        (pair) { -1, 0 }, (pair) { 1, 0},
        (pair) { -1, 1 }, (pair) { 0, 1 }, (pair) { 1, 1},
    };
    calculate(len, row);
    printf("\n----------\n");
    int total = printmap(len, row);
    printf("\npart 1 %d\n", total);
    int total_2 = 0;
    int removed;
    int iterations = 0;
    while((removed = remove_rolls(len, row)) > 0) {
        total_2 += removed;
        calculate(len, row);
        iterations++;
    }
    printmap(len, row);
    printf("\npart 2 %d\n", total_2);
    printf("\npart 2 iterations %d\n", iterations);

}