#include "profile.h"

#define EVENTSIZE  4
int EventSet;
long_long start[EVENTSIZE], end[EVENTSIZE], values[EVENTSIZE];

void init_and_start_collect() {
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
        exit(-1);
    EventSet = PAPI_NULL;

    if (PAPI_create_eventset(&EventSet) != PAPI_OK)
        exit(-1);

// Section1 CPI 
//    if (PAPI_add_event(EventSet, PAPI_TOT_CYC) != PAPI_OK)
//        exit(-1);
//   if (PAPI_add_event(EventSet, PAPI_LST_INS) != PAPI_OK)
//        exit(-1);


// Section2 L1 cache miss rate
//    if (PAPI_add_event(EventSet, PAPI_SR_INS) != PAPI_OK)
//        exit(-1);
//    if (PAPI_add_event(EventSet, PAPI_LD_INS) != PAPI_OK)
//        exit(-1);
//    if (PAPI_add_event(EventSet, PAPI_L1_TCM) != PAPI_OK)
//        exit(-1);
	
 
// Section3 L2 cache miss rate
//    if (PAPI_add_event(EventSet, PAPI_L1_TCM) != PAPI_OK)
//        exit(-1);
//    if (PAPI_add_event(EventSet, PAPI_L2_TCM) != PAPI_OK)
//        exit(-1);


// Section4 TLB miss rate
    if (PAPI_add_event(EventSet, PAPI_LD_INS) != PAPI_OK)
        exit(-1);
    if (PAPI_add_event(EventSet, PAPI_SR_INS) != PAPI_OK)
        exit(-1);
    if (PAPI_add_event(EventSet, PAPI_TLB_DM) != PAPI_OK)
        exit(-1);
//    if (PAPI_add_event(EventSet, PAPI_TLB_IM) != PAPI_OK)
//        exit(-1);



/*   // [4]Level 1 cache misses
    if (PAPI_add_event(EventSet, PAPI_L1_TCM) != PAPI_OK)
        exit(-1);
    // [5]Level 1 load misses
    if (PAPI_add_event(EventSet, PAPI_L1_LDM) != PAPI_OK)
        exit(-1);
    // [6]Level 1 store misses
    if (PAPI_add_event(EventSet, PAPI_L1_STM) != PAPI_OK)
        exit(-1);
    printf("add okall");
*/
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


        for (int i = 0; i < EVENTSIZE; i++) {
            values[i] = end[i] - start[i];
//            printf("%lld\n", values[i]);
        }

// Section1 CPI
//        printf("CPI: %f\n", (values[1]/(float)values[0]));

// Section2 L1 cache miss rate
//        printf("L1 cache miss rate: %f\n", values[2]/(float)(values[0]+values[1]));

// Section3 L2 cache miss rate
//        printf("L2 cache miss rate: %f\n", values[1]/(float)(values[0]));

// Section4 TLB miss rate
        printf("TLB miss rate: %f\n", (values[2]+values[3])/(float)(values[0]+values[1]));
}
