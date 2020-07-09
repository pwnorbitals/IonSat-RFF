#pragma once

#include "FFS.h"

namespace FFS {
    class Mode {
    protected:
        std::string name;
        std::function<void (void) > onEnter;
        std::function<void (void) > onLeave;

    public:
        Mode(std::string _name) : name{_name} {};
        virtual ~Mode() {};
    };

}
