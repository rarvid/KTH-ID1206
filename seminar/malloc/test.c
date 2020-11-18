
#include <stdio.h>
#include <stdlib.h>
#include "dlmall.h"
#include <string.h>

int main() {
    char *array1 = dalloc(sizeof(char) * 20);
    char *array2 = dalloc(sizeof(char) * 100);
    char *array3 = dalloc(sizeof(char) * 74);
    char *array4 = dalloc(sizeof(char) * 10);
    printf("        \n\nAFTER ALLOC\n\n");
    sanity();

    dfree(array1);
    dfree(array2);
    dfree(array3);
    dfree(array4);
    printf("        \n\nAFTER FREE\n\n");
    sanity();
    return 0;
}