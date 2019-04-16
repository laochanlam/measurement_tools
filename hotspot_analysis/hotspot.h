#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <libunwind.h>
#include <stdio.h>
/*
struct itimerval tick;
struct itimerval zero;

struct func_record {
    char name[1024];
    int occurrence;
} func_record[1024];

int func_count = 0;
*/
void run_sample();

void signal_handler(int signal);

void hot_spot_analysis();

void print_analysis(); 
