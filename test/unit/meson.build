test_files = ['controller.cpp', 'eventHandler.cpp', 'FreeRTOS.cpp', 'event.cpp', 'mode.cpp', 'module.cpp']

foreach tf : test_files
    name = tf.split('.')[0]
    ut = executable(name+'_unit_test', source_files + files(tf), include_directories : incdir, dependencies: [thread_dep, freertos_dep, boost_dep])
    test(name+' unit test', ut)
endforeach
