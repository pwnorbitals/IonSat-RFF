
#define FFS_CROSS

#include "FFS.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>


struct BlinkEvent { };


void handler(BlinkEvent const& evt) {
    gpio_toggle(GPIOG, GPIO13);
    
}

void blinkTimer(void*) {
    int i;
    while(true) {
        for (i = 0; i < 500; i++) {	/* Wait a bit. */
			__asm__("nop");
		}
		FFS::emit(BlinkEvent{});
    }
}


FFS::EventHandler<BlinkEvent, 2, 16, 2048> h1{handler, "first"};
FFS::Module m1{h1};


FFS::Setup setup{std::make_tuple(FFS::Mode{"abc"}), m1};

FFS::Task<1, 512> t1{blinkTimer, "blinkTimer"};

void ffs_main() {
    
    rcc_periph_clock_enable(RCC_GPIOG); // Enable clock GPIOC
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13); // Configure GPIO13
}

