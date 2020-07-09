#include "catch.hpp"
#define FFS_TEST

#include <iostream>
#include <thread>
#include "FFS.h"


TEST_CASE("Task creation and deletion", "[FreeRTOS-tasks]") {
    
    
    
    int data = 8;
    std::function<void(void*)> hdlr = [] (void* mydata) { int* myintdata = reinterpret_cast<int*>(mydata); REQUIRE(*myintdata == 8); };
    auto controller = FFS::Controller{FFS::OSSettings{}, std::make_tuple(FFS::Mode{"test"}), std::make_tuple(), std::make_tuple()};
    auto task = FFS::Task<int, 20>{hdlr, "test", FFS::Event{data, &controller}, 1};
    std::this_thread::sleep_for(std::chrono::seconds{2});
    
    
    
}
