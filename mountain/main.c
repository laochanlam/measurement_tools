#include <stdio.h>
#include "fcyc2.h"
#include "clock.h"
#define MINBYTES (1 << 11)  /* Working set size ranges from 2 KB */
#define MAXBYTES (1 << 25)  /* ... up to 64 MB */
#define MAXSTRIDE 64        /* Strides range from 1 to 64 elems */
#define MAXELEMS MAXBYTES/sizeof(double) 


double data[MAXELEMS];

int test(int elems, int stride) {
    long i;
    long sx2 = stride * 2;
    long sx3 = stride * 3;
    long sx4 = stride * 4;
    long acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;
    long length = elems;
    long limit = length - sx4;

    for (i = 0; i < limit; i+= sx4) {
        acc0 = acc0 + data[i];
        acc1 = acc1 + data[i + stride];
        acc2 = acc2 + data[i + sx2];
        acc3 = acc3 + data[i + sx3];
    }

    for (; i < length; i+=stride) {
        acc0 = acc0 + data[i];
    }

    return ((acc0 + acc1) + (acc2 + acc3));
}

double run(int size, int stride, double myMhz) {
    double cycles;
    int elems = size / sizeof(double);
    cycles = fcyc2(test, elems, stride, 0);
//    printf("[inside run] size: %d, stride: %d, cycles: %f, Mhz:%f\n", size, stride, cycles, myMhz);
    return (size / stride) / (cycles / myMhz);
}


int main(){
    double myMhz = mhz(1);
//    printf("myMhz: %f\n", myMhz);
    for (int i =0; i < MAXELEMS; i++) 
        data[i] = i;

    for (int size = MAXBYTES; size >= MINBYTES; size>>=1) {
        for (int stride = 1; stride <= MAXSTRIDE; stride++) {
	    printf("%d %d %f\n", size, stride, run(size, stride, myMhz));
	}
    }
    //double result = run(4096, 4, myMhz);
    //printf("%f\n", result);
}
