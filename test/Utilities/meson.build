utilities_test_name = utilities_library_name + 'Test'
utilities_test_sources = [
  'main.cpp',
  'ArrayUtilitiesTest.cpp',
  'FileIoTest.cpp',
  'FixedCapacityArrayTest.cpp',
  'IdPackingTest.cpp',
  'MathUtilitiesTest.cpp',
  'TemporaryDirectoryTest.cpp',
  'TimerTest.cpp',
]

utilities_test = executable(utilities_test_name,
  cpp_args: compiler_options,
  sources: utilities_test_sources,
  dependencies: [doctest_dependency, utilities_dependency])

test(utilities_test_name, utilities_test)