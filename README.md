# ku
A fast and lightweight C++ project to generate, solve, and export Sudoku puzzles with varying constraints.

![BuildAndTest Status](https://img.shields.io/github/actions/workflow/status/SilvoSposetti/ku/BuildAndTest.yaml)

## Description
The library handles any supported constraint through the following steps:
1. Constraints are reduced from a Sudoku problem to an _Exact Covering with Colors_ problem (XCC).
2. The resulting problem is fed into an XCC solver heavily inspired by Donald Knuth's [SSXCC](https://cs.stanford.edu/~knuth/programs/ssxcc.w) program.
3. The solutions to the XCC problem found, if any, are then reduced back to a Sudoku solution.

The XCC problem may be built with both primary and secondary items, depending on the constraints provided by the Sudoku problem, for which the following applies:
- `Primary` items must be covered __exactly__ once.
- `Secondary` items must be covered __at most__ once.
  - Secondary items can have a `color` assigned to them. These colored items are allowed to be covered by one or more options, as long as the colors are compatible.

Generated Sudokus can be exported to svg, where each constraint provides its own way of being drawn on the board.
The matrix-likedata structure used by Algorithm C can also be exported to an svg file for visual debugging.

The project consists of:
- The `libku` library, used by the following executables
- A `ku_sandbox` executable, useful for experimenting
- A `ku_unit_test` executable
- A `ku_performance_test` executable (generated only for Release builds)

### Available Constraints
These are the classic sudoku constraints that are supported:
- `Sudoku Cell`
- `Sudoku Row`
- `Sudoku Column`
- `Sudoku Box`

The following are the non-classic constraints supported:
- `Anti-King`
- `Anti-King (Torus)`
- `Anti-Knight`
- `Anti-Knight (Torus)`
- `Asterisk`
- `Disjoint Boxes`
- `Hyper Sudoku`
- `Negative Diagonal`
- `Negative Diagonal Even`
- `Negative Diagonal Odd`
- `Positive Diagonal`
- `Positive Diagonal Even`
- `Positive Diagonal Odd`

## Getting Started
- The build system used by the project is [Meson](https://mesonbuild.com/), download and install it on your system.
- Make sure you have a `C++23`-capable compiler.

The `/bin` directory is gitignored, the easiest way to generate a `Debug` build is to run the following in the root of the repository:

```
meson setup bin/Debug
meson compile -C bin/Debug
```
Or, for a `Release` build:
```
meson setup --buildtype=release bin/Release
meson compile -C bin/Release
```

### Dependencies
- [Meson](https://mesonbuild.com/) `1.4.1+`
- [doctest](https://github.com/doctest/doctest) `2.4.11+`
