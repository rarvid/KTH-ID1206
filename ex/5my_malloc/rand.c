#include <math.h>
#include <stdlib.h>

#define MAX 1000
#define MIN 8

int request() {

    // k = log(MAX/MINs)
    double k = log(((double) MAX)/ MIN);

    // r is [0..k]
    double r = ((double)( rand() % (int)(k * 100000))) / 100000;

    // size is [0..MAX]
    int size = (int)((double)MAX / exp(r));

    return size;
}
