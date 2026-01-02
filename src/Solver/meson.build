solver_library_name = 'Solver'
solver_sources = [
  'AlgorithmC.cpp',
  'DancingCellsStructure.cpp',
  'ItemData.cpp',
  'OptionData.cpp',
  'XccElement.cpp',
]

solver_dependencies = [
  core_dependency,
]

solver_library = static_library(solver_library_name,
  cpp_args: compiler_options,
  sources: solver_sources,
  dependencies: solver_dependencies,
)

solver_dependency = declare_dependency(
  link_with: solver_library,
  include_directories: include_directories('.'),
  dependencies: solver_dependencies,
)

