

#define FFS_CROSS

#include <iostream>
#include "FFS.h"
#include <csignal>
#include <cstdlib>
// #include <chrono>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>


struct BlinkEvent { int eventNo; };

void blinker(BlinkEvent const& evt) {
    gpio_toggle(GPIOG, GPIO13);
    
}



void timer(void*) {
    while(1) {
        for (int i = 0; i < 1000000; i++) {	/* Wait a bit. */
            __asm__("nop");
        }
        
        FFS::emit(BlinkEvent{});
    }
    
}


FFS::EventHandler<BlinkEvent, 1, 16, 2048> h1{blinker, "blinker"};
FFS::Module m1{h1};

FFS::Setup setup{std::make_tuple(FFS::Mode{"abc"}), m1};

FFS::Task<1, 512> t1{timer, "timer"};

void ffs_main() {
    rcc_periph_clock_enable(RCC_GPIOG); // Enable clock GPIOC
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13); // Configure GPIO13
}

