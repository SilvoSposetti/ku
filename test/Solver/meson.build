solver_test_name = solver_library_name + 'Test'
solver_test_sources = files(
  'AlgorithmCTest.cpp',
  'DancingCellsStructureTest.cpp',
  'main.cpp',
)

solver_test = executable(
  solver_test_name,
  cpp_args: compiler_options,
  sources: solver_test_sources,
  dependencies: [
    doctest_dependency,
    solver_dependency,
  ],
)
test(solver_test_name, solver_test)
