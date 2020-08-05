fs = import('fs')

deps = []
# thread_dep = dependency('threads')
deps += dependency('freertos', fallback:['freertos', 'freertos_dep'])
if meson.is_cross_build()
    cc = meson.get_compiler('c')
    libopencm3 = cc.find_library('opencm3_stm32f4', dirs:[meson.source_root()+'/subprojects/libopencm3/lib'])
    deps += libopencm3
endif

incdir += include_directories('.')

test_files = []
subdirs = ['unit', 'phys', 'integration', 'emul']
test_master = files('test.cpp')

message(link_args)
message(comp_args)
message(linker_deps)
foreach sd : subdirs
    test_files = []
    subdir(sd)
    foreach tf : test_files
        name = tf.split('.')[0]
        file = files(meson.source_root()+'/test/'+sd+'/'+tf)
        ut = executable(
            name+'_'+sd+'_test', 
            source_files + file, 
            include_directories : incdir, 
            dependencies: deps, 
            link_args: link_args, 
            cpp_args: comp_args,                    
            c_args: comp_args, 
            link_depends: linker_deps
        )
        test(name+' unit test', ut)
    endforeach
endforeach



