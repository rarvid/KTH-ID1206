#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

char global[] = "This is a global string";
const int read_only = 123456;

int main() {

    int pid = getpid();

foo:
    printf("process id: %d\n", pid);
    printf("global string: %p\n", &global);
    printf("read_only: %p\n", &read_only);
    printf("the code: %p\n", &&foo);

    printf("\n\n /proc/%d/maps \n\n", pid);
    char command[50];
    sprintf(command, "cat /proc/%d/maps \n\n", pid);
    system(command);

    return 0;
}