
#define FFS_TEST

#include <iostream>
#include "FFS.h"
#include <csignal>
#include <cstdlib>
#include <chrono> 


struct MyCustomEventType { int eventNo; };
struct MyOtherCustomEventType { bool sent; };

struct SignalEvent { int signum; };
struct RandomEvent { int value; };

FFS::Controller* globalController = NULL;


void handler(MyCustomEventType const& evt) {
    std::cout << "YAY ! got " << evt.eventNo << std::endl;
    assert(evt.eventNo == 42);
    FFS::emit(MyOtherCustomEventType{true});
    
}

void otherHandler(MyOtherCustomEventType const& evt) {
    std::cout << "boom !! that's " << evt.sent << std::endl;
    assert(evt.sent == true);
}

void signalHandler(SignalEvent const& evt) {
    std::cout << "Got event " << evt.signum << std::endl;
    if(evt.signum == SIGINT) {
        std::cout << "Quitting" << std::endl;
        FFS::OSStop();
    }
}

void randomHandler(RandomEvent const& evt) {
    std::cout << "Random handler : " << evt.value << std::endl;
}

void randomGenerator(void*) {
    std::srand(std::time(nullptr));
    auto start = std::chrono::steady_clock::now();
    while(true) {
        auto now =  std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > 5000) {
            std::cout << "Ok" << std::endl;
            FFS::OSStop();
        }
        int random = std::rand();
        std::cout << "random> " << random << std::endl;
        FFS::emit(RandomEvent{random});
        FFS::Task<64>::delay(1 * configTICK_RATE_HZ);
    }
}


FFS::EventHandler<MyCustomEventType, 1, 64, 100000> h1{handler, "first"};
FFS::EventHandler<MyOtherCustomEventType, 2, 64, 100000> h2{otherHandler, "second"};
FFS::Module m1{h1, h2};


FFS::EventHandler<SignalEvent, 3, 64, 100000> h3{signalHandler, "signal"};
FFS::EventHandler<RandomEvent, 4, 64, 100000> h4{randomHandler, "keyboard"};
FFS::Module m2{h3, h4};

FFS::Setup setup{std::make_tuple(FFS::Mode{"abc"}), m1, m2};

FFS::Task<1, 512> t1{randomGenerator, "randomGenerator"};

void ffs_main() {
    signal(SIGINT, [](int signum) {FFS::emit(SignalEvent{signum});});
	FFS::emit(MyCustomEventType{42});
}

