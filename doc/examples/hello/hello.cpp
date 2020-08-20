#include <iostream>
#include "FFS.h"

// Event we handle when we want to print something to screen
struct PrintMessage { std::string message };
struct ExitMessage {};

// Event handler that prints stuff to screen
void printer(PrintMessage const& msg) {
    std::cout << msg.message << std::endl;
    FFS::emit(ExitMessage{})
}

void exiter(ExitMessage const& msg) { exit(0); }

// Define how the event handlers behave
FFS::EventHandler<PrintMessage, 1, 16, 2048> printHandler{printer, "printer"};
FFS::EventHandler<PrintMessage, 1, 16, 2048> exitHandler{exiter, "exiter"};

// Group events in modules and setup RFF
FFS::Module printModule{printHandler, exitHandler};
FFS::Setup setup{{}, printModule};

// Entry point
void ffs_main() {
	FFS::emit(PrintMessage{"Hello world !"}); // Send the event to print 
}

