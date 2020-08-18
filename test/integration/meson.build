test_files = ['example0.cpp']


_tf = 'blink_standalone.cpp'
_name = _tf.split('.')[0]
_file = _tf
_tobuild = [target_specific_sources, files(_file)]
_ut = executable(
    _name+'_test', 
    _tobuild, 
    include_directories : incdir, 
    dependencies:  minimal_deps, 
    link_args: link_args, 
    cpp_args: comp_args,                    
    c_args: comp_args, 
    link_depends: linker_deps
)
test(_name+' unit test', _ut)

_tf = 'blink.cpp'
_name = _tf.split('.')[0]
_file = _tf
_ut = executable(
    _name+'_test', 
    source_files + files(_file), 
    include_directories : incdir, 
    dependencies:  minimal_deps + deps, 
    link_args: link_args, 
    cpp_args: comp_args,                    
    c_args: comp_args, 
    link_depends: linker_deps
)
test(_name+' unit test', _ut)
