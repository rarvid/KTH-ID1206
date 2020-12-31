#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "green.h"

green_mutex_t g_mutex;

int g_count = 0;


void *g_test(void *arg) {
    int loop = *(int*)arg;
    for(int i = 0; i < loop; i++){
        green_mutex_lock(&g_mutex);
        g_count++;
        green_mutex_unlock(&g_mutex);
    }
}

int main(int argc, char* argv[]) {
    int loops = atoi(argv[1]);

    green_mutex_init(&g_mutex);

    clock_t g_start, g_stop;

    green_t g0 ,g1 ,g2;
    g_start = clock();
    green_create(&g0, g_test, (void *)&loops);
    green_create(&g1, g_test, (void *)&loops);
    green_create(&g2, g_test, (void *)&loops);
    green_join(&g0, NULL);
    green_join(&g1, NULL);
    green_join(&g2, NULL);
    g_stop = clock();

    double g_time = ((double)g_stop - g_start) / CLOCKS_PER_SEC; // in seconds
    g_time = g_time * 1000 * 1000;
    printf("%.4f\t%d\n", g_time, g_count);
}
