drawing_test_name = drawing_library_name + 'Test'
drawing_test_sources = [
  'main.cpp',
  'DrawingUtilitiesTest.cpp',
  'SvgDocumentTest.cpp',
  'SvgElementTest.cpp',
  'SvgSquigglyLineTest.cpp',
  'SvgZigZagLineTest.cpp',
]

drawing_test = executable(
  drawing_test_name,
  cpp_args: compiler_options,
  sources: drawing_test_sources,
  dependencies: [doctest_dependency, drawing_dependency]
)
test(drawing_test_name, drawing_test)