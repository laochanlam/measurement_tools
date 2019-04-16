#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <libunwind.h>
#include <stdio.h>
#include "hotspot.h"
//extern void hot_spot_detect();
//extern void print_result();

void funca(){
    for (int i = 0; i < 100000; i++);
}

int funcb(){
    for (int i = 0; i < 1000000000; i++);
}

void funcc(){
    for (int i =0; i < 1000000000; i++);
}

int main(){

    hot_spot_analysis();
    funca();
    funcb();
    funcc();
    print_analysis();
}
