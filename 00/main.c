#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include <ctype.h>

char *FILENAME = "input/1.txt";

typedef struct _data {
    char name[10];
    int value;
    UT_hash_handle hh;
} data;

data *hasht = NULL;
char store(char tri, int x, int y, char *grid, int MAXX, int MAXY);
char get(int x, int y, char *grid, int MAXX, int MAXY);

void add_data(char *n, int v) {
    data *newval;
    newval = malloc(sizeof *newval);
    strcpy(newval->name, n);
    newval->value = v;
    HASH_ADD_INT(hasht, name, newval);
}

char store(char tri, int x, int y, char *grid, int MAXX, int MAXY);
char get(int x, int y, char *grid, int MAXX, int MAXY);

int main(int argc, char **argv){
    FILE *f;
    int bufferLength = 255;
    char buffer[bufferLength];

    f = fopen(FILENAME, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    while(fgets(buffer, bufferLength, f)) {
        printf("%s\n", buffer);
        char name[10];
        int value;
        sscanf(buffer, "%s = %d", name, &value);
        add_data(name, value);
    }

    printf("hello %d\n", HASH_COUNT(hasht));
    long hi = 348548952146313;
    long lo =  338205036561241;
    printf("%ld to %ld = %ld\n", hi, lo, (hi - lo));
    char c;
    printf("size of c: %lu\n", sizeof(int));
    int MAXX = 400;
    int MAXY = 29;
    char *grid = malloc(MAXY*MAXX/4 * sizeof(char));
    for (int ii = 0; ii < MAXX /4; ii++) {
        for (int jj = 0; jj < MAXX /4; jj++) {
            grid[ii * MAXX/4 + jj] = 0;
        }
    }
    printf("size of grid: %zu\n", sizeof(grid));
    int i = 0;
    printf("%d\t set %d\n", i++, store(0, 0, 0, grid, MAXX, MAXY)); //0
    printf("%d\t set %d\n", i++, store(1, 0, 0, grid, MAXX, MAXY));
    printf("%d\t set %d\n", i++, store(2, 0, 0, grid, MAXX, MAXY));

    printf("%d\t set %d\n", i++, store(0, 1, 0, grid, MAXX, MAXY)); //3
    printf("%d\t set %d\n", i++, store(1, 1, 0, grid, MAXX, MAXY));
    printf("%d\t set %d\n", i++, store(2, 1, 0, grid, MAXX, MAXY));

    printf("%d\t set %d\n", i++, store(0, 2, 0, grid, MAXX, MAXY)); //6
    printf("%d\t set %d\n", i++, store(1, 2, 0, grid, MAXX, MAXY));
    printf("%d\t set %d\n", i++, store(2, 2, 0, grid, MAXX, MAXY));

    printf("%d\t set %d\n", i++, store(0, 3, 0, grid, MAXX, MAXY)); //9
    printf("%d\t set %d\n", i++, store(1, 3, 0, grid, MAXX, MAXY));
    printf("%d\t set %d\n", i++, store(2, 3, 0, grid, MAXX, MAXY));

    printf("%d\t set %d\n", i++, store(0, 4, 0, grid, MAXX, MAXY)); //12
    printf("%d\t set %d\n", i++, store(1, 4, 0, grid, MAXX, MAXY));
    printf("%d\t set %d\n", i++, store(2, 4, 0, grid, MAXX, MAXY));

    printf("%d\t set %d\n", i++, store(0, 5, 0, grid, MAXX, MAXY)); //15
    printf("%d\t set %d\n", i++, store(1, 5, 0, grid, MAXX, MAXY));
    printf("%d\t set %d\n", i++, store(2, 5, 0, grid, MAXX, MAXY));

}
char powers[] = { 1, 4, 16, 64, 256 };
char store(char q, int x, int y, char *grid, int MAXX, int MAXY) {
    int pos = x / 4;
    int diff = x % 4;
    char chunk = grid[y * MAXX/4 + x];
    chunk |= q * powers[diff];
    grid[y * MAXX/4 + x] = chunk;
    return chunk;
}
char get(int x, int y, char *grid, int MAXX, int MAXY) {
    int pos = x / 4;
    int diff = x % 4;
    char chunk = grid[y * MAXX/4 + x];
    char q = 0 | q * powers[diff];
    grid[y * MAXX/4 + x] = chunk;
    printf(" get x:%d, y:%d = %d in %d\n", x, y, oldnum, oldchunk);
    return oldnum;
}
