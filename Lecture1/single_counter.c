#include <stdlib.h>
#include <stdio.h> // printf()
#include <unistd.h> // sleep()

int counter = 0; // global variable

int main (int argc, char *argv[]) {

    while (counter != 10) {

        counter++;
        printf("Hello, %s %d %p\n",  argv[1], counter, &counter);
        sleep(1);
    }

    return 0;
}