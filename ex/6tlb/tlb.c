#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGES 16
#define REFS 1024*10
#define PAGESIZE 4*1024

int main(int argc, char *argv[]){
    clock_t c_start, c_stop;

    printf("#TLB experiment\n");
    printf("#   page size - %d bytes\n", PAGESIZE);
    printf("#   max pages - %d\n", PAGES);
    printf("#   total number or refrences - %d Mi\n", REFS/(1024*1024));
    printf("#pages\t proc\t sum\n");
    for(int pages = 1; pages <= PAGES; pages++) {
        int loops = REFS / pages;

        c_start = clock();
        long sum = 0;
        for(int l = 0; l < loops; l++) {
            char *memory = malloc((long)PAGESIZE * PAGES);
            for(int p = 0; p < pages; p++) {
                long ref = (long)p * PAGESIZE;
                // force page to be allocated
                memory[ref] += 1;
                sum += memory[ref];
            }
        }
        c_stop = clock();

        {
            double proc;
            proc = ((double)(c_stop - c_start)) / CLOCKS_PER_SEC;

            printf("%d\t %.6f\t %ld\n", pages, proc, sum);
        }
    }
    return 0;
}