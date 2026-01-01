project('ku',
  'cpp',
  version : '1.0.0',
  license : 'GPL-3.0-or-later',
  default_options : ['cpp_std=c++23']
)

# GCC compiler options
compiler_options = [
  '-Werror', # Treats warning as errors
  '-Wall', # Enables all the warnings about constructions that some users consider questionable
  '-Wextra', # Enables some extra warning flags that are not enabled by -Wall
]

# Add defines
add_project_arguments('-DOUT_DIR="' + meson.project_source_root() +'/out"', language : 'cpp')

# External dependencies
subproject('doctest')
doctest_project = subproject('doctest')
doctest_dependency = doctest_project.get_variable('doctest_dep')

subdir('src')