
#define RFF_TEST

#include <iostream>
#include <thread>
#include "RFF.h"

struct eventType { int myint; };

void hdlr(eventType* evt) {
    assert(evt->myint == 8);
    std::cout << "Ok" << std::endl;
    // RFF::OSStop();
    exit(0);
}

eventType evt{8};
RFF::Task<1, 20000> task{(void(*)(void*))hdlr, "test", &evt};

void rff_main() {
	

}
