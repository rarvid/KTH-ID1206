#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>

static pthread_mutex_t p_mutex;

int p_count = 0;

void *p_test(void *arg) {
    int loop = *(int*)arg;
    for(int i = 0; i < loop; i++){
        pthread_mutex_lock(&p_mutex);
        p_count++;
        pthread_mutex_unlock(&p_mutex);
    }
}

int main(int argc, char* argv[]) {
    int loops = atoi(argv[1]);

    int rc = pthread_mutex_init(&p_mutex, NULL);
    assert(rc == 0);

    clock_t p_start, p_stop;
    pthread_t p0, p1, p2;
    p_start = clock();
    pthread_create(&p0, NULL, p_test, (void *)&loops);
    pthread_create(&p1, NULL, p_test, (void *)&loops);
    pthread_create(&p2, NULL, p_test, (void *)&loops);
    pthread_join(p0, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    p_stop = clock();

    pthread_mutex_destroy(&p_mutex);

    double p_time = ((double)p_stop - p_start) / CLOCKS_PER_SEC; // in seconds
    p_time = p_time * 1000 * 1000; 

    printf("%.f\t%d\n", p_time, p_count);
}
