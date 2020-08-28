#include "RFF.h"

RFF::Queue<int, 10> q1;
RFF::Queue<float, 10> q2;

RFF::QueueSet<20> qs;
RFF::Mutex check;

void tester(void*) {
    auto qh = qs.select(portMAX_DELAY);
    assert(qh == q1.handle());
    
    int val;
    q1.receive(val, portMAX_DELAY);
    check.give();

    RFF::suspendCurrent();
}

void rff_main() {
    qs.add(q1);
    qs.add(q2);
    check.take(portMAX_DELAY);

    {
    
        q1.send(10);
        RFF::Task<1> test{tester};
        check.take(portMAX_DELAY);

    }
    

    RFF::OSStop();
}