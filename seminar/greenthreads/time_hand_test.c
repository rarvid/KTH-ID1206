#include <stdio.h>
#include <unistd.h>
#include "green.h"

void *test(void *arg) {
  int i = *(int*)arg;
  int loop = 4;
  while(loop > 0) {
    printf("thread %d: %d\n", i, loop);
    loop--;
  }
  return 0;
}

int main() {
  green_t g0, g1, g2;
  int a0 = 0;
  int a1 = 1;
  int a2 = 2;
  green_create(&g0, test, &a0);
  green_create(&g1, test, &a1);
  green_create(&g2, test, &a2);

  green_join(&g0, NULL);
  green_join(&g1, NULL);
  green_join(&g2, NULL);
  printf("done\n");
  return 0;
}
