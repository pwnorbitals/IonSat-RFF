
libopencm3 = meson.get_compiler('c').find_library('opencm3_stm32f4', dirs:[meson.source_root()+'/subprojects/libopencm3/lib'])
minimal_deps += libopencm3

# that's messed up, but meson refuses to let me use meson.source_root() to build include dirs
incdir += include_directories('../../subprojects/libopencm3/include') 

add_global_arguments('-DSTM32F4', language : ['c', 'cpp'])

target_specific_sources = files('syscalls.c', 'sysmem.c')
source_files += target_specific_sources
