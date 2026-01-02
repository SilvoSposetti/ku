drawing_library_name = 'Drawing'
drawing_sources = [
  'DrawingOptions.cpp',
  'DrawingUtilities.cpp',
  'SvgCircle.cpp',
  'SvgDocument.cpp',
  'SvgElement.cpp',
  'SvgGroup.cpp',
  'SvgLine.cpp',
  'SvgPolyline.cpp',
  'SvgRect.cpp',
  'SvgSquigglyLine.cpp',
  'SvgText.cpp',
  'SvgZigZagLine.cpp',
]

drawing_library = static_library(drawing_library_name,
  cpp_args: compiler_options,
  sources: drawing_sources,
  dependencies: [utilities_dependency],
)

drawing_dependency = declare_dependency(
  link_with: drawing_library,
  include_directories: include_directories('.'),
  dependencies: [utilities_dependency],
)

