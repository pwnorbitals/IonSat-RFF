# IonSat-RFF
This repository hosts the RFF (Realtime Flight Framework), developped for french Ecole Polytechnique's student space center and its cubesat IonSat. It was created as a framework to help with realtime embeddedded development by wrapping the RTOS and a custom event-driven library into a simple C++ library. It is powered by the Meson build system generator for ease of use. 

![Polytechnique logo](https://upload.wikimedia.org/wikipedia/commons/thumb/0/06/%C3%89cole_polytechnique_signature.svg/170px-%C3%89cole_polytechnique_signature.svg.png) 

## Getting started

The full documentation (tutorials, reference documentation, examples and design philosophy) can be found as Markdown files in the *doc* library. As a suggested learning path, you should start by looking at the tutorials and a few example project before delving in the design philosophy and full reference documentation. 


## Tutorials

1) [Basics](tutorials/basics.md)
2) [Compilation and cross-compilation](tutorials/compilation.md)


## Examples

1) [Hello world !](examples/hello/hello.md)
2) [Blinker](examples/blinker/blinker.md)
3) [Synchronization](examples/synchronization/synchronization.md)
4) [Cross-compilation](examples/cross-compilations/cross-compilation.md)

## Design

1) [Architecture](design/architecture.md)
2) [RTOS wrapper](design/RTOS.md)
3) [Event handlers](design/event-handlers.md)
4) [Modules](design/modules.md)
5) [Testing](design/testing.md)

## Reference

[Full reference documentation](reference/full.md)