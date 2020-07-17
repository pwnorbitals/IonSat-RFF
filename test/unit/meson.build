test_names = ['controller.cpp', 'eventHandler.cpp', 'task.cpp', 'event.cpp', 'mode.cpp', 'module.cpp']
test_master = files('../test.cpp')
test_files = files(test_names)

foreach tf : test_names
    #tf = 'eventHandler.cpp'
    name = tf.split('.')[0]
    ut = executable(name+'_unit_test', source_files + files(tf) + test_master, include_directories : incdir, dependencies: [thread_dep, freertos_dep, boost_dep])
    test(name+' unit test', ut)
endforeach

#ut = executable('unit_tests', source_files + test_files + test_master, include_directories : incdir, dependencies: [thread_dep, freertos_dep, boost_dep])
#test('unit tests', ut)
