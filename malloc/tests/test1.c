#include <stdio.h>
#include <stdlib.h>

#include "../src/libs.h"

int main()
{
    char *ptr = malloc(3);
    char *ptr2 = malloc(15);
    char *ptr22 = calloc(4, 15);
    char *ptr3 = realloc(ptr, 6000);
    char *ptr4 = malloc(6000);

    ptr[0] = 2;
    ptr[1] = 8;
    ptr[2] = 10;

    for (int i = 0; i < 3; i++)
        printf("value : %d\n", ptr[i]);

    puts("\n");

    free(ptr);
    puts("Premier pointer enlevé\n");
    free(ptr2);
    free(ptr22);
    puts("Deuxieme pointer enlevé\n");
    free(ptr3);
    puts("Troisieme pointer enlevé\n");
    free(ptr4);
    puts("Quatreieme pointer enlevé\n");
    return 0;
}
