#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "green.h"

pthread_mutex_t p_mutex;
green_mutex_t g_mutex;

int p_count = 0;
int g_count = 0;

void *p_test(void *arg) {
    int loop = *(int*)arg;
    for(int i = 0; i < loop; i++){
        pthread_mutex_lock(&p_mutex);
        p_count++;
        pthread_mutex_unlock(&p_mutex);
    }
}

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

    pthread_mutex_init(&p_mutex, NULL);
    green_mutex_init(&g_mutex);

    clock_t p_start, p_stop;
    clock_t g_start, g_stop;
    pthread_t p0, p1, p2;
    p_start = clock();
    pthread_create(&p0, NULL, p_test, (void *)&loops);
    pthread_create(&p1, NULL, p_test, (void *)&loops);
    pthread_create(&p2, NULL, p_test, (void *)&loops);
    pthread_join(p0, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    p_stop = clock();

    green_t g0 ,g1 ,g2;
    g_start = clock();
    green_create(&g0, g_test, (void *)&loops);
    green_create(&g1, g_test, (void *)&loops);
    green_create(&g2, g_test, (void *)&loops);
    green_join(&g0, NULL);
    green_join(&g1, NULL);
    green_join(&g2, NULL);
    g_stop = clock();

    double p_time = ((double)p_stop - p_start) / CLOCKS_PER_SEC; // in seconds
    p_time = p_time * 1000 * 1000; 

    double g_time = ((double)g_stop - g_start) / CLOCKS_PER_SEC; // in seconds
    g_time = g_time * 1000 * 1000;
    printf("%.4f\t%.4f\t%d\t%d\n", p_time, g_time, p_count, g_count);


}