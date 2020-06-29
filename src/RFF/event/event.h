#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

namespace FFS {
    
    template<typename data_t>
    class Event {
        data_t data;
        char sha1[32];
        unsigned long emissionTime;
        unsigned char priority;
        
    public:
        Event(){};
        ~Event(){};
    };
    
}


#endif
