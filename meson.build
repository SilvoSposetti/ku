project(
  'ku',
  'cpp',
  version: '1.0.0',
  license: 'GPL-3.0-or-later',
  meson_version: '>=1.3.2',
  default_options: {
    'cpp_std': 'c++23',
    'werror': true,  # Treat warnings as errors
    'warning_level': '3',  # GCC: [-Wall, -Wextra, -Wpedantic], MSVC: [/W4]
    'b_lto': true,  # Enable Link-Time-Optimizations
  },
)

# GCC compiler options
compiler_options = [
  # '-Wconversion',
  # '-Wshadow',
]

# Add defines
add_project_arguments('-DOUT_DIR="' + meson.project_source_root() + '/out"', language: 'cpp')

# External dependencies
subproject('doctest')
doctest_project = subproject('doctest')
doctest_dependency = doctest_project.get_variable('doctest_dep')

subdir('src')
subdir('test')
