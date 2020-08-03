
linker_scripts = meson.get_cross_property('linker_scripts', [])
startup_code = meson.get_cross_property('startup_code', '')
cross_folder = meson.get_cross_property('cross_folder', '')

if cross_folder != ''
    c_link_args += '-L'+cross_folder
    foreach script : linker_scripts
        c_link_args += '-T'+script
    endforeach

    if startup_code != ''
        source_files += files(cross_folder+startup_code)
    endif
endif
