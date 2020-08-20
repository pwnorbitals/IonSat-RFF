#include <iostream>
#include "RFF.h"

// Event we handle when we want to print something to screen
struct PrintMessage { std::string message; };
struct ExitMessage {};

// Event handler that prints stuff to screen
void printer(PrintMessage const& msg) {
    std::cout << msg.message << std::endl;
    RFF::emit(ExitMessage{});
}

void exiter(ExitMessage const& msg) { exit(0); }

// Define how the event handlers behave
RFF::EventHandler<PrintMessage> printHandler{printer, "printer"};
RFF::EventHandler<ExitMessage> exitHandler{exiter, "exiter"};

// Group events in modules and setup RFF
RFF::Module printModule{printHandler, exitHandler};
RFF::Setup setup{printModule};

// Entry point
void RFF_main() {
	RFF::emit(PrintMessage{"Hello world !"}); // Send the event to print 
}

