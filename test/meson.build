fs = import('fs')

# thread_dep = dependency('threads')


incdir += include_directories('.')

subdirs = ['Controller', 'EventHandler', 'Module', 'Queue', 'QueueSet', 'Semaphore', 'Task']

foreach sd : subdirs
    test_files = []
    subdir(sd)
    
    foreach tf : test_files
        name = tf.split('.')[0]
        file = files(meson.current_source_dir()+'/'+sd+'/'+tf)
        ut = executable(
            name+'_'+sd+'_test', 
            source_files + file, 
            include_directories : incdir, 
            dependencies: deps, 
            link_args: link_args,               # TODO : MUST CHANGE THIS
            cpp_args: comp_args + ['-DRFF_TEST'], # TODO : MUST CHANGE THIS  
            c_args: comp_args + ['-DRFF_TEST'],   # TODO : MUST CHANGE THIS
            link_depends: linker_deps 
        )
        test(name+' unit test', ut)
    endforeach
    
    
endforeach



