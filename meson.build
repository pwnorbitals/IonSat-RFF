project(
  'FSW',
  ['c', 'cpp'],
  default_options : ['cpp_std=c++17', 'c_std=c11', 'warning_level=3']
)

if meson.is_cross_build()
    add_global_arguments('-DSTM32F4', language : ['c', 'cpp'])
endif

deps = []
link_args = []
comp_args = []
source_files = []
incdir = []
linker_deps = []
incdir += include_directories('subprojects/libopencm3/include')
subdir('cross-compile')
subdir('src')
subdir('test')
