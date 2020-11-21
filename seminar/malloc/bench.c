#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "rand.h"
#include "dlmall.h"

#define ROUNDS 10
#define BUFFER 10

int main(int argc, char* argv[]) {
    int LOOP =   atoi(argv[1]);
    void *buffer[BUFFER];
    for(int i = 0; i < BUFFER; i++){
        buffer[i] = NULL;
    }
    void *init = sbrk(0);
    void *current;

    for(int j = 0; j < ROUNDS; j++) {
        for(int i = 0; i < LOOP; i++) {
            int index = rand() % BUFFER;
            if (buffer[index] != NULL){
                //first: 1 for merge , 0 for no merge
                //second: 1 for insert sort, 0 insert no sort
                dfree(buffer[index], 0, 1);
                buffer[index] = NULL;
            } else {
                size_t size = (size_t) request();
                int *memory;
            
                memory = dalloc(size, 1);  
                if (memory == NULL) {
                    fprintf(stderr, "malloc failed\n");
                    return 1;
                }
                buffer[index] = memory;
                // writing to memory so we know it exists
                *memory = 123;
            }
        }
        // sanity();
    }
    get_block_count();
    
    return 0;
}
