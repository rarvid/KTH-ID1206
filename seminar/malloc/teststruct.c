
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

typedef struct something{
    int x;
    long y;
    char z;
    struct something *next;
}something;

int main(){
    something *test = malloc(sizeof(something));
    test->x = 1;
    test->y = 3;
    test->z = 'z';
    // test->next = malloc(sizeof(something));
    printf("int of something %d\n", test->x);
    printf("long of something %ld\n", test->y);
    printf("char of something %c\n", test->z);
    printf("whole struct lol %p\n", test->next);
    return 0;
}