#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    int pid = fork();

    if(pid == 0){
        int child = getpid();
        printf("child:  session %d\n", getsid(child));
        // getsid() returns session identifier
    } else {
        int parent = getpid();
        printf("parent: session %d\n", getsid(parent));
    }

    return 0;
}

// a deamon is a process runs in the backgroud and is detached from any controlling terminal