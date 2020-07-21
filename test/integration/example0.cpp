
#include "catch.hpp"

#define FFS_TEST

#include <iostream>
#include "FFS.h"


TEST_CASE( "typical use case", "[example]" ) {
    

    struct MyCustomEventType { int eventNo; };
    struct MyOtherCustomEventType { bool sent; };
    
    using event_tags = std::tuple<FFS::Tag<MyCustomEventType>, FFS::Tag<MyOtherCustomEventType>>;
    // using events = FFS::declare_events<myCustomEventType, MyOtherCustomEventType>

    
    auto handler = [](FFS::Event<MyCustomEventType> const& evt){  
        std::cout << "YAY ! got " << evt.data.eventNo << std::endl;
        REQUIRE(evt.data.eventNo == 42);
        
        evt.controller->emit(MyOtherCustomEventType{true});
    };
    
    
    
    auto otherhandler = [](FFS::Event<MyOtherCustomEventType> const& evt) { 
        std::cout << "boom !! that's" << evt.data.sent << std::endl;
        REQUIRE(evt.data.sent == true);
        
        FFS::OSStop();
    };
    
    
    

    
    auto handlers = std::make_tuple(FFS::QueuedEventHandler<MyCustomEventType, 2048, 64>{handler, "first", 1}, 
                                    FFS::TaskedEventHandler<MyOtherCustomEventType, 2048, 64>{otherhandler, "second", 2});

     
    auto modules = std::make_tuple(FFS::Module{std::move(handlers)});
    
    auto controller = FFS::Controller{FFS::OSSettings{}, std::make_tuple(FFS::Mode{"abc"}), std::move(modules), event_tags{}};

    controller.emit(MyCustomEventType{42});
    
    
    
    FFS::OSStart();
    
    std::cout << "OK" << std::endl;
    REQUIRE(1 == 1);
}

