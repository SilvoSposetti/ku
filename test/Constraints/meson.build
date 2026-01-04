constraints_test_name = constraints_library_name + 'Test'
constraints_test_sources = [
  'main.cpp',
  'AsteriskSudokuConstraintTest.cpp',
  'CellConstraintTest.cpp',
  'ConstraintTest.cpp',
  'ConstraintTraitsTest.cpp',
  'Disjoint3x3BoxesSudokuConstraintTest.cpp',
  'Exact3x3BoxesConstraintTest.cpp',
  'ExactColumnConstraintTest.cpp',
  'ExactNegativeDiagonalConstraintTest.cpp',
  'ExactPositiveDiagonalConstraintTest.cpp',
  'ExactRowConstraintTest.cpp',
  'HyperSudokuConstraintTest.cpp',
  'KingPatternConstraintTest.cpp',
  'KingTorusPatternConstraintTest.cpp',
  'KnightPatternConstraintTest.cpp',
  'KnightTorusPatternConstraintTest.cpp',
  'NegativeDiagonalEvenConstraintTest.cpp',
  'NegativeDiagonalOddConstraintTest.cpp',
  'NeighborPatternUtilitiesTest.cpp',
  'PositiveDiagonalEvenConstraintTest.cpp',
  'PositiveDiagonalOddConstraintTest.cpp',
]

constraints_test = executable(constraints_test_name,
  cpp_args: compiler_options,
  sources: constraints_test_sources,
  dependencies: [doctest_dependency, constraints_dependency]
)
test(constraints_test_name, constraints_test)