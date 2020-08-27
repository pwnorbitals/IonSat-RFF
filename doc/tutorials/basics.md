# Structure of a program

First, you need to include the master RFF header file :
```
#include "RFF.h"
``` 

Then, as global variables, declare your RFF event handlers and modules and setup the RFF. 
Using global-scope variables helps with creating better embeddable binaries, as those variables aren't allocated on the stack at runtime.

Finally, and only if needed, you can create an initialization function that will be called at startup :
```
void rff_main() {
    // your code here 
}
```

# Event handlers

To declare an event handler, two parts are needed :

1) A handling function. The handling function's prototype is the following : `void handlingFunctionName (MessageType const& MessageName)`.
2) An event handler object, which refers to the handling function.

The event handler object hides all the thread magic that makes event handlers work. Internally, they are implemented with a handler thread that serially handles the events. If needed, implementing a parallel event handling is trivial.

You will need to declare your event handlers as follows :
```
RFF::EventHandler<MessageType> HandlerObjectName{handlerFunctionName};
```

Additional customization options are available for event handlers :

1) Priority : higher is executed first, default is 0
2) Queue length : maximum number of waiting events for the serial handling of events, default 16
3) Stack depth : the stack size for the event handling thread in number of words, default is 2*configMINIMAL_STACK_SIZE

For example, an event handler with a high-priority, a large-queue, and a large stack depth can be declared as follows :
```
RFF::EventHandler<MessageType, 10, 64, 512000> HandlerObjectName{handlerFunctionName};
```



# Modules

For now, modules are a simple syntactic sugar that help with grouping event handlers and dispatching messages. You can declare a module as follows:
``` 
RFF::Module MyFirstModule{firstHandler, secondHandler, thirdHandler};
```

You can group as many handlers as you want in a single module.

# Setting-up the RFF

RFF Setup cannot be simpler, you just need to create a Setup object and give it your modules :

```
RFF::Setup setup{firstModule, secondModule, thirdModule};
```

You can give as many modules as you want to the setup object. Internally, this creates the necessary internal variables and starts the OS.



# Compilation basics

Create your project by writing a `meson.build` file which contains :
``` 
project('project-name', ['c', 'cpp'], default_options : ['cpp_std=c++17', 'c_std=c11'])

RFF = subproject('RFF')
RFF_dep = RFF.get_variable('rff_dep')

exec_obc = executable('my-executable', files('main.cpp'), dependencies: RFF_dep)
``` 
If needed, you can refer to the Meson documentation to get a better overview of the features of `meson.build` files.

Then, create a `subprojects` folder and copy the `RFF.wrap` file from one of the example projects. 
To build the project, run :
``` 
meson build # this creates and initializes the build directory
cd build    # move in the build directory
ninja       # this runs the build step
```

To run the binary, you can use this command from your build directory
```
./project-name
```

# Running RFF tests

Move to the RFF subproject directory :
```
cd subprojects/RFF
```
Create a build directory :
```
meson build
cd build
```
Run the tests :

``` 
meson test
```

# Writing tests for your application

When redacting your `meson.build` file, create your test executable then declare your tests as :
```
test_exec_obj('my-test', files('test.cpp'), dependencies: RFF_dep)
test('my very cool test', test_exec_obj)
```

The Catch2 test framework will soon be supported !