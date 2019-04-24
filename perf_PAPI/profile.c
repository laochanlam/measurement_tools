#include "papi.h"

#include <stdlib.h>

#include <stdio.h>

int main() {

    int EventSet;

    int i, sum;

    long_long values[2], values1[2], values2[2];

    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
        exit(-1);
    EventSet = PAPI_NULL;

    if (PAPI_create_eventset(&EventSet) != PAPI_OK)
        exit(-1);

    if (PAPI_add_event(EventSet, PAPI_TOT_INS) != PAPI_OK)
        exit(-1);

    if (PAPI_add_event(EventSet, PAPI_L1_DCM) != PAPI_OK)
        exit(-1);

    if (PAPI_start(EventSet) != PAPI_OK)
        exit(-1);

    if (PAPI_read(EventSet, values1) != PAPI_OK)
        exit(-1);

    for (i=0;i<10000;i++)
    sum+=i;

    if (PAPI_stop(EventSet, values2) != PAPI_OK)
        exit(-1);

    if (PAPI_cleanup_eventset(EventSet) != PAPI_OK)
        exit(-1);

    if (PAPI_destroy_eventset(&EventSet) != PAPI_OK)
        exit(-1);

    PAPI_shutdown();

    /* Get value */

    values[0]=values2[0]-values1[0];

    values[1]=values2[1]-values1[1];

    printf("TOT_INS:%lld\nL1_DCM: %lld\n",values[0], values[1]);

    return 0;

}
