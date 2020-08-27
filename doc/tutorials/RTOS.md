# RTOS Wrapper

As a comfort feature, the RFF has been designed to wrap any RTOS in user-friendly C++ classes and namespaces. This comes from the realization that most of the RTOS are now written in plain C which has poor interoperability and usability when it comes to modern object-oriented software design. 

# FreeRTOS

When it comes to using the wrappped FreeRTOS, the most important and useful features are the synchronization primitives, namely `RFF::Mutex` and `RFF::Semaphore`/`RFF::SemaphoreHolder`. Aside from this, access is given to the other RTOS features : `RFF::Queue`/`RFF::QueueSet` and `RFF::Task`. Those two last sets of features shouldn't really be needed when using the event-driven RFF features on embedded targets, but can be useful for demonstration purposes or for solving complex and intricate real-time problems.

`RFF::Mutex` and `RFF::Semaphore` are actually two sides of the same coin. 

# Others ?

In the future, expanding the list of wrapped real-time OSes to let the user make a free choice and support more architectures sounds like an important objective to pursue. Interesting integrations could be RTEMS, µC/OS-II and III, and possibly non-free commercial aerospace RTOS to pursue qualification objectives (DO-178C ?)