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
        
        Event(data_t const& _data, Controller* _ctrlr) : data{_data}, controller{_ctrlr}{};
        ~Event(){};
        
        bool operator==(Event const& other) const { 
            
            // does not require any comparison operator for user-provided event types !
            return other.emissionTime == this->emissionTime && other.sha1 == this->sha1;
            
        }
    };
    
}


#endif
