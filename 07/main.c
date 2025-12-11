#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include <ctype.h>

char *FILENAME = "input/input.txt";

int count(int MAXX, int MAXY, int x, int y, char grid[MAXY][MAXX], int oldtimelines);

long results[143][143];
long down(int MAXX, int MAXY, int x, int y, char grid[MAXY][MAXX]);

int main(int argc, char **argv){
    int MAXX, MAXY;
    if (strcmp(FILENAME, "input/input.txt") == 0) {
        MAXX = 142;
        MAXY = 142;
    }
    if (strcmp(FILENAME, "input/1.txt") == 0) {
        MAXX = 16;
        MAXY = 16;
    }
    if (strcmp(FILENAME, "input/2.txt") == 0) {
        MAXX = 16;
        MAXY = 10;
    }

    FILE *f;
    int bufferLength = 255;
    char buffer[bufferLength];

    f = fopen(FILENAME, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    char grid[MAXY][MAXX];
    int row = 0;
    while(fgets(buffer, bufferLength, f)) {
        sscanf(buffer, "%s", grid[row++]);
    }

    int total = 0;
    int total_even = 0;
    int split = 0;
    int start = 0;
    for(int yy = 0; yy < MAXY; yy++) {
        printf("%s\n", grid[yy]);
        for(int xx = 0; xx < MAXX; xx++) {
            if (yy == 0) {
                if (grid[yy][xx] == 'S') {
                    start = xx;
                }
            } else {
                if (grid[yy-1][xx] == '|' || grid[yy-1][xx] == 'S') {
                    if (grid[yy][xx] == '.') {
                        grid[yy][xx] = '|';
                    } else if (grid[yy][xx] == '^') {
                        split++;
                        grid[yy][xx-1] = '|';
                        grid[yy][xx+1] = '|';
                    }
                }
            }

            if (grid[yy][xx] == '^') {
                total++;
                total_even += yy % 2? 1: 0;
            }
        }
    }
    printf("*****\n");
    for(int yy = 0; yy < MAXY; yy++) {
        printf("%s\n", grid[yy]);
    }

    printf("part 1 total = %d\n", split);
    //1649 too high. it's for someone else? (it was just the total of ^s which is wrong)
    //1524
    int S = 0;
    for (int xx = 0; xx < MAXX; xx++) {
        if (grid[0][xx] == 'S') {
            S = xx;
            break;
        }
    }
    for (int yy = 0; yy < 143; yy++) {
        for (int xx = 0; xx < 143; xx++) {
            results[yy][xx] = -1;
        }
    }
    long timelines = down(MAXX, MAXY, 0, S, grid);

    //part 2
    printf("part 2: timelines: %ld\n", timelines);
    //2937 too low
    //32982105837605 correct

}
long down(int MAXX, int MAXY, int y, int x, char grid[MAXY][MAXX]) {
    printf("down! y: %d, x:%d, %c\n", y, x, grid[y][x]);
    if(results[y][x] >= 0) {
        printf("cached result[%d][%d] = %ld\n", y, x, results[y][x]);
        return results[y][x];
    }
    if (y >= MAXY) {
        printf("out the bottom at %d, %d\n", y, x);
        return 1;
    }
    if (x >= MAXX) { //??
        printf("exiting through the side? shouldn't happen\n");
        return 0;
    }
    //printf("down! y: %d, x:%d, %c\n", y, x, grid[y][x]);
    if (grid[y][x] == '|' || grid[y][x] == 'S') {
        long val = down(MAXX, MAXY, y+1, x, grid);
        results[y+1][x] = val;
        printf("caching temp y= %d and x= %d. value: %ld\n", y, x, val);
        return val;
    }
    if (grid[y][x] == '^') {
        long val_minus_1 = down(MAXX, MAXY, y, x-1, grid);
        long val_plus_1 = down(MAXX, MAXY, y, x+1, grid);
        results[y][x-1] = val_minus_1;
        results[y][x+1] = val_plus_1;
        results[y][x] = val_minus_1 + val_plus_1;
        printf("caching temp values y= %d and x= %d and %d. value = %ld and %ld\n", y, x-1, x+1, val_minus_1, val_plus_1);
        return val_minus_1 + val_plus_1;
    }
    printf("fell through? shouldn't also happen?\n");
    return 0;
}