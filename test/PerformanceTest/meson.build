performance_test_name = 'PerformanceTest'
performance_test_sources = files(
  'ClassicSudokuBaseTest.cpp',
  'KuTestArguments.cpp',
  'SingleConstraintTest.cpp',
  'main.cpp',
)

# Create PerformanceTest executable and link library only if building in release
if get_option('buildtype') == 'release'
  performance_test_executable = executable(
    performance_test_name,
    cpp_args: compiler_options,
    sources: performance_test_sources,
    dependencies: [
      doctest_dependency,
      ku_dependency,
    ],
  )
  test(performance_test_name, performance_test_executable)
endif
