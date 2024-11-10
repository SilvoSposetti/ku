# ku 
A lightweight C++ library to generate, solve, and export Sudoku puzzles with varying constraints.

![BuildAndTest Status](https://img.shields.io/github/actions/workflow/status/SilvoSposetti/ku/BuildAndTest.yaml)

## Description
The library handles any supported constraint through the following steps:
1. Constraints are reduced from a Sudoku problem to an _Exact Covering with Colors_ problem (XCC).
2. The resulting problem is fed into an XCC solver heavily inspired by Donald Knuth's [SSXCC](https://cs.stanford.edu/~knuth/programs/ssxcc.w) program.
3. The solutions to the XCC problem found, if any, are then reduced back to a Sudoku solution.

The XCC problem may be built with both primary and secondary items, depending on the constraints provided by the Sudoku problem, for which the following applies:
- __Primary__ items must be covered __exactly__ once.
- __Secondary__ items must be covered __at most__ once.
- __Secondary__ items can have a __color__ assigned to them. These colored items are allowed to be
covered by many different options, as long as the colors are compatible.

Generated Sudokus can be exported to svg, where each constraint provides its own way of being drawn on the board.
The matrix-likedata structure used by Algorithm C can also be exported to svg for visual debugging.

The project consists of:
- The `ku` library
- A `ku_sandbox` executable, used to experiment
- A `ku_unit_test` executable
- A `ku_performance_test` executable (generated only for Release builds)

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
- Then C++ build system used is [Meson](https://mesonbuild.com/), download and install it on your system.
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
