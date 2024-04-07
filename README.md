# ku 
A lightweight C++ library to generate, solve, and export Sudoku puzzles with varying constraints.

![BuildAndTest Status](https://img.shields.io/github/actions/workflow/status/SilvoSposetti/ku/BuildAndTest.yaml)

## Description
The library handles any supported constraint through the following steps:
1. Constraints are reduced from Sudoku to the Exact Cover.
2. The Exact Cover problem is solved with an implementation of Donald Knuth's Algorithm X.
3. The solutions to the exact cover problem found, if any, are reduced back to a Sudoku solution.

The Exact Cover problem may be built with both primary and secondary items, depending on the constraints provided by the Sudoku problem, and in particular:
- __Primary__ items must be covered __exactly__ once.
- __Secondary__ items must be covered __at most__ once.

Generated Sudokus can be exported to svg, where each constraint provides its own way of being drawn on the board.
The matrix-likedata structure used by Algorithm X can also be exported to svg for visual debugging.

The project consists of:
- A `ku` library
- A `ku_sandbox` executable
- A `ku_unit_test` executable
- And a `ku_performance_test` executable (Enabled only for Release builds) 

### Available Constraints
These are the classic sudoku constraints that are supported:
- __Sudoku Cell__
- __Sudoku Row__
- __Sudoku Column__
- __Sudoku Box__

The following are the non-classic constraints supported:
- __Anti-King__
- __Anti-King (Torus)__
- __Anti-Knight__
- __Anti-Knight (Torus)__
- __Asterisk__
- __Disjoint Boxes__
- __Hyper Sudoku__
- __Negative Diagonal__
- __Negative Diagonal Even__
- __Negative Diagonal Odd__
- __Positive Diagonal__
- __Positive Diagonal Even__
- __Positive Diagonal Odd__

## Getting Started
This project uses the Meson build system to configure and generate builds.
Since the `bin` directory is gitignored, the easiest way to generate debug and release builds is to run the following commands in the cloned repository:
```
meson setup bin/Debug
```
```
meson setup --buildtype=release bin/Release
```

### Dependencies
- C++20
- [Meson](https://mesonbuild.com/)
- [doctest](https://github.com/doctest/doctest)
