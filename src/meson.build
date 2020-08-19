incdir += include_directories('RFF')
source_files += files('RFF/OS/FreeRTOS/FFS_FreeRTOS.cpp')
message('b')
freertos = subproject('freertos')
deps += freertos.get_variable('freertos_dep')
# incdir += include_directories(meson.source_root()+'subprojects/freertos/include')

rff_dep = declare_dependency(dependencies: deps, include_directories: incdir, sources: source_files)
