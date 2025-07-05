# Sudoku Solver (Console-Based Game)

A fully interactive Sudoku game built using **C++**, utilizing **STL containers**, **backtracking algorithms**, and **modular design**. The player can play Sudoku directly in the terminal, with real-time input validation, hints, undo functionality, and more.

## Features

- **Console-based interactive game**: Solve Sudoku puzzles directly in your terminal.
- **Undo Functionality**: Revert the last move using the `undo` command.
- **Hint System**: Get a valid next move suggestion using the `hint` command.
- **Reset**: Restart the current puzzle from its original state.
- **Auto-generated puzzles**: Each puzzle has a unique solution, generated using backtracking.
- **Fixed Cell Protection**: Pre-filled cells cannot be modified by the user.
- **Input Validation**: Ensures moves are within bounds and Sudoku rules.

## Technologies Used

- **C++**  
- **Standard Template Library (STL)**:  
  - `vector` – For board representation  
  - `stack` – For undo history  
  - `map` – To track fixed cells  
  - `set`, `function` – For additional logic and utilities  
- **DSA Concepts**:  
  - Backtracking (for puzzle generation and solving)  
  - Stack-based state saving (undo system)  
  - Grid traversal and constraint checking
