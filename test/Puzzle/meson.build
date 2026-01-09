puzzle_test_name = puzzle_library_name + 'Test'
puzzle_test_sources = files(
  'PuzzleTest.cpp',
  'SudokuSamplesTest.cpp',
  'main.cpp',
)

puzzle_test = executable(
  puzzle_test_name,
  cpp_args: compiler_options,
  sources: puzzle_test_sources,
  dependencies: [
    doctest_dependency,
    puzzle_dependency,
  ],
)
test(puzzle_test_name, puzzle_test)
