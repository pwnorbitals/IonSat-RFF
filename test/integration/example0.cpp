
#include "catch.hpp"

#include <iostream>
#include "FFS.h"

/*
TEST_CASE( "typical use case", "[example]" ) {

    struct MyCustomEventType { int eventNo; };
    struct MyOtherCustomEventType { bool sent; };
    
    using event_tags = std::tuple<FFS::Tag<MyCustomEventType>, FFS::Tag<MyOtherCustomEventType>>;

    auto handler = [](FFS::Event<MyCustomEventType>* evt){  
        std::cout << "YAY ! got " << evt->data.eventNo << std::endl;
        evt->controller->emit(MyOtherCustomEventType{true});
    };
    
    auto otherhandler = [](FFS::Event<MyOtherCustomEventType>* evt) { 
        std::cout << "boom !!" << std::endl;
        std::cout << evt->data.sent << std::endl;
    };

    auto handlers = std::make_tuple(FFS::EventHandler<MyCustomEventType, 1000000>{handler, "first", 1}, 
                                    FFS::EventHandler<MyOtherCustomEventType, 1000000>{otherhandler, "second", 2});
    auto modules = std::make_tuple(FFS::Module{std::move(handlers)});
    auto controller = FFS::Controller{FFS::OSSettings{}, std::make_tuple(FFS::Mode{"abc"}), std::move(modules), event_tags{}};

    controller.emit(MyCustomEventType{42});
    
    controller.start();
    
    
    REQUIRE( 1 == 1 );
}
*/
