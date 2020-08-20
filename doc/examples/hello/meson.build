project('RFF-hello', ['c', 'cpp'], default_options : ['cpp_std=c++17', 'c_std=c11'])

RFF = subproject('RFF')
RFF_dep = RFF.get_variable('rff_dep')

executable('hello-world', files('hello.cpp'), dependencies: RFF_dep)