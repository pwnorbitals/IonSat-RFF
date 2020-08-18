
linker_script = meson.get_cross_property('linker_script', '')
linker_depend = meson.get_cross_property('linker_depend', [])
startup_code = meson.get_cross_property('startup_code', '')
cross_folder = meson.get_cross_property('cross_folder', '')
has_meson_build = meson.get_cross_property('has_meson_build', '')

if cross_folder != ''
    
    link_args += '-Lcross-compile/'+cross_folder+'/'
    incdir += include_directories(cross_folder)   
    
    
    if linker_script != ''
        link_args += '-T'+meson.source_root()+'/cross-compile/'+cross_folder+'/'+linker_script
        foreach ld : linker_depend
            link_args += '-L'+meson.source_root()+'/cross-compile/'+cross_folder
        endforeach
    endif

    if startup_code != ''
        source_files += files(cross_folder+'/'+startup_code)
    endif
    
    foreach ld : linker_depend
        linker_deps += meson.source_root()+'/cross-compile/'+cross_folder+'/'+ld
    endforeach
    
    if has_meson_build == true
        subdir(cross_folder)
    endif

endif
