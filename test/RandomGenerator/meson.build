random_generator_test_name = random_generator_library_name + 'Test'
random_generator_test_sources = [
  'main.cpp',
  'RandomGeneratorTest.cpp',
]

random_generator_test = executable(
  random_generator_test_name,
  cpp_args: compiler_options,
  sources: random_generator_test_sources,
  dependencies: [doctest_dependency, random_generator_dependency]
)
test(random_generator_test_name, random_generator_test)