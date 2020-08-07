test_files = ['example0.cpp', 'blink.cpp']

if meson.is_cross_build()
    _tf = 'blink_raw.cpp'
    _name = _tf.split('.')[0]
    _file = _tf
    _tobuild = [files(meson.source_root()+'/cross-compile/stm32/syscalls.c'), files(meson.source_root()+'/cross-compile/stm32/sysmem.c'), files(_file)]
    _ut = executable(
        _name+'_test', 
        _tobuild, 
        include_directories : incdir, 
        dependencies:  [libopencm3], 
        link_args: link_args, 
        cpp_args: comp_args,                    
        c_args: comp_args, 
        link_depends: linker_deps
    )
    test(_name+' unit test', _ut)
endif
