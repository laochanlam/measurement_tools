#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <libunwind.h>
#include <stdio.h>
#include "hotspot.h"

struct itimerval tick; 
struct itimerval zero;
   
struct func_record {
    char name[1024];
    int occurrence;
    float percentage;
} func_record[1024]; 

int func_count = 0; 
int sample_rate = 0;

void run_sample() {
   
    unw_context_t context;
    unw_cursor_t cursor;
    unw_word_t offset;
    char buffer[1024] = {};
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    int next_res;
    for (int i = 0; i < 3; i++)
        unw_step(&cursor);
    buffer[0] = '\0';       
    unw_get_proc_name(&cursor, buffer, sizeof(buffer), &offset);
    //printf("%s\n", buffer);   
    
    bool found = false;
    for (int i = 0; i < func_count; i++) {
        if (strcmp(func_record[i].name, buffer) == 0) {
	     func_record[i].occurrence++;
	     found = true;
	}
    }
    if (!found) {
	strcpy(func_record[func_count].name, buffer);
	func_record[func_count].occurrence = 0;
	func_count++;
    }

}

void signal_handler(int signal) {
    if (signal == SIGALRM) {
        //tick.it_value.tv_usec = i1;
        //setitimer(ITIMER_REAL, &tick, NULL);
	run_sample();
    }
}

void hot_spot_analysis(int usec) {
    sample_rate = usec;
    signal(SIGALRM, signal_handler);
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_usec = usec;    
    tick.it_interval.tv_usec = usec;
    setitimer(ITIMER_REAL, &tick, NULL);
}


void print_analysis(){ 
    int total_occurrence = 0;
    for (int i = 0; i < func_count; i++)
        total_occurrence += func_record[i].occurrence;
    for (int i = 0; i < func_count; i++) 
        func_record[i].percentage = func_record[i].occurrence / (float) total_occurrence;

    for (int i = 0; i < func_count; i++) {
        for (int j = i+1; j < func_count; j++) {
	    if (func_record[i].percentage > func_record[j].percentage) {
	        struct func_record tmp = func_record[i];
		func_record[i] = func_record[j];
		func_record[j] = tmp;
	    }
	}
    }
    printf("\n\n#####################Stat Info#####################\n");
    printf("[sample rate] %d usecs/time\n", sample_rate);
    for (int i = 0; i < func_count; i++)
        printf("%s\t%f%%\n", func_record[i].name, 100 * func_record[i].percentage);
    printf("#####################Stat Info#####################\n\n\n");
}

