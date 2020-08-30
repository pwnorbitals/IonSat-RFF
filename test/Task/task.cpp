#include <iostream>
#include "RFF.h"

RFF::Mutex check;
unsigned int i = 0;

void tester1(void* in) {
    assert(((*((int*)in))) == 42);
    check.give();

    RFF::suspendCurrent();
}

void tester2(void*) {
    check.take(portMAX_DELAY);
    RFF::suspendCurrent();
}

void tester3(void*) {
    while(1) {
        i++;
    }
}

void tester4(void*) {
    RFF::delay(portMAX_DELAY);
    check.give();
    RFF::suspendCurrent();
}

void rff_main() {
    {

        check.take(portMAX_DELAY);

        {
            int test = 42;
            RFF::Task<configMAX_PRIORITIES-1, 20000> task{tester1, "test1", &test};
            assert(task.handle() != 0);
            check.take(portMAX_DELAY);
            
        }
        assert(check.getCount() == 0);
        {
            RFF::Task<1, 20000> task{tester2, "test2"};
            assert(task.priority() == 1);
            task.priority(2);
            assert(task.priority() == 2);
        }
        assert(check.getCount() == 0);
        i = 0;
        {
            RFF::Task<configMAX_PRIORITIES-1, 20000> task{tester3, "test3"};
            auto i_c1 = i;
            task.suspend();
            RFF::delay(1 * configTICK_RATE_HZ);
            auto i_c2 = i;
            task.resume();
            assert(i_c1 == i_c2);
        }
        {
            auto time1 = RFF::getTickCount();
            RFF::delay(1*configTICK_RATE_HZ);
            auto time2 = RFF::getTickCount();
            assert(time2 > time1+(0.98*configTICK_RATE_HZ));
            assert(time2 < time1+(1.02*configTICK_RATE_HZ));
        }
        {
            auto time1 = RFF::getTickCount();
            auto time1_mod = time1;
            // delayUntil modifies its first argument !
            RFF::delayUntil(time1_mod, 1*configTICK_RATE_HZ);
            auto time2 = RFF::getTickCount();
            assert(time2 >= time1+(0.98*configTICK_RATE_HZ));
            assert(time2 <= time1+(1.02*configTICK_RATE_HZ));
        }
        assert(check.getCount() == 0);
        {
            RFF::Task<configMAX_PRIORITIES-1, 20000> task{tester4, "test4"};
            RFF::delay(1*configTICK_RATE_HZ);
            std::cout << "aborting delay" << std::endl;
            task.abortDelay();
            std::cout << "done aborting delay delay" << std::endl;
            check.take(portMAX_DELAY);
            std::cout << "delay aborted confirmed" << std::endl;

        }

        
    }

    RFF::OSStop();
}   
