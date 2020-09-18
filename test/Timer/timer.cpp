#include <iostream>
#include "RFF.h"

RFF::Mutex check;

void cb() {
    check.give();
}

void rff_main() {
    {
        check.take(portMAX_DELAY);
        auto t1 = RFF::InitTimer(1000, false, cb);
        t1.start();
        check.take(portMAX_DELAY);

    }

}