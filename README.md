# ku ![GitHub repo size](https://img.shields.io/github/repo-size/SilvoSposetti/ku?style=for-the-badge) ![GitHub](https://img.shields.io/github/license/SilvoSposetti/ku?style=for-the-badge)

A lightweight C++ application to generate, solve, and export Sudokus.

## Description
Handles generic constraints which are reduced from Sudoku to Exact Cover. A constraint may define columns which are covered either "exactly once" (Primary Columns) or "at most once" (Secondary Columns).

The supported constraints are:
- __Sudoku Cell__ (classic Sudoku)
- __Sudoku Row__ (classic Sudoku)
- __Sudoku Column__ (classic Sudoku)
- __Sudoku Box__ (classic Sudoku)
- __King's Move__
- __King's Move (Torus)__
- __Negative Diagonal__
- __Negative Diagonal Even__
- __Negative Diagonal Odd__
- __Positive Diagonal__
- __Positive Diagonal Even__
- __Positive Diagonal Odd__


An export of the the DLX matrix or the Sudoku itself in SVG format is also possible

## Getting Started
Use CMake to configure and generate builds.

### Dependencies
- C++20
- CMake
