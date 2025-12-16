#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include <ctype.h>
#include <limits.h>

char *FILENAME = "input/input.txt";

typedef struct _data {
    char name[30];
    int set;
    int x, y, z;
} data;

typedef struct pair {
    int i, j;
    long distance;
} pair;

data *hasht = NULL;

int comp(const void *a, const void *b) {
    const pair *pair_a = (const pair *)a;
    const pair *pair_b = (const pair *)b;
    if (pair_a->distance < pair_b->distance) return -1;
    if (pair_a->distance > pair_b->distance) return 1;
    return 0;
}

int compare_desc(const void *a, const void *b) {
    return *(const int *)b - *(const int *)a;
}

long calc_distance(data d1, data d2) {
    long dx = d1.x - d2.x;
    long dy = d1.y - d2.y;
    long dz = d1.z - d2.z;
    return dx * dx + dy * dy + dz * dz;
}

int main(int argc, char **argv){
    int MAX = 20;
    int JOINS = 10;
    int TWOINS = 29; //part two joins
    if (strcmp(FILENAME, "input/input.txt") == 0) {
        MAX = 1000;
        JOINS = 1000;
        TWOINS = 4765;
    }

    FILE *f;
    int bufferLength = 255;
    char buffer[bufferLength];

    f = fopen(FILENAME, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    data all[MAX];
    int index = 0;
    while(fgets(buffer, bufferLength, f)) {
        printf("%s", buffer);
        int x, y, z;
        sscanf(buffer, "%s", all[index].name);
        sscanf(buffer, "%d,%d,%d", &x, &y, &z);
        all[index].set = index; //initially every set just points to itself
        all[index].x = x;
        all[index].y = y;
        all[index].z = z;
        index++;
    }
    printf("\nfile read\n");
    for (int ii = 0; ii < MAX; ii++) {
        //printf("all[%d] = %s\n", ii, all[ii].name);
    }
    pair distances[MAX*MAX];
    for (long ii = 0; ii < MAX * MAX; ii++) {
        distances[ii].i = -1;
        distances[ii].j = -1;
        distances[ii].distance = -1;
    }
    int count = 0;
    long min = LONG_MAX;
    int min_index_i = -1, min_index_j = -1;
    for (int ii = 0; ii < MAX; ii++) {
        for (int jj = ii + 1; jj < MAX; jj++) {
            distances[count].i = ii;
            distances[count].j = jj;
            if (ii == jj) {
                distances[count].distance = -1;
                count++;
                continue;
            }
            distances[count++].distance = calc_distance(all[ii], all[jj]);
        }
    }
    int n = count;
    qsort(distances, n, sizeof(distances[0]), comp);
    for (int ii = 0; ii < MAX; ii++) {
        if (distances[ii].distance > 0) {
            printf(" + pair: [%d] %s (%d) %s (%d) = %ld\n", ii, all[distances[ii].i].name, distances[ii].i,
                all[distances[ii].j].name, distances[ii].j, distances[ii].distance);
        }
    }
    printf("distances sorted\n");

    for (int kk = 0; kk < JOINS; kk++) {
        pair d = distances[kk];
        printf("joining %s (%d) (set: %d) and %s (%d) (set: %d)   dist: %ld\n", all[d.i].name, d.i, all[d.i].set, all[d.j].name, d.j, all[d.j].set, d.distance);

        data di = all[d.i];
        data dj = all[d.j];

        int oldset = dj.set;
        int newset = di.set;

        if (oldset == newset) {
            printf("early nothing happens\n");
            continue;
        }
        for (int ii = 0; ii < MAX; ii++) {
            //printf("cmp: %d\n",strcmp(all[ii].set, all[d.j].set));
            if (all[ii].set == oldset ) {
                printf("  I need to join the sets %d (of %s) and %d (of %s) so writing %d as the set of %s\n", all[ii].set, all[ii].name, di.set, all[di.set].name,
                    di.set, all[ii].name);
                all[ii].set = newset;
            } else if (all[ii].set == newset) {
                printf("  nothing happens for [%d] and [%d]\n", ii, d.i);
            }
        }
        for (int ii =0; ii < MAX; ii++){
            if (all[ii].set == oldset) {
                printf("dangling set! %d %s %d\n", ii, all[ii].name, all[ii].set);
                exit(99);
            }
        }
    }

    int sets[MAX];
    for (int ii = 0; ii < MAX; ii++) {
        sets[ii] = 0;
    }
    printf("sets reset\n");
    for (int ii = 0; ii < MAX; ii++) {
        sets[all[ii].set] += 1;
    }

    // Sort descending
    qsort(sets, MAX, sizeof(int), compare_desc);
    printf("[%d ", sets[0]);
    for (int ii = 1; ii < MAX; ii++) {
        printf(",%d ", sets[ii]);
    }
    printf("]\n");
    // Multiply top 3
    long answer = (long)sets[0] * sets[1] * sets[2];
    printf("Part 1: %ld\n", answer);
    //377 * 15 * 14 = 79170...too low
    //99 * 15 * 279 = 414315 wrong
    //75 * 43 * 114 = 367650 wrong
    //[785 * 10 * 7 = 54950 wrong
    //73 *52 * 26 = 98696 right


    //part 2. Let's start over
    printf("part 2\n");
    for (long ii = 0; ii < MAX; ii++) {
        all[ii].set = ii;
    }
    printf("sets reset\n");
    pair last;
    for (int kk = 0; kk < TWOINS; kk++) {
        pair d = distances[kk];
        //printf("joining %s (%d) (set: %d) and %s (%d) (set: %d)   dist: %ld\n", all[d.i].name, d.i, all[d.i].set, all[d.j].name, d.j, all[d.j].set, d.distance);
        last = d;
        data di = all[d.i];
        data dj = all[d.j];

        int oldset = dj.set;
        int newset = di.set;

        if (oldset == newset) {
            //printf("early nothing happens\n");
            continue;
        }
        printf("pair: %d (%s) x %d (%s)\n", d.i, di.name, d.j, dj.name);
        for (int ii = 0; ii < MAX; ii++) {
            if (all[ii].set == oldset ) {
                printf("  %d\tI need to join the sets %d (of %s) and %d (of %s) so writing %d as the set of %s\n", kk, all[ii].set, all[ii].name, di.set, all[di.set].name,
                    di.set, all[ii].name);
                all[ii].set = newset;
            } else if (all[ii].set == newset) {
                printf("  nothing happens for [%d] and [%d]\n", ii, d.i);
            }
        }
        for (int ii =0; ii < MAX; ii++){
            if (all[ii].set == oldset) {
                printf("dangling set! %d %s %d\n", ii, all[ii].name, all[ii].set);
                exit(99);
            }
        }
    }
    for (int ii = 0; ii < MAX; ii++) {
        sets[ii] = 0;
    }
    printf("sets reset AGAIN\n");
    for (int ii = 0; ii < MAX; ii++) {
        sets[all[ii].set] += 1;
    }

    // Sort descending
    qsort(sets, MAX, sizeof(int), compare_desc);
    printf("[%d ", sets[0]);
    for (int ii = 1; ii < MAX; ii++) {
        printf(",%d ", sets[ii]);
    }
    printf("]\n");
    printf("last pair: %i = %s and %i = %s\n", last.i, all[last.i].name, last.j, all[last.j].name);
    long x1 = all[last.i].x;
    long x2 = all[last.j].x;
    printf("part 2: %ld\n", x1 * x2);
    //last pair: 331 = 42116,99530,49060 and 549 = 53310,91520,51546
    //part 2: 2245203960  right
    //do note I binary searched TWOINS and if it goes over, it naturally changes the answer
}
