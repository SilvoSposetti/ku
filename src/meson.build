# Create all sub-libraries
subdir('RandomGenerator')
subdir('Utilities')

# Main library that aggregates other libraries
ku_dependency = declare_dependency(
  dependencies : [
    random_generator_dependency,
    utilities_dependency,
  ]
)

# Tests
subdir('test/Utilities')
subdir('test/RandomGenerator')

# Sandbox executable
subdir('Sandbox')
