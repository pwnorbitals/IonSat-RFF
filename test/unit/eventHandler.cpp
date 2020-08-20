#define RFF_TEST

#include <iostream>
#include "RFF.h"


void rff_main() {
    struct eventType { int myint; };

    auto hdlr = [](eventType const& mydata) {
        assert(mydata.myint == 8);
        std::cout << "Ok" << std::endl;
        // RFF::OSStop();
        exit(0);
    };



    auto evt = eventType{8};

    
    auto evtHandler = RFF::EventHandler<eventType, 1> {hdlr, "first"};
    evtHandler(std::move(evt));
    RFF::suspendCurrentTask();
}


/*
TEST_CASE( "Tasked event handler" , "[EventHandler]" ){
    struct eventType{ int myint; };

    auto hdlr = [] (RFF::Event<eventType> const& mydata) {
        REQUIRE(mydata.data.myint == 8);

        RFF::OSStop();
    };



    auto evt = RFF::Event{eventType{8}};



    auto evtHandler = RFF::TaskedEventHandler<eventType, 1024, 64>{hdlr, "first", 1};
    evtHandler(evt);
    RFF::OSStart();
}
*/
