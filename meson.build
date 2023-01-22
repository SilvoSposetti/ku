# Define project, language, license
project('ku', 
  'cpp',
  version : '1.0.0',
  license : 'GPL-3.0-or-later',
  default_options : ['cpp_std=c++20'])

# Add Defines
add_project_arguments('-DOUT_DIR="' + meson.source_root() +'/out"', language : 'cpp')

# Grab all sources
subdir('src')

executable('ku', ku_sources)
