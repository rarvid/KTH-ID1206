
#include <stdio.h>
#include <stdlib.h>
#include "dlmall.h"
#include <string.h>

int main() {
    char *array1 = dalloc(sizeof(char) * 16, 1);
    char *array2 = dalloc(sizeof(char) * 9, 1);
    char *array3 = dalloc(sizeof(char) * 7, 1);
    char *array4 = dalloc(sizeof(char) * 32, 1);
    printf("        \n\nAFTER ALLOC\n\n");
    sanity();
    // 0 no merge, 1 merge
    // 0 no sort, 1 sort
    dfree(array1,1,1);
    dfree(array2,1,1);
    dfree(array3,1,1);
    dfree(array4,1,1);
    printf("        \n\nAFTER FREE\n\n");
    sanity();
    return 0;
}