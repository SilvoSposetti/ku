# ku
A C++ library to generate, solve, and export grid-like puzzles of numbers with varying constraints.

![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/SilvoSposetti/ku/BuildAndTest.yaml?branch=main&style=for-the-badge&logo=GitHub)

## Description
The library handles supported puzzle constraints as follows:
1. Constraints are reduced from a puzzle problem to an _Exact Covering with Colors_ (XCC) problem.
2. The resulting problem is fed into an XCC solver originally described by [Donald E. Knuth](https://www-cs-faculty.stanford.edu/~knuth/) on his website ([original source](https://cs.stanford.edu/~knuth/programs/ssxcc.w)) and adapted here to C++. Also instrumental was his early-access draft publication of [Pre-Fascicle 7A](https://www-cs-faculty.stanford.edu/~knuth/fasc7a.ps.gz): section 7.2.2.3, see `Dancing cells`.
3. If any solutions to the XCC problem are found, they are reduced back to a solution of the original puzzle problem.

The XCC problem is constructed with `primary` and `secondary` items for which the following applies:
- `Primary` items must be covered __exactly__ once.
- `Secondary` items must be covered __at most__ once.
  - Secondary items inside an option may have a `color` assigned to them.
    Colored items are allowed to be covered by more than one option as long as the color they describe is the same.

Generated puzzles may be printed to standard output or exported as SVG.
A visual representation of the matrix-like data structure fed to the solver can also be exported to SVG for visual debugging.

The project consists of several small libraries combined to produce:
- A `ku_sandbox` executable, useful for experimenting.
- A set of `*Test` executables, one for each internal library.
- A `PerformanceTest` executable, generated only for optimized (`release`) builds.

### Available Constraints
Basic constraints that can be used to generate or solve classic Sudoku:
- `Cell`
- `Exact Row`
- `Exact Column`
- `Exact 3x3 Boxes`

Additional constraints:
- `Asterisk`
- `Disjoint Boxes`
- `Exact Negative Diagonal`
- `Exact Positive Diagonal`
- `Hyper Sudoku`
- `King Pattern`
- `King Torus Pattern`
- `Knight Pattern`
- `Knight Torus Pattern`
- `Negative Diagonal Even`
- `Negative Diagonal Odd`
- `Positive Diagonal Even`
- `Positive Diagonal Odd`

## Getting Started
This project has been developed and tested on Linux using GCC 14+.
Other platforms and compilers may work, but they are not officially supported.

- The build system used by the project is [Meson](https://mesonbuild.com/), download and install it on your system.
  Meson will also retrieve `doctest` for you.
- Make sure you have a `C++23`-capable compiler.

- The `/bin` directory is gitignored.
  After cloning, the easiest way to generate a `debug` build is:
  ```bash
  meson setup bin/Debug
  meson compile -C bin/Debug
  ```
  And for a `release` build:
  ```bash
  meson setup --buildtype=release bin/Release
  meson compile -C bin/Release
  ```

## Dependencies

### Build requirements
- [Meson](https://mesonbuild.com/) `>=1.3.2`: Build system used to configure and compile the project.
- [Ninja](https://ninja-build.org/): Backend build tool used by Meson.

### Testing
- [doctest](https://github.com/doctest/doctest) `2.4.12`: Used as development dependency for unit testing and performance testing.

## License
This project is licensed under `GPL-3.0`. See `LICENSE.md` for details.

