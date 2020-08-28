#include "RFF.h"

RFF::Queue<int, 10> q;
RFF::Mutex check;

void qReceiver(void*) {
    int val;
    q.receive(val, portMAX_DELAY);

    assert(val == 42);

    check.give();

}


void rff_main() {
    

    auto qHandle = q.handle();
    assert(qHandle != 0);

    check.take(portMAX_DELAY);

    {
        RFF::Task recvTask{qReceiver};
        q.send(42, portMAX_DELAY);
        check.take(portMAX_DELAY);
    }

    {
        RFF::Task recvTask{qReceiver};
        q.sendToBack(42, portMAX_DELAY);
        check.take(portMAX_DELAY);
    }

    {
        RFF::Task recvTask{qReceiver};
        q.sendFromISR(42, nullptr);
        check.take(portMAX_DELAY);
    }

    {
        RFF::Task recvTask{qReceiver};
        q.sendToBackFromISR(42);
        check.take(portMAX_DELAY);
    }

    {
        RFF::Task recvTask{qReceiver};
        q.sendToFront(42);
        check.take(portMAX_DELAY);
    }

    {
        RFF::Task recvTask{qReceiver};
        q.sendToFrontFromISR(42, nullptr);
        check.take(portMAX_DELAY);
    }


    RFF::OSStop();
}