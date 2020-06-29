source_files += files('unit/controller.cpp')
thread_dep = dependency('threads')
ut = executable('unit_test', source_files, include_directories : incdir, dependencies: thread_dep)

test('unit test', ut)
