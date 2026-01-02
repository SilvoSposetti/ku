random_generator_library_name = 'RandomGenerator'
random_generator_sources = [
  'RandomGenerator.cpp',
]

random_generator_library = static_library(random_generator_library_name,
  cpp_args: compiler_options,
  sources: random_generator_sources,
)

random_generator_dependency = declare_dependency(
  link_with: random_generator_library,
  include_directories: include_directories('.')
)

