#include <iostream>
#include <thread>
#include "RFF.h"


// Event we handle when we want to print something to screen
struct ToggleMessage { };

// Event handler that toggles a value
void toggler(ToggleMessage const& msg) {
    static auto isOn = false;
    
    isOn = !isOn;
    std::cout << "Output is now " << isOn;
    
}

// Task that triggers an event at a given interval
void toggleTimer(void*) {
    std::this_thread::sleep_for(std::chrono::seconds{1});
    RFF::emit(ToggleMessage{});
} 

// Define how the event handler behaves
RFF::EventHandler<ToggleMessage> toggleHandler{toggler};

// Define the timer task
RFF::Task toggleTask{toggleTimer, "ToggleTimer"};

// Group events in modules and setup RFF
RFF::Module blinkerModule{toggleHandler};
RFF::Setup setup{blinkerModule};


void rff_main() { }
