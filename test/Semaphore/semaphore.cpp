#include "RFF.h"

RFF::Semaphore s{2, 0};
RFF::Mutex m;

void tester(void*) {

}

void rff_main() {

    assert(s.handle() != 0);

    assert(s.getCount() == 0);
    s.give();
    assert(s.getCount() == 1);
    s.take(portMAX_DELAY);
    assert(s.getCount() == 0);

    #if ENABLE_ISR_TEST
        assert(s.getCount() == 0);
        s.giveFromISR();
        assert(s.getCount() == 1);
        s.takeFromISR(portMAX_DELAY);
        assert(s.getCount() == 0);
    #endif

    assert(m.handle() != 0);
    assert(m.getCount() == 1);
    m.take(portMAX_DELAY);
    assert(m.getCount() == 0);

    s.give();
    assert(s.getCount() == 1);
    {
        RFF::SemaphoreHolder h{s};
        assert(s.getCount() == 0);
    }
    assert(s.getCount() == 1);





    RFF::OSStop();
}