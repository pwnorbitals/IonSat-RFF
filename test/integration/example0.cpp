
#define FFS_TEST

#include <iostream>
#include "FFS.h"
#include <csignal>
#include <cstdlib>


struct MyCustomEventType { int eventNo; };
struct MyOtherCustomEventType { bool sent; };

struct SignalEvent { int signum; };
struct RandomEvent { int value; };

FFS::Controller* globalController = NULL;


void handler(FFS::Event<MyCustomEventType>* evt) {
    std::cout << "YAY ! got " << evt->data.eventNo << std::endl;
    assert(evt->data.eventNo == 42);
    evt->controller->emit(MyOtherCustomEventType{true});
    
}

void otherHandler(FFS::Event<MyOtherCustomEventType>* evt) {
    std::cout << "boom !! that's " << evt->data.sent << std::endl;
    assert(evt->data.sent == true);
}

void signalHandler(FFS::Event<SignalEvent>* evt) {
    std::cout << "Got event " << evt->data.signum << std::endl;
    if(evt->data.signum == SIGINT) {
        std::cout << "Quitting" << std::endl;
        FFS::OSStop();
    }
}

void randomHandler(FFS::Event<RandomEvent>* evt) {
    std::cout << "Random handler : " << evt->data.value << std::endl;
}

void randomGenerator(void*) {
    std::srand(std::time(nullptr));
    while(true) {
        int random = std::rand();
        std::cout << "random> " << random << std::endl;
        globalController->emit(RandomEvent{random});
        FFS::Task<64>::delay(1 * configTICK_RATE_HZ);
    }
}


void ffs_main() {
    
    auto h1 = FFS::QueuedEventHandler<MyCustomEventType, 100000, 64> {handler, "first", 1};
    auto h2 = FFS::TaskedEventHandler<MyOtherCustomEventType, 100000, 64> {otherHandler, "second", 2};
    auto m1 = FFS::Module{h1, h2};
    
    auto h3 = FFS::QueuedEventHandler<SignalEvent, 100000, 64>{signalHandler, "signal", 3};
    auto h4 = FFS::QueuedEventHandler<RandomEvent, 100000, 4>{randomHandler, "keyboard", 4};
    auto m2 = FFS::Module{h3, h4};

	auto controller = FFS::Controller{std::make_tuple(FFS::Mode{"abc"}), m1, m2};
    globalController = &controller;

    signal(SIGINT, [](int signum) {globalController->emit(SignalEvent{signum});});
	controller.emit(MyCustomEventType{42}); // Not yet interrupt-safe for TaskedEventHandler
    
    auto t1 = FFS::Task<512>{randomGenerator, "randomGenerator", 1};

	FFS::suspendCurrentTask();

}

