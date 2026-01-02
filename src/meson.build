# Create all sub-libraries
subdir('RandomGenerator')
subdir('Utilities')
subdir('Core')
subdir('Solver')
subdir('Drawing')
subdir('Constraints')

# Main library that aggregates other libraries
ku_dependency = declare_dependency(
  dependencies : [
    random_generator_dependency,
    utilities_dependency,
    core_dependency,
    solver_dependency,
    drawing_dependency,
    constraints_dependency,
  ]
)

# Tests
subdir('test/Utilities')
subdir('test/RandomGenerator')
subdir('test/Core')
subdir('test/Solver')
subdir('test/Drawing')
subdir('test/Constraints')

# Sandbox executable
subdir('Sandbox')
