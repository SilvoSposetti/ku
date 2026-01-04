utilities_library_name = 'Utilities'
utilities_sources = [
  'FileIo.cpp',
  'StringUtilities.cpp',
  'TemporaryDirectory.cpp',
  'Timer.cpp',
]

utilities_dependencies = [
  random_generator_dependency,
]

utilities_library = static_library(utilities_library_name,
  cpp_args: compiler_options,
  sources: utilities_sources,
  dependencies: utilities_dependencies
)

utilities_dependency = declare_dependency(
  link_with: utilities_library,
  include_directories: include_directories('.'),
  dependencies: utilities_dependencies
)

