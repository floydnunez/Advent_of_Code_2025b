#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"

char *FILENAME = "input/1.txt";

typedef struct _data {
    char name[10];
    int value;
    UT_hash_handle hh;
} data;

data *hasht = NULL;

void add_data(char *n, int v) {
    data *newval;
    newval = malloc(sizeof *newval);
    strcpy(newval->name, n);
    newval->value = v;
    HASH_ADD_INT(hasht, name, newval);
}

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
}