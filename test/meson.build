thread_dep = dependency('threads')
freertos_dep = dependency('freertos', fallback:['freertos', 'freertos_dep'])
boost_dep = dependency('boost')

incdir += include_directories('.')

subdir('unit')
subdir('phys')
subdir('integration')
subdir('emul')

