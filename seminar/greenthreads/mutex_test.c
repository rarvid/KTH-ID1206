#include <stdio.h>
#include <unistd.h>
#include "green.h"

int glob = 0;

green_mutex_t mutex;

void *test(void *arg) {
  int i = *(int*)arg;
  int loop = 1000000;
  while(loop > 0) {
    green_mutex_lock(&mutex);
    // printf("thread %d: %d\n", i, glob);
    glob++;
    loop--;
    green_mutex_unlock(&mutex);
  }
}

int main() {
  green_t g0, g1, g2;
  int a0 = 0;
  int a1 = 1;
  int a2 = 2;
  green_mutex_init(&mutex);
  green_create(&g0, test, &a0);
  green_create(&g1, test, &a1);
  green_create(&g2, test, &a2);

  green_join(&g0, NULL);
  green_join(&g1, NULL);
  green_join(&g2, NULL);
  printf("%d\n", glob);
  return 0;
}
