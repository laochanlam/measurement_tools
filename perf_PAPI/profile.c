#include "profile.h"

    int EventSet;

    int i, sum;

    long_long start[2], end[2], values[2];



void init_and_start_collect() {
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
        exit(-1);
    EventSet = PAPI_NULL;

    if (PAPI_create_eventset(&EventSet) != PAPI_OK)
        exit(-1);

    if (PAPI_add_event(EventSet, PAPI_TOT_CYC) != PAPI_OK)
        exit(-1);

    if (PAPI_add_event(EventSet, PAPI_LST_INS) != PAPI_OK)
        exit(-1);

    //if (PAPI_add_event(EventSet, ) != PAPI_OK)
    //    exit(-1);


    if (PAPI_start(EventSet) != PAPI_OK)
        exit(-1);

    if (PAPI_read(EventSet, start) != PAPI_OK)
        exit(-1);
}

//
  //  for (i=0;i<10000;i++)
   // sum+=i;
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
    values[0]=end[0]-start[0];

    values[1]=end[1]-start[1];

    
    printf("PAPI_TOT_CYC:%lld\nPAPI_LST_INS: %lld\n",values[0], values[1]);
    printf("CPI: %f\n", values[1]/(float)values[0]);


}
