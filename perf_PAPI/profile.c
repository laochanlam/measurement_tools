#include "profile.h"

int EVENTSIZE = 8;
int EventSet;
long_long start[8], end[8], values[8];

void init_and_start_collect() {
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
        exit(-1);
    EventSet = PAPI_NULL;

    if (PAPI_create_eventset(&EventSet) != PAPI_OK)
        exit(-1);

    // [0]Total cycles
    if (PAPI_add_event(EventSet, PAPI_TOT_CYC) != PAPI_OK)
        exit(-1);
    // [1]Load/store instructions completed
    if (PAPI_add_event(EventSet, PAPI_LST_INS) != PAPI_OK)
        exit(-1);
    // [2]Level 1 data cache misses
    if (PAPI_add_event(EventSet, PAPI_L1_DCM) != PAPI_OK)
        exit(-1);
    // [3]instruction cache misses
    if (PAPI_add_event(EventSet, PAPI_L1_ICM) != PAPI_OK)
        exit(-1);
    // [4]Level 1 cache misses
    if (PAPI_add_event(EventSet, PAPI_L1_TCM) != PAPI_OK)
        exit(-1);
    // [5]Level 1 load misses
    if (PAPI_add_event(EventSet, PAPI_L1_LDM) != PAPI_OK)
        exit(-1);
    // [6]Level 1 store misses
    if (PAPI_add_event(EventSet, PAPI_L1_STM) != PAPI_OK)
        exit(-1);
    printf("add okall");
    if (PAPI_start(EventSet) != PAPI_OK)
        exit(-1);

    if (PAPI_read(EventSet, start) != PAPI_OK)
        exit(-1);
}

void end_of_collect(){
    if (PAPI_stop(EventSet, end) != PAPI_OK)
        exit(-1);

    if (PAPI_cleanup_eventset(EventSet) != PAPI_OK)
        exit(-1);

    if (PAPI_destroy_eventset(&EventSet) != PAPI_OK)
        exit(-1);

    PAPI_shutdown();
}
    /* Get value */

void calculate_and_printout(){

    // for 
    // printf("PAPI_TOT_CYC:%lld\nPAPI_LST_INS: %lld\n",values[0], values[1]);
    // printf("CPI: %f\n", values[1]/(float)values[0]);

        for (int i = 0; i < EVENTSIZE; i++) {
            values[i] = end[i] - start[i];
            printf("%lld\n", values[i]);
        }
}