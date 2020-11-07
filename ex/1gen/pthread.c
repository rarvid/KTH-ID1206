#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // for sleep()

volatile int counter = 0;

int loops = 0;

void* worker(void* name) {
    
    for(int i = 0; i < loops; i++) {

        counter++;
       // printf("worker %s %d\n", (char*)name, counter);
    }

    return NULL;
}

int main(int argc, char*  argv[]) {

    if (argc != 2){

        fprintf(stderr, "usage: threads <loops>\n");
    }

    loops = atoi(argv[1]);

    pthread_t p1, p2;

    pthread_create(&p1, NULL, &worker, "A");
    pthread_create(&p1, NULL, &worker, "B");

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("Final value is %d\n", counter);
    return 0;
}