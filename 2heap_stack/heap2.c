#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    long *heap = (unsigned long*)calloc(40, sizeof(unsigned long));

    int pid = getpid();
    printf("heap[2]: 0x%lx\n", heap[2]);
    printf("heap[1]: 0x%lx\n", heap[1]);
    printf("heap[0]: 0x%lx\n", heap[0]);
    printf("heap[-1]: 0x%lx\n", heap[-1]);
    printf("heap[-2]: 0x%lx\n", heap[-2]);

    free(heap);
    printf("\nheap freed\n\n");

    printf("heap[2]: 0x%lx\n", heap[2]);
    printf("heap[1]: 0x%lx\n", heap[1]); // this will have address to heap in memory
    printf("heap[0]: 0x%lx\n", heap[0]);
    printf("heap[-1]: 0x%lx\n", heap[-1]);
    printf("heap[-2]: 0x%lx\n", heap[-2]);
    
    free(heap);
    printf("\nheap freed\n\n");

    printf("heap[2]: 0x%lx\n", heap[2]);
    printf("heap[1]: 0x%lx\n", heap[1]); // this will have address to heap in memory
    printf("heap[0]: 0x%lx\n", heap[0]);
    printf("heap[-1]: 0x%lx\n", heap[-1]);
    printf("heap[-2]: 0x%lx\n", heap[-2]);
    printf("\n\n /proc/%d/maps \n\n", pid);
    
    char command[50];
    sprintf(command, "cat /proc/%d/maps", pid);
    system(command);

    return 0;
}