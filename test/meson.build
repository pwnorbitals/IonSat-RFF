fs = import('fs')

# thread_dep = dependency('threads')
freertos_dep = dependency('freertos', fallback:['freertos', 'freertos_dep'])

incdir += include_directories('.')

test_files = []
subdirs = ['unit', 'phys', 'integration', 'emul']
test_master = files('test.cpp')

foreach sd : subdirs
    test_files = []
    subdir(sd)
    foreach tf : test_files
        name = tf.split('.')[0]
        file = files(sd+'/'+tf)
        ut = executable(name+'_unit_test', source_files + file, include_directories : incdir, dependencies: [freertos_dep])
        test(name+' unit test', ut)
    endforeach
endforeach



