source_files += files('test.cpp')
subdir('unit')
subdir('phys')
subdir('integration')
subdir('emul')

incdir += include_directories('.')



thread_dep = dependency('threads')
freertos_dep = dependency('freertos', fallback:['freertos', 'freertos_dep'])
boost_dep = dependency('boost')

ut = executable('unit_test', source_files, include_directories : incdir, dependencies: [thread_dep, freertos_dep, boost_dep])
test('unit test', ut)
