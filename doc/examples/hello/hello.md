# Description

This is the most basic RFF Hello World project !

# Steps
``` 
meson build
meson wrap promote subprojects/RFF/subprojects/freertos.wrap
meson build
cd build
ninja
``` 