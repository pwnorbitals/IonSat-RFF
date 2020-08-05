
#define FFS_CROSS

#include <iostream>
#include "FFS.h"
#include <csignal>
#include <cstdlib>
// #include <chrono>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>


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
        // FFS::OSStop();
    }
}

void randomHandler(RandomEvent const& evt) {
    gpio_toggle(GPIOG, GPIO13);
    std::cout << "Random handler : " << evt.value << std::endl;
}

void randomGenerator(void*) {
    // std::srand(std::time(nullptr));
    std::srand(0);
    // auto start = std::chrono::steady_clock::now();
    while(true) {
        /*
        auto now =  std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > 5000) {
            std::cout << "Ok" << std::endl;
            // FFS::OSStop();
        }
        */
        int random = std::rand();
        std::cout << "random> " << random << std::endl;
        FFS::emit(RandomEvent{random});
        FFS::Task<64>::delay(1 * configTICK_RATE_HZ);
    }
}


FFS::EventHandler<MyCustomEventType, 1, 16, 2048> h1{handler, "first"};
FFS::EventHandler<MyOtherCustomEventType, 2, 16, 2048> h2{otherHandler, "second"};
FFS::Module m1{h1, h2};


FFS::EventHandler<SignalEvent, 3, 16, 2048> h3{signalHandler, "signal"};
FFS::EventHandler<RandomEvent, 4, 16, 2048> h4{randomHandler, "keyboard"};
FFS::Module m2{h3, h4};

FFS::Setup setup{std::make_tuple(FFS::Mode{"abc"}), m1, m2};

FFS::Task<1, 512> t1{randomGenerator, "randomGenerator"};

void ffs_main() {
    
    rcc_periph_clock_enable(RCC_GPIOG); // Enable clock GPIOC
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13); // Configure GPIO13
    
    signal(SIGINT, [](int signum) {FFS::emit(SignalEvent{signum});});
	FFS::emit(MyCustomEventType{42});
}

