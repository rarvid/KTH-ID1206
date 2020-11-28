#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
int count = 0;
volatile int global = 0;

int try(volatile int *mutex) {
  return __sync_val_compare_and_swap(mutex, 0 , 1);
}

int lock(volatile int *mutex) {
  int spin = 0;
  while(try(mutex) != 0) {
    spin++;
    sched_yield();
  }
  return spin;
}

void unlock(volatile int *mutex) {
  *mutex = 0;
}

typedef struct args {
  int inc;
  int id;
  volatile int *mutex;
} args;

void *increment(void *arg) {
  int inc = ((args *)arg)->inc;
  int id = ((args *)arg)->id;
  volatile int *mutex = ((args *)arg)->mutex;
  
  int tot = 0;
  for(int i =0; i < inc; i++) {
    tot += lock(mutex);
    count++;
    unlock(mutex);
  }
  printf("number of tries:  %d\n", tot);
}

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("usage peterson <inc>\n");
    exit (0);
  }

  int inc = atoi(argv[1]);

  pthread_t one_p, two_p;
  args one_args, two_args;
  
  one_args.inc = inc;
  two_args.inc = inc;

  one_args.id = 0;
  two_args.id = 1;

  one_args.mutex = &global;
  two_args.mutex = &global;

  struct timespec t_start, t_stop;

  clock_gettime(CLOCK_MONOTONIC_COARSE, &t_start);
  
  pthread_create(&one_p, NULL, increment, &one_args);
  pthread_create(&two_p, NULL, increment, &two_args);
  pthread_join(one_p, NULL);
  pthread_join(two_p, NULL);

  clock_gettime(CLOCK_MONOTONIC_COARSE, &t_stop);

  long wall_sec = t_stop.tv_sec - t_start.tv_sec;
  long wall_nsec = t_stop.tv_sec - t_start.tv_sec;
  long wall_msec = (wall_sec * 1000) + (wall_nsec / 1000000);
  
  printf("done in %ld ms \n", wall_msec);
  printf("result is %d\n", count);
  return 0;
}



