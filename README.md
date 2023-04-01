# ku ![GitHub repo size](https://img.shields.io/github/repo-size/SilvoSposetti/ku?style=for-the-badge) ![GitHub](https://img.shields.io/github/license/SilvoSposetti/ku?style=for-the-badge)

A lightweight C++ library to generate, solve, and export Sudokus.

## Description
Handles generic constraints via the following steps:
1. Constraints are reduced from Sudoku to the Exact Cover.
2. The Exact Cover problem is solved with Algorithm X, by Donald Knuth.
3. Any Exact Cover solution found is reduced back to a Sudoku solution .

The Exact Cover problem may be built with both primary and secondary items, depending on the constraints selected, and in particular:
- _Primary_ items must be covered _exactly_ once.
- _Secondary_ items must be covered _at most_ once.

Generated Sudokus can be exported to svg.
The DLX-matrix used by Algorithm X can also be exported to svg for visual debugging.

### Available Constraints
These are the classic sudoku constraints that are supported:
- __Sudoku Cell__
- __Sudoku Row__
- __Sudoku Column__
- __Sudoku Box__

And this is a list of the non-classic constraints supported:
- __Anti-King__
- __Anti-King (Torus)__
- __Anti-Knight__
- __Anti-Knight (Torus)__
- __Asterisk__
- __Disjoint Boxes__
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

The project consists of the `ku` library, a `ku_sandbox` executable, and a `ku_test` executable.

### Dependencies
- C++20
- [Meson](https://mesonbuild.com/)
- [Doctest](https://github.com/doctest/doctest) (grabbed automatically by Meson as it's a WrapDB package)
