#include "RFF.h"


RFF::Queue<int, 10> q;
RFF::Mutex check;

void qReceiver(void*) {
    int val;
    q.receive(val, portMAX_DELAY);

    assert(val == 42);
    val = 0;

    check.give();

    RFF::suspendCurrent();

}

void qReceiver2(void*) {
    int val;

    #ifdef ENABLE_ISR_TEST
        q.receiveFromISR(val, nullptr);
        assert(val == 42);
        val = 0;
        check.give();
    #endif

    q.peek(val, portMAX_DELAY);
    assert(val == 42);

    #ifdef ENABLE_ISR_TEST
        q.peekFromISR(val);
        assert(val == 42);
    #endif

    assert(q.messagesWaiting() == 1);
    #ifdef ENABLE_ISR_TEST
        assert(q.messagesWaitingFromISR() == 1);
    #endif
    assert(q.spacesAvailable() == 9);

    q.receive(val, portMAX_DELAY);
    check.give();

    #ifdef ENABLE_ISR_TEST
        assert(q.isEmptyFromISR());
    #endif
    assert(q.messagesWaiting() == 0);

    for(unsigned i = 0; i < 10; i++) {
        q.send(42, portMAX_DELAY);
    }
    #ifdef ENABLE_ISR_TEST
        assert(q.isFullFromISR());
    #endif

    q.reset();

    #ifdef ENABLE_ISR_TEST
        assert(q.isEmptyFromISR());
    #endif
    assert(q.messagesWaiting() == 0);

    check.give();

    std::cout << "f" << std::endl;

    RFF::suspendCurrent();

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

    #ifdef ENABLE_ISR_TEST
    {
        RFF::Task recvTask{qReceiver};
        q.sendFromISR(42, nullptr);
        check.take(portMAX_DELAY);
    }
    #endif

    #ifdef ENABLE_ISR_TEST
    {
        RFF::Task recvTask{qReceiver};
        q.sendToBackFromISR(42);
        check.take(portMAX_DELAY);
    }
    #endif

    {
        RFF::Task recvTask{qReceiver};
        q.sendToFront(42);
        check.take(portMAX_DELAY);
    }

    #ifdef ENABLE_ISR_TEST
    {
        RFF::Task recvTask{qReceiver};
        q.sendToFrontFromISR(42, nullptr);
        check.take(portMAX_DELAY);
    }
    #endif

    {
        RFF::Task recvTask{qReceiver2};

        q.send(42, portMAX_DELAY);
        check.take(portMAX_DELAY);

        #ifdef ENABLE_ISR_TEST
            q.send(42, portMAX_DELAY);
        #endif
        check.take(portMAX_DELAY);
    }


    RFF::OSStop();
}