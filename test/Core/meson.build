core_test_name = core_library_name + 'Test'
core_test_sources = [
  'main.cpp',
  'CellTest.cpp',
  'ConstraintTypeTest.cpp',
  'DigitsTest.cpp',
  'GridUtilitiesTest.cpp',
  'PuzzleIntrinsicsTest.cpp',
  'PuzzleSpaceTest.cpp',
]

core_test = executable(core_test_name,
  cpp_args: compiler_options,
  sources: core_test_sources,
  dependencies: [doctest_dependency, core_dependency]
)
test(core_test_name, core_test)