#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <iostream>
#include "FFS.h"



TEST_CASE( "typical use case", "[example]" ) {
    
    /*
    
    struct testEvent{int evnum;};
    
    auto handler = [](FFS::Event<testEvent>* evt) { 
        
        std::cout << evt->data.evnum << std::endl; 
        // need to tell that I'm done ?!
    };
    auto eventHandler = FFS::EventHandler<testEvent>{handler, "testHandler", 256};
    auto module = FFS::Module{std::make_tuple(eventHandler)};
    auto modes = std::make_tuple(FFS::Mode{"test"});
    auto controller = FFS::Controller{FFS::OSSettings{}, modes, std::make_tuple(module)};
    
    controller.emit(testEvent{2});
    
    */
    
    struct MyCustomEventType { int eventNo; };
    struct MyOtherCustomEventType { bool sent; };
    
    using event_tags = std::tuple<FFS::Tag<MyCustomEventType>, FFS::Tag<MyOtherCustomEventType>>;

    auto handler = [](FFS::Event<MyCustomEventType>* evt){  
        std::cout << evt->data.eventNo << std::endl;
        evt->controller->emit(MyOtherCustomEventType{true});
    };
    
    auto otherhandler = [](FFS::Event<MyOtherCustomEventType>* evt) { 
        std::cout << "boom !!" << std::endl;
        std::cout << evt->data.sent << std::endl;
    };

    auto handlers = std::make_tuple(FFS::EventHandler<MyCustomEventType, 100000000>{handler, "first"}, 
                                    FFS::EventHandler<MyOtherCustomEventType, 100000000>{otherhandler, "second"});
    auto modules = std::make_tuple(FFS::Module{handlers});
    auto controller = FFS::Controller{FFS::OSSettings{}, std::make_tuple(FFS::Mode{"abc"}), modules, event_tags{}};

    controller.emit(MyCustomEventType{42});
    
    controller.start();
    
    
    REQUIRE( 1 == 1 );
}
