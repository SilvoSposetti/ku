# Create all sub-libraries
subdir('RandomGenerator')
subdir('Utilities')
subdir('Core')
subdir('Solver')
subdir('Drawing')
subdir('Constraints')
subdir('Puzzle')

# Main library that aggregates other libraries
ku_dependency = declare_dependency(
  dependencies : [
    random_generator_dependency,
    utilities_dependency,
    core_dependency,
    solver_dependency,
    drawing_dependency,
    constraints_dependency,
    puzzle_dependency,
  ]
)

# Sandbox executable
subdir('Sandbox')
