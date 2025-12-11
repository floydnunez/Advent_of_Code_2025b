#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include <ctype.h>

char *FILENAME = "input/1.txt";

int count(int MAXX, int MAXY, int x, int y, char grid[MAXY][MAXX], int oldtimelines);

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
    printf("part 1? total ^ = %d total ^ on even lines %d\n", total, total_even);
    //1649 too high. it's for someone else? (it was just the total of ^s which is wrong)
    //1524

    //part 2
    int timelines = 0;
    for (int xx = 0; xx < MAXX; xx++) {
        if (grid[MAXY-1][xx] == '|') {
            timelines += count(MAXX, MAXY, MAXY-1, xx, grid, 0);
        }
    }
    printf("part 2: timelines: %d\n", timelines);
    //2937 too low

}

int count(int MAXX, int MAXY, int y, int x, char grid[MAXY][MAXX], int oldtimelines) {
    printf("referencing %d, %d in \t", y, x);
    for (int xx = 0; xx < MAXX; xx++) {
        printf("%c", x != xx && grid[y][xx] == '|' ? '.' : grid[y][xx]);
    }
    printf("\n");

    if (grid[y][x] == 'S' || y <= 0) {
        printf("inicio at %d %d, tm: %d\n", y, x, oldtimelines);
        return oldtimelines + 1;
    }
    if (grid[y-1][x] == '|' || grid[y-1][x] == 'S') {
        return count(MAXX, MAXY, y-1, x, grid, oldtimelines);
    }
    if (grid[y-1][x] == '.') {
        int subtotal = oldtimelines;
        if (x > 0) {
            if (grid[y][x-1] == '^') {
                subtotal += count(MAXX, MAXY, y, x-1, grid, oldtimelines);
            }
        }
        if (x < MAXX) {
            if (grid[y][x+1] == '^') {
                subtotal += count(MAXX, MAXY, y, x+1, grid, oldtimelines);
            }
        }
        return subtotal;
    }
    printf("default at %d %d\n", y, x);
    return oldtimelines;
}