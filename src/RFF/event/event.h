#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED



namespace FFS {    
    template<typename data_t>
    class Event {
    public:
        data_t data;
        char sha1[32];
        unsigned long emissionTime;
        
        Event(data_t _data) : data{_data}{};
        ~Event(){};
    };
    
}


#endif
