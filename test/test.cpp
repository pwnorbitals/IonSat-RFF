#include "RFF.h"


void timeout_fct(void*) {
    RFF::delay(5 * configTICK_RATE_HZ);
    exit(-1);

}

RFF::Task timeout{timeout_fct};