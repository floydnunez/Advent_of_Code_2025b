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
    int MAXLIMIT = strcmp(FILENAME, "input/input.txt") == 0? 100000: 16;
    printf("max limit: %d strcmp: %d\n", MAXLIMIT, strcmp(FILENAME, "input/input.txt"));
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
    // part 2
    int lla[MAX][5];
    for (int jj = 0;jj < 4; jj++) {
        for (int ii = 0; ii < MAX; ii++) {
            if (isspace(all[jj][ii]) || all[jj][ii] == 0) {
                lla[ii][jj] = -1;
            } else {
                lla[ii][jj] = all[jj][ii] - '0';
            }
        }
    }

    int pows[] = { 1, 10, 100, 1000 };
    int nums[MAX];
    for (int ii = 0; ii < MAX; ii++) {
        nums[ii] = 0;
    }

    char oper[MAX];
    last = all[4][0];

    for (int ii = 0; ii < MAX; ii++) {
        oper[ii] = last;
        if (all[4][ii] != 0 && !isspace(all[4][ii])) {
            last = all[4][ii];
        }
    }

    for (int ii = 0; ii < MAX; ii++) {
        int pos = 0;
        if (lla[ii][0] == -1 && lla[ii][1] == -1 && lla[ii][2] == -1 && lla[ii][3] == -1) {
            nums[ii] = -1;
        }
        for (int jj = 3; jj >= 0; jj--) {
            if (lla[ii][jj] == -1) {
                continue;
            }
            int subtotal = lla[ii][jj] * pows[pos];
            nums[ii] += subtotal;
            //printf("nums[%d][%d] = %d, %d, pows: %d pos: %d\n", ii, jj, nums[ii], lla[ii][jj], pows[pos], pos);
            pos++;
        }
    }
    printf("uh\n");
    long totals[1000];
    for (int ii = 0; ii < 1000; ii++) {
        totals[ii] = -1;
    }

    int pos = 0;
    totals[0] = oper[0] == '*'? 1: 0;
    for (int ii = 0; ii < MAX && ii < MAXLIMIT; ii++) {
        printf("nums[%d] = \t%d %c\n", ii, nums[ii], oper[ii]);
        if (nums[ii] == -1) {
            pos++;
            totals[pos] = oper[ii] == '*'? 1: 0;
            continue;
        }
        if (oper[ii] == '+') {
            totals[pos] += nums[ii];
        } else {
            totals[pos] *= nums[ii];
        }
        printf("totals[%d] = %ld oper %c to %d\n", pos, totals[pos], oper[ii], nums[ii]);
    }
    long total_p2 = 0;
    for (int ii = 0; ii < 1000 && ii < MAXLIMIT; ii++) {
        printf("totals[%d] = \t%ld\n", ii, totals[ii]);
        if (totals[ii] > 0) {
            total_p2 += totals[ii];
        }
    }
    printf("part 2: total: %ld\n", total_p2);
    //116139322782 too low due to using ints instead of longs for the totals
    //10153315705125
}