
#include <stdio.h>

int mod(int a, int b) {
    return (a%b+b)%b;
}

int main() {
    printf("mod: %d\n", -502 % 100);
    printf("mod: %d\n", mod(-502 , 100));

}

