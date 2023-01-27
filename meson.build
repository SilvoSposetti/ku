# Define project, language, license
project('ku', 
  'cpp',
  version : '1.0.0',
  license : 'GPL-3.0-or-later',
  default_options : ['cpp_std=c++20'])

# Add Defines
add_project_arguments('-DOUT_DIR="' + meson.source_root() +'/out"', language : 'cpp')

# Grab all sources and define includes
subdir('src')
includes = include_directories('src/ku/')

# Create library
ku_library = library(meson.project_name(), ku_sources)

# Create executable sandbox and link it with library
executable('ku_sandbox',
 'src/sandbox/main.cpp',
  include_directories : includes,
  link_with : ku_library)

# Create test executable and link library
test_executable = executable('ku_test',
 'test/main.cpp',
  include_directories : includes,
  link_with : ku_library)
test('test', test_executable)
