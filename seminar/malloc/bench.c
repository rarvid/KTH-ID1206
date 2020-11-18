#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "rand.h"
#include "dlmall.h"

#define ROUNDS 10
#define LOOP 10
#define BUFFER 100

int main() {
    void *buffer[BUFFER];
    for(int i = 0; i < BUFFER; i++){
        buffer[i] = NULL;
    }
    void *init = sbrk(0);
    void *current;

    printf("The initial top of the heap is %p.\n", init);

    for(int j = 0; j < ROUNDS; j++) {
        for(int i = 0; i < LOOP; i++) {
            int index = rand() % BUFFER;
            if (buffer[index] != NULL){
                dfree(buffer[index]);
                buffer[index] = NULL;
            }
            size_t size = (size_t) request();
            int *memory;
            memory = dalloc(size);
            if (memory == NULL) {
                fprintf(stderr, "malloc failed\n");
                return 1;
            }
            buffer[index] = memory;
            // writing to memory so we know it exists
            *memory = 123;
        }
        current = sbrk(0);
        int allocated = (int)((current - init) / 1024);
        printf("%d\n", j);
        printf("The current top of the heap is %p.\n", current);
        printf("    increased by %d Kbyte\n", allocated);
    }
    return 0;
}