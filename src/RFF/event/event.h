#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED



namespace FFS {    
    class Controller;
    
    template<typename data_t>
    class Event {
    public:
        data_t data;
        char sha1[32];
        unsigned long emissionTime;
        Controller* controller = nullptr;
        
        Event(data_t _data, Controller* _ctrlr) : data{_data}, controller{_ctrlr}{};
        ~Event(){};
    };
    
}


#endif
