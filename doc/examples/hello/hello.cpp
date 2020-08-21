#include <iostream>
#include "RFF.h"

// Event we handle when we want to print something to screen
struct PrintMessage { std::string message; };
struct ExitMessage {};

// Event handler that prints stuff to screen
void printer(PrintMessage const& msg) {
    std::cout << msg.message << std::endl;
    RFF::OSStop();
}

// Define how the event handler behaves
RFF::EventHandler<PrintMessage> printHandler{printer};

// Group events in modules and setup RFF
RFF::Module printModule{printHandler};
RFF::Setup setup{printModule};

// Entry point
void rff_main() {
	RFF::emit(PrintMessage{"Hello world !"}); // Send the event to print 
}

