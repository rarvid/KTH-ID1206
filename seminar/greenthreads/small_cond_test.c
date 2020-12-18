#include <stdio.h>
#include "green.h"

int flag = 0;
green_cond_t *cond;

void *test(void *arg) {
  int id = *(int*)arg;
  int loop = 4;
  while(loop > 0) {
    if(flag == id) {
      printf("thread %d: %d\n", id, loop);
      loop--;
      green_cond_signal(cond);
    } else {
      green_cond_wait(cond);
    }
  }
}

int main() {
  green_t g0, g1;
  int a0 = 0;
  int a1 = 1;
  green_create(&g0, test, &a0);
  green_create(&g1, test, &a1);

  green_join(&g0, NULL);
  green_join(&g1, NULL);
  printf("done\n");
  return 0;
}
