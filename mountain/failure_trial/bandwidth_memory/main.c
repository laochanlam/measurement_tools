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
    int mb_unit = 256;
    int loop_time = 10;
    bool is_dump_cache = true;

    // allocate 1gb memory
    long long int *ptr = (char*) malloc(1024 * 1024 * 1024 * sizeof(char));

    int fd;
    struct timespec start, end;
    double average_time = 0.0;
    for (long long int i=1; i <= 1024/mb_unit; i++) {

        if (is_dump_cache)
            dump_cache();

        long long int count = 128 * 1024 * mb_unit * i;
        
        // 10 times
        long int total_time = 0;
        for (int k =0; k < loop_time; k++){
            if (is_dump_cache)
                dump_cache();
            clock_gettime(CLOCK_REALTIME, &start);
            // 8byte pre loop
            for (long long int j=0; j<count; j++) {
                ptr[j] = 0;
                // printf("%p %p\n", ptr+j, ptr+j+1);
            }
            clock_gettime(CLOCK_REALTIME, &end);
            total_time += diff_in_us(start, end);
        }
        average_time += loop_time * (double)(i*mb_unit/(double)1024)/(double)((double)total_time/(double)1000000);
        // printf("Execution Time: %ld us\n\n", total_time);
        // printf("Execution Bandwidth: %lf GB/s\n\n", loop_time * (double)(i*mb_unit/(double)1024)/(double)((double)total_time/(double)1000000));
    }
    average_time /= (float)1024/mb_unit;
    printf("\nAverage Execution Bandwidth: %lf GB/s\n\n", average_time);
    close(fd);
    free(ptr);
}   