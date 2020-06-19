#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

namespace FFS {
    
    template<typename data_t>
    classs Event {
        data_t data;
        char sha1[32];
        unsigned long emissionTime;
        
    public:
        Event();
        ~Event();
    }
    
}


#endif
