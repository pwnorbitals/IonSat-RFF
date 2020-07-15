test_names = ['controller.cpp', 'eventHandler.cpp', 'FreeRTOS.cpp', 'event.cpp', 'mode.cpp', 'module.cpp']
test_files = files(test_names)

foreach tf : test_names
    name = tf.split('.')[0]
    ut = executable(name+'_unit_test', source_files + files(tf), include_directories : incdir, dependencies: [thread_dep, freertos_dep, boost_dep])
    test(name+' unit test', ut)
endforeach

#ut = executable('unit_tests', source_files + test_files, include_directories : incdir, dependencies: [thread_dep, freertos_dep, boost_dep])
#test('unit tests', ut)
