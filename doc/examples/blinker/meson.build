project('RFF-blinker', ['c', 'cpp'], default_options : ['cpp_std=c++17', 'c_std=c11'])

RFF = subproject('RFF')
RFF_dep = RFF.get_variable('rff_dep')

executable('blinker', files('blinker.cpp'), dependencies: RFF_dep)