#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
int fd;

static long diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec - t1.tv_nsec < 0) {
        diff.tv_sec = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
} 

void dump_cache() {
        fd = open("/proc/sys/vm/drop_caches", O_WRONLY);
        // printf("fsync: %s\n", strerror(errno));
        write(fd, "3", sizeof(char));
        // printf("fsync: %s\n", strerror(errno));
        sync();
        // printf("fsync: %s\n", strerror(errno));
        close(fd);
}

int main() {
    // printf("%d", sizeof(__uint128_t));
    // allocate 1gb memory
    long long int *ptr = (char*) malloc(1024 * 1024 * 1024 * sizeof(char));

    bool is_dump_cache = false;
    int fd;
    struct timespec start, end;
    // 125 mb to 1gb
    for (long long int i=1; i < 5; i++) {

        if (is_dump_cache)
            dump_cache();

        // 256 mb/unit
        long long int count = 128 * 1024 * 256 * i;
        // clock_gettime(CLOCK_REALTIME, &start);
        
        // 10 times
        long int total_time = 0;
        for (int k =0; k < 10; k++){
            if (is_dump_cache)
                dump_cache();
            clock_gettime(CLOCK_REALTIME, &start);
            for (long long int j=0; j<count; j++) {
                ptr[j] = 0;
                // printf("%p %p\n", ptr+i, ptr+i+1);
            }
            clock_gettime(CLOCK_REALTIME, &end);
            total_time += diff_in_us(start, end);
        }
        // clock_gettime(CLOCK_REALTIME, &end);
        // long int cpu_time = diff_in_us(start, end);
        printf("Execution Time: %ld us\n\n", total_time);
        printf("Execution Time: %lf GB/s\n\n", 10 * (double)(i*0.25)/(double)((double)total_time/(double)1000000));
    }
    close(fd);
    // printf("%p %p", ptr, ptr+1);
    free(ptr);
}   