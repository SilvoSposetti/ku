# Define project, language, license
project('ku', 
  'cpp',
  version : '1.0.0',
  license : 'GPL-3.0-or-later',
  default_options : ['cpp_std=c++20'])

# Add Defines
add_project_arguments('-DOUT_DIR="' + meson.source_root() +'/out"', language : 'cpp')

# Grab subprojects
subproject('doctest')
doctest_project = subproject('doctest')
doctest_dependency = doctest_project.get_variable('doctest_dep')

# Grab all sources and define includes
subdir('src/ku')
includes = include_directories('src/ku')

# Create library
ku_library = library(meson.project_name(),
  sources: ku_sources)

# Create executable sandbox and link it with library
subdir('src/sandbox')
executable('ku_sandbox',
  sources: sandbox_sources,
  include_directories : includes,
  link_with : ku_library)

# Create test executable and link library
subdir('test')
test_executable = executable('ku_test',
  sources: test_sources,
  include_directories : includes,
  link_with : ku_library,
  dependencies: doctest_dependency)
test('test', test_executable)
