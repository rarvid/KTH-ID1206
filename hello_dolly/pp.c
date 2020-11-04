#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    int pid = fork();

    if(pid == 0){
        printf("I'm the child %d with parent %d\n", (int) getpid(), (int) getppid());
        // getppid() returns parent process identifier
    } else {
        printf("I'm the parent %d with parent %d\n", (int) getpid(), (int) getppid());
        wait(NULL);
    }

    return 0;
}