puzzle_test_name = puzzle_library_name + 'Test'
puzzle_test_sources = [
  'main.cpp',
  'PuzzleTest.cpp',
  'SudokuSamplesTest.cpp',
]

puzzle_test = executable(puzzle_test_name,
  cpp_args: compiler_options,
  sources: puzzle_test_sources,
  dependencies: [doctest_dependency, puzzle_dependency]
)
test(puzzle_test_name, puzzle_test)