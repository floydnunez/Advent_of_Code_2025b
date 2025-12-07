#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include <ctype.h>

char *FILENAME = "input/input.txt";

typedef struct _data {
    char name[10];
    int value;
    UT_hash_handle hh;
} data;

data *hasht = NULL;

int main(int argc, char **argv){
    FILE *f;
    int bufferLength = 5000;
    char buffer[bufferLength];

    f = fopen(FILENAME, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    long numbers[4][1000];
    for (int jj = 0; jj < 4; jj++) {
        for (int ii = 0; ii < 1000; ii++) {
            numbers[jj][ii] = 0;
        }
    }
    char *str_numbers[5][1000];
    for (int jj = 0; jj < 5; jj++) {
        for (int ii = 0; ii < 1000; ii++) {
            str_numbers[jj][ii] = malloc(sizeof (char) * 10);;
        }
    }
    #define MAX 3745
    char all[5][MAX];
    for (int jj = 0;jj < 4; jj++) {
        for (int ii = 0; ii < MAX; ii++) {
            all[jj][ii] = '\0';
        }
    }

    char *operators[1000];
    for (int ii = 0; ii < 1000; ii++) {
        operators[ii] = malloc(sizeof (char) * 10);;
    }


    int count = 0;
    while(fgets(buffer, bufferLength, f)) {
        printf("%s\n", buffer);
        int pos = 0;
        char *token;
        sprintf(all[count], "%s", buffer);

        if (count < 4) {
            token = strtok(buffer, " ,");
        } else {
            token = strtok(buffer, " ");
        }
        printf("-count: %d %s\n", count,token);
        while(token != NULL) {
            if (count < 4) {
                sscanf(token, "%s", str_numbers[count][pos]);
                token = strtok(NULL, " ,");
                pos++;
            } else {
                printf("reading: %s\n", token);
                sscanf(token, "%s", operators[pos]);
                token = strtok(NULL, " ");
                pos++;
            }
        }
        count++;
        printf("+count: %d\n", count);
    }

    char last = ' ';
    for (int ii = 0; ii < MAX; ii++) {
        if (all[4][ii] != ' ') {
            last = all[4][ii];
        } else {
            all[4][ii] = last;
        }
    }
    long results[1000];
    for (int ii = 0; ii < 1000; ii++) {
        if (strchr(operators[ii], '*') != NULL) {
            results[ii] = 1;
        } else {
            results[ii] = 0;
        }
    }
    for (int jj = 0; jj < 4; jj++) {
        for (int ii = 0; ii < 1000; ii++) {
            numbers[jj][ii] = atoi(str_numbers[jj][ii]);
            printf("%ld, ",numbers[jj][ii]);
        }
        printf("\n");
    }

    for (int jj = 0; jj < 4; jj++) {
        for (int ii = 0; ii < 1000 ; ii++) {
            if (strchr(operators[ii], '+') != NULL) {
                printf("%d \t %ld adding to %ld\n", ii, numbers[jj][ii], results[ii]);
                results[ii] += numbers[jj][ii];
            } else if (strchr(operators[ii], '*') != NULL) {
                printf("%d \t %ld multiplying to %ld\n", ii, numbers[jj][ii], results[ii]);
                results[ii] *= numbers[jj][ii];
            }
        }
    }
    long total = 0;
    printf("\n");
    for (int ii = 0; ii < 1000; ii++){
        total += results[ii];
        printf("result[%d] = %ld\n", ii, results[ii]);
    }
    printf("part 1 total %ld\n", total);
    // 5595593539811
}