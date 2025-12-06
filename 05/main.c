#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"

char *FILENAME = "input/input.txt";

typedef struct _data {
    long ini, fin;
    int valid;
} data;

int comp(const void *a, const void *b) {
    const data *da = (const data *)a;
    const data *db = (const data *)b;
    if (da->ini < db->ini) return -1;
    if (da->ini > db->ini) return 1;
    return 0;
}

int main(int argc, char **argv){
    FILE *f;
    int bufferLength = 255;
    char buffer[bufferLength];

    f = fopen(FILENAME, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    data ranges[1024];
    long ids[1024];
    for (int i = 0; i < 1024; i++) {
        ids[i] = -1;
        ranges[i].valid = 0;
    }
    int index = 0;
    int mode = 0;
    int max_ranges = 1;
    long max_range_end = 0;
    long max_value = 0;
    while(fgets(buffer, bufferLength, f)) {
        printf("%d \t %s", index, buffer);
        if (mode == 0) {
            if (strlen(buffer) < 2) {
                printf("switching mode\n");
                mode = 1;
                index = 0;
                continue;
            }
            long ini, fin;
            sscanf(buffer, "%ld-%ld", &ini, &fin);
            data *d = malloc(sizeof(data));
            d->ini = ini;
            d->fin = fin;
            if (ini > fin) {
                exit(1);
            }
            d->valid = 1;
            ranges[index++] = *d;
            max_ranges++;
            if (max_range_end < fin) {
                max_range_end = fin;
            }
        } else {
            long num;
            sscanf(buffer, "%ld", &num);
            ids[index++] = num;
            if (num > max_value) {
                max_value = num;
            }
        }
    }
    printf("\n------\n");
    index = 0;
    data curr = ranges[0];
    int total_1 = 0;
    for (int i = 0; i < 1024 && ids[i] >= 0; i++) {
        long val = ids[i];
        for(int jj = 0; jj < max_ranges && ranges[jj].valid == 1; jj++) {
            if (val >= ranges[jj].ini && val <= ranges[jj].fin) {
                total_1++;
                break;
            }
        }
    }
    int equals = 0;
    //not sure if I can deal with equality and inclusion in the same pass
    for(int jj = 0; jj < max_ranges; jj++) {
        if (ranges[jj].valid != 1) {
            continue;
        }
        data r1 = ranges[jj];
        for(int kk = jj + 1; kk < max_ranges; kk++) {
            if (ranges[kk].valid != 1) {
                continue;
            }
            data r2 = ranges[kk];
            if (r1.ini == r2.ini && r1.fin == r2.fin) {
                printf("removing %d (%ld - %ld) cause it is equals to %d  (%ld - %ld)\n", kk, r2.ini, r2.fin, jj, r1.ini, r1.fin);
                equals++;
                ranges[kk].valid = 0;
                continue;
            }
        }
    }

    int contains = 0;
    for(int jj = 0; jj < max_ranges; jj++) {
        data r1 = ranges[jj];
        if (r1.valid != 1) {
            continue;
        }
        for(int kk = 0; kk < max_ranges; kk++) {
            //do note the 0. we want to check them all twice, in both orders
            if(jj == kk) { //so we need this check
                continue;
            }
            data r2 = ranges[kk];
            //printf("checking for inclusion: %d (%d) and %d (%d)\n", jj, r1.valid, kk, r2.valid);
            if (r2.valid != 1) {
                continue;
            }
            if ((r1.ini < r2.ini && r1.fin < r2.ini) || (r2.ini < r1.ini && r2.fin < r1.ini)) {
                //disjunct
                continue;
            }
            if (r1.ini <= r2.ini && r1.fin >= r2.fin) {
                //since we do it twice, we only consider one type of inclusion
                printf("removing %d (%ld - %ld) cause it is contained in %d  (%ld - %ld)\n", kk, r2.ini, r2.fin, jj, r1.ini, r1.fin);
                contains++;
                ranges[kk].valid = 0;
                continue;
            }
        }
    }
    //merging only works with sorted ranges. Don't ask why
    int merged;
    int morder = 0;
    do {
        merged = 0;
        for(int jj = 0; jj < max_ranges; jj++) {
            if (ranges[jj].valid != 1) continue;

            for(int kk = jj+1; kk < max_ranges; kk++) {
                if (ranges[kk].valid != 1) continue;

                if (ranges[jj].ini < ranges[kk].ini && ranges[kk].ini <= ranges[jj].fin) {
                    //r2 starts inside r1
                    printf("merging (%ld - %ld) and (%ld - %ld) --- ", ranges[jj].ini, ranges[jj].fin, ranges[kk].ini, ranges[kk].fin);
                    ranges[jj].fin = ranges[kk].fin;
                    ranges[jj].valid = 1; //necessary as the check is in the outer loop
                    printf("merged to (%ld - %ld) %d x %d, %d\n", ranges[jj].ini, ranges[jj].fin, jj, kk, morder++);
                    ranges[kk].valid = 0;
                    merged++;
                } else if (ranges[kk].ini < ranges[jj].ini && ranges[jj].ini <= ranges[kk].fin) {
                    //r1 starts inside r2
                    printf("merging (%ld - %ld) and (%ld - %ld) --- ", ranges[kk].ini, ranges[kk].fin, ranges[jj].ini, ranges[jj].fin);
                    ranges[kk].fin = ranges[jj].fin;
                    printf("merged to (%ld - %ld) %d x %d  %d\n", ranges[kk].ini, ranges[kk].fin, jj,kk, morder++);
                    ranges[jj].valid = 0;
                    merged++;
                }
            }
        }
    } while(merged > 0);
    long total_2 = 0;
    for(int jj = 0; jj < max_ranges; jj++) {
        data r1 = ranges[jj];
        if (r1.valid != 1) {
            continue;
        }
        long subtotal = r1.fin - r1.ini + 1;
        total_2 += subtotal;
        printf("adding new range: %d (%ld - %ld) = %ld \t = %ld\n", jj, r1.ini, r1.fin, subtotal, total_2);
    }
    printf("part 1 total: %d\n", total_1);
    printf("part 1 max_ranges: %d\n", max_ranges);
    printf("part 2 equals: %d\n", equals);
    printf("part 2 contains: %d\n", equals);
    printf("part 2 total: %ld\n", total_2);
    printf("part 2 max_value    : %ld\n", max_value);
    printf("part 2 max_range_end: %ld\n", max_range_end);
    //348548952146322 too high
    //348548952146313 right
    //338205036561241 unsorted wrong answer
    // 10343915585072
    //340382455313623 too low
    //344465703729972 wrong
    //without eliminating inclusion: 338205036561241
    //338205036561241

    //174444174621323 small.txt
    // 67133934674381 small unsorted

    //153577049118233 small 50
    //152424348352989
    printf("argc: %s\n", argc == 1? "sorted" : "UNsorted");
}