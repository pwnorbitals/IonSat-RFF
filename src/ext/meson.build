port_path = 'port_GCC_posix'
FreeRTOS_allocator = 'heap_3'


FreeRTOS_inc = include_directories(
    'FreeRTOS/include/',
    port_path,
    'ESFree/'
    )
FreeRTOS_src = files(
    'FreeRTOS/croutine.c',
    #'FreeRTOS/event_groups.c',
    'FreeRTOS/list.c',
    'FreeRTOS/queue.c',
    #'FreeRTOS/stream_buffer.c',
    'FreeRTOS/tasks.c',
    'FreeRTOS/timers.c',
    'FreeRTOS/portable/MemMang/'+FreeRTOS_allocator+'.c',
    port_path+'/port.c',
    'ESFree/scheduler.c'
    )


source_files += FreeRTOS_src
incdir += FreeRTOS_inc
incdir += include_directories('headers')
