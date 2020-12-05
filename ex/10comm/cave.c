#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ITERATIONS 10
#define BURST 1000

int main() {
  int descr[2];
  /* create the name pipe */
  int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  mkfifo("sesame", mode);

  int flag = O_WRONLY;
  int pipe = open("sesame", flag);
  
  for(int i = 0; i < ITERATIONS; i++){
    for(int j = 0; j < BURST; j++){
      write(descr[1], "0123456789", 10);
    }
    printf("producer burst %d done\n", i);
  }

  printf("producer done\n");
  return 0;
}
