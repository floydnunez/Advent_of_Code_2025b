#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "uthash.h"

char *FILENAME = "input/input.txt";

typedef struct _data {
    int val;
    int pos;
} data;

data *get_partial_joltage(char *line, int ini, int end) {
    data *result = malloc(sizeof (data));
    int len = strlen(line);
    char max_val = 0;
    int max_pos = 0;
    for (int i = ini; i < len - end; i++) {
        if (line[i] > max_val) {
            max_val = line[i];
            max_pos = i;
        }
    }
    result->val = max_val - '0';
    result->pos = max_pos;
    printf("search from %d to %d, string length: %d end: %d val: %d, pos %d\n",
        ini, len - end, len, end, result->val, result->pos);
    return result;
}

long p(int power) {
    double val = pow(10, power);
    return (long) val;
}

int main(int argc, char **argv){
    FILE *f;
    int bufferLength = 255;
    char buffer[bufferLength];

    f = fopen(FILENAME, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    int total = 0;
    long total_2 = 0;
    while(fgets(buffer, bufferLength, f)) {
        char line[1024];
        sscanf(buffer, "%s\n", line);
        printf("%s\n", line);
        { //part 1
            data *dec = get_partial_joltage(line, 0, 1);
            data *uni = get_partial_joltage(line, dec->pos+1, 0);
            int subtotal = dec->val * 10 + uni->val;
            total += subtotal;
            printf("dec: %d at %d, uni %d at %d = %d\n", dec->val, dec->pos, uni->val, uni->pos, subtotal);
            free(dec);
            free(uni);
        }
        { //part 2
            long subtotal = 0;
            int prev = 0;
            for (int i = 11; i >= 0; i--) {
                data *d = get_partial_joltage(line, prev, i);
                prev = d->pos + 1;
                subtotal += d->val * p(i);
                free(d);
            }
            char ssub[1023];
            sprintf(ssub, "%ld", subtotal);
            int len = strlen(ssub);
            printf("subtotal    : %ld = %s len: %d\n", subtotal, ssub, len);
            if (len != 12) {
                printf("fatal, length is not 12? %d\n", len);
                exit(1);
            }
            total_2 += subtotal;
        }
    }
    printf("total joltage part 1: %d\n", total);
    printf("total joltage part 2: %ld\n", total_2);
    //1719903126275598 too high
    //997766652324 too low
}