project(
  'FSW',
  ['c', 'cpp'],
  default_options : ['cpp_std=gnu++17', 'c_std=gnu11', 'warning_level=3']
)

add_project_link_arguments('--verbose', '--cref', language: ['c', 'cpp'])

deps = []
minimal_deps = []
link_args = []
comp_args = []
source_files = []
target_specific_sources = []
incdir = []
linker_deps = []


subdir('src')
subdir('test')

