#ifndef EVENTHANDLER_H_INCLUDED
#define EVENTHANDLER_H_INCLUDED

namespace FFS {
    enum deadline_policy {
        KILL, EMIT, IGNORE
    }

    template<typename evt_t>
    class eventHandler {
        
        // UINT MAX is 4294967295, which in ms is equivalent to 49,7 days.
        // We should probably have a static check for this.
        unsigned int deadlineMs;
        unsigned int WCETMs;
        
        
        deadline_policy wcetPolicy;
        deadline_policy deadlinePolicy;
        FFS::Task handlerTask;
        FFS::Event handledEvent;
        
        
        eventHandler() {
            
        }
        ~EventHandler() {
            
        }
    }
}




#endif
