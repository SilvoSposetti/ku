# Define project, language, license
project('ku', 
  'cpp',
  version : '1.0.0',
  license : 'GPL-3.0-or-later',
  default_options : ['cpp_std=c++23'])

# Default GCC compiler options
compilerFlags = [
  '-Werror', # Treats warning as errors
  '-Wall', # Enables all the warnings about constructions that some users consider questionable
  '-Wextra', # Enables some extra warning flags that are not enabled by -Wall
]

# Add Defines
add_project_arguments('-DOUT_DIR="' + meson.project_source_root() +'/out"', language : 'cpp')

# Grab subprojects
subproject('doctest')
doctest_project = subproject('doctest')
doctest_dependency = doctest_project.get_variable('doctest_dep')

# Grab all sources and define includes
subdir('src/ku')
includes = include_directories('src/ku')

# Create library
ku_library = library(meson.project_name(),
  cpp_args: compilerFlags,
  sources: ku_sources)

# Create executable sandbox and link it with library
subdir('src/sandbox')
executable('ku_sandbox',
  cpp_args: compilerFlags,
  sources: sandbox_sources,
  include_directories : includes,
  link_with : ku_library)

# Create unit test executable and link library
subdir('test/unitTest')
unit_test_executable = executable('ku_unit_test',
  cpp_args: compilerFlags,
  sources: unit_test_sources,
  include_directories : includes,
  link_with : ku_library,
  dependencies: doctest_dependency)

test('unit_test', unit_test_executable)

# Create performance test executable and link library only if building in release
if get_option('buildtype') == 'release'
  subdir('test/performanceTest')
    performance_test_executable = executable('ku_performance_test',
      cpp_args: compilerFlags,
      sources: performance_test_sources,
      include_directories : includes,
      link_with : ku_library,
      dependencies: doctest_dependency)
  test('performance_test', performance_test_executable)
endif

