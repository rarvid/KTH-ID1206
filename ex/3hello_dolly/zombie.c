#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    int pid = fork();

    if(pid == 0){
        printf("check the status\n");
        sleep(10);
        printf("and again (pid:%d)\n", (int) getpid());
        return 42;
    } else {
        sleep(20);
        int res;
        wait(&res);
        printf("The result was %d\n", WEXITSTATUS(res));
        printf("and again (pid:%d)\n", (int) getpid());
        sleep(10);
    }

    printf("That's it %d\n", (int) getpid());

    return 0;
}