#include "catch.hpp"

#define FFS_TEST

#include <iostream>
#include "FFS.h"



TEST_CASE("creation", "[EventHandler]") {
    
    std::function<void(void*)> hdlr = [] (void* mydata) { 
        auto* evt = static_cast<FFS::Event<int>*>(mydata); 
        REQUIRE(evt->data == 8);
        evt->controller->stop();
    };    
    struct eventType{ int data; };
    auto evtHandler = FFS::EventHandler<eventType, 1000000>{hdlr, "first", 1};
    
    auto controller = FFS::Controller(FFS::OSSettings{}, std::make_tuple(FFS::Mode{"test"}), std::make_tuple(), std::make_tuple(FFS::Tag<int>{}));
    auto evt = FFS::Event(eventType{8}, &controller);
    
    evtHandler(evt);
    
    controller.start();
    
}
