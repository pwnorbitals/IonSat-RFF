Note : this tutorial has been redacted with Meson v0.53.2. Newer updates might break the guide.

# Compiling as a subproject

Compiling the RFF to be used as a Meson subproject is effectively straightforward. First, you need to create a *subprojects/* directory in which you can create your wrap file. For example, you can call it *RFF.wrap* :

``` 
[wrap-git]
url = https://github.com/astronautix/IonSat-RFF.git
revision = head
```

Then in your *meson.build* file, you need to import the subproject :

```
RFF = subproject('RFF')
deps += RFF.get_variable('rff_dep')
```

Because the RFF itself uses subprojects, and Meson doesn't support automatic handling of nested subprojects, you will need to manually promote the RFF's dependencies. This is explained when you run `meson build`, and looks like this : `meson wrap promote subprojects/RFF/subprojects/freertos.wrap`. The easiest way to do it is to copy-paste the command in the error when you try building at first !

# Adding subprojects

In your *subprojects/* directory, you can add as many wrap files as you need, and import them just like you did with the RFF. If needed, you will find further guidance in the [dedicated meson documentation](https://mesonbuild.com/Subprojects.html).

# Cross-compiling

Cross-compilation for embedded targets need a few components :

- a cross-file, that will guide Meson through the cross-compilation process
- a linker script, that explains what is the output format you need for your target
- some additional infrastructure to conditionnaly import libraries you will need to interact with the target

## Cross files

WIP

## Linker scripts

WIP

## Custom libraries

WIP