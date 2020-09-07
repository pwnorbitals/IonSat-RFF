# Description

This is the most basic RFF Hello World project !

# Steps to build
``` 
meson build
meson wrap promote subprojects/RFF/subprojects/freertos.wrap
meson build
cd build
ninja
``` 

# Steps to run
in the build directory :
``` 
./hello-world
``` 
