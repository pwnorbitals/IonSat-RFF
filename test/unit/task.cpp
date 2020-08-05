
#define FFS_TEST

#include <iostream>
#include <thread>
#include "FFS.h"

struct eventType { int myint; };

void hdlr(eventType* evt) {
    assert(evt->myint == 8);
    std::cout << "Ok" << std::endl;
    FFS::OSStop();
}

eventType evt{8};
FFS::Task<1, 20000> task{(void(*)(void*))hdlr, "test", &evt};

void ffs_main() {
	

}
