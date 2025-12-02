#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *INPUT_FILE = "input/input.txt";

typedef struct _data {
    long ini, fin;
    int valid;
    int repeating_ini, repeating_fin;
} data;

void zero(long *array, int length) {
    for(int i = 0; i < length; i++) {
        array[i] = 0l;
    }
}

int check_not_repeat(char *number, int pos, int x) {
    char repeat_char = number[pos];
    int len = strlen(number);
    int sublen = len / x;
    for (int i = 1; i < x; i++) {
        if (repeat_char != number[pos + sublen * i]) {
            return 1; //does NOT repeat;
        }
    }
    return 0; //does repeat
}

int is_made_of_x_repeaded_strings(char *number, long num, int x) {
    //printf("num int: %ld, num str: %s\n", num, number);
    int len = strlen(number);
    if (len % x == 0) {
        int sublen = len / x;
        for (int i = 0; i < sublen; i++) {
            if (check_not_repeat(number, i, x)) {
                //printf("is invalid! %s at %d %c != %c\n", number, i, number[i], number[i+len]);
                return 0; //is invalid!
            }
        }
    } else {
        return 0;
    }
    return 1;
}

int is_elf_invalid(long num, int part) {
    char number[100];
    sprintf(number, "%ld", num);

	if (part == 1) {
		return  is_made_of_x_repeaded_strings(number, num, 2);
	}
	//fuck it, every possibility
    int elf_invalid = is_made_of_x_repeaded_strings(number, num, 10)
    || is_made_of_x_repeaded_strings(number, num, 9)
    || is_made_of_x_repeaded_strings(number, num, 8)
    || is_made_of_x_repeaded_strings(number, num, 7)
    || is_made_of_x_repeaded_strings(number, num, 6)
    || is_made_of_x_repeaded_strings(number, num, 5)
    || is_made_of_x_repeaded_strings(number, num, 4)
    || is_made_of_x_repeaded_strings(number, num, 3)
    || is_made_of_x_repeaded_strings(number, num, 2);

    return elf_invalid;
}

long *find_invalids(data d, int part) {
    long *result = malloc(sizeof (long)*1024*1024);
    zero(result, 1024*1024);
    int pos = 0;
    for (long i = d.ini; i <= d.fin; i++) {
        if (is_elf_invalid(i, part)) {
            result[pos++] = i;
        }
    }
    return result;
}

int main(int argc, char **argv){
    FILE *f;
    int bufferLength = 1024 * 1024;
    char buffer[bufferLength];

    f = fopen(INPUT_FILE, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);

    int MAX = 10000;
    data array[MAX];
    for (int i = 0; i < MAX; i++) {
        array[i].valid = 0;
        array[i].repeating_ini = 0;
        array[i].repeating_fin = 0;
    }
    while(fgets(buffer, bufferLength, f)) {
        printf("%s\n", buffer);
        char *token;
        token = strtok(buffer, ",");
        int i = 0;
        while(token != NULL) {
            sscanf(token, "%ld-%ld", &array[i].ini, &array[i].fin);
            token = strtok(NULL, ",");
            array[i].valid = 1;
            if (array[i].ini < 0 || array[0].fin < 0) {
                printf("neg number??? %s\n", token);
                exit(1);
            }
            i++;
        }
    }

    long total_part_1 = 0;
    long total_part_2 = 0;
    for (int i = 0; i < MAX && array[i].valid == 1; i++) {
        //printf("data: ini %ld fin %ld\n", array[i].ini, array[i].fin);
        long *invalids_1 = find_invalids(array[i], 1);
        long *invalids_2 = find_invalids(array[i], 2);
        for (int jj = 0; jj < bufferLength; jj++) {
            total_part_1 += invalids_1[jj];
            total_part_2 += invalids_2[jj];
        }
        free(invalids_1);
        free(invalids_2);
    }
    printf("total 1: %ld\n", total_part_1);
    printf("total 2: %ld\n", total_part_2);
}