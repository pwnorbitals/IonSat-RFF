source_files += files('unit/controller.cpp')
thread_dep = dependency('threads')
freertos_dep = dependency('freertos', fallback:['freertos', 'freertos_dep'])
boost_dep = dependency

ut = executable('unit_test', source_files, include_directories : incdir, dependencies: [thread_dep, freertos_dep, boost_dep])
test('unit test', ut)
