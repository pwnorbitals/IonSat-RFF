FreeRTOS_dirname = 'FreeRTOSv10.3.1'
FreeRTOS_compiler = 'GCC'
FreeRTOS_port = 'Posix'
FreeRTOS_allocator = 'heap_3'

FreeRTOS_dir = 'FreeRTOS/'+FreeRTOS_dirname+'/FreeRTOS/Source/'

FreeRTOS_inc = include_directories(
    FreeRTOS_dir+'include/',
    FreeRTOS_dir+'portable/'+FreeRTOS_compiler+'/'+FreeRTOS_port+'/',
    'FreeRTOS/'+FreeRTOS_dirname,
    'FreeRTOS/ESFree/'
    )
FreeRTOS_src = files(
    FreeRTOS_dir+'croutine.c',
    FreeRTOS_dir+'event_groups.c',
    FreeRTOS_dir+'list.c',
    FreeRTOS_dir+'queue.c',
    FreeRTOS_dir+'stream_buffer.c',
    FreeRTOS_dir+'tasks.c',
    FreeRTOS_dir+'timers.c',
    FreeRTOS_dir+'portable/'+FreeRTOS_compiler+'/'+FreeRTOS_port+'/port.c',
    FreeRTOS_dir+'portable/MemMang/'+FreeRTOS_allocator+'.c',
    'FreeRTOS/ESFree/scheduler.c'
    )


source_files += FreeRTOS_src
incdir += FreeRTOS_inc
