#include <iostream>
#include "RFF.h"

RFF::Mutex check;

void tester1(void* in) {
    assert(((*((int*)in))) == 42);
    check.give();

    std::cout << "suspending" << std::endl;

    RFF::suspendCurrent();
}

void rff_main() {
    {
        check.take(portMAX_DELAY);
        int test = 42;
        RFF::Task<1, 20000> task{tester1, "test", &test};
        check.take(portMAX_DELAY);

        assert(task.priority() == 1);
        task.priority(2);
        assert(task.priority() == 2);

        
    }

    RFF::OSStop();
}   
