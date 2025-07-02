#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <map>
#include <functional>

using namespace std;

const int SIZE = 9;

class SudokuGame {
    vector<vector<int>> board;
    vector<vector<int>> original;
    map<pair<int, int>, bool> fixedCells;
    stack<vector<vector<int>>> history;

public:
    SudokuGame() {
        board = vector<vector<int>>(SIZE, vector<int>(SIZE, 0));
        original = board;
        generateBoard();
    }

    void generateBoard();
    void printBoard();
    bool isValid(int row, int col, int num);
    bool solveSudoku(vector<vector<int>>& grid);
    void play();
    void giveHint();
};

bool SudokuGame::isValid(int row, int col, int num) {
    for (int i = 0; i < SIZE; i++) {
        if (board[row][i] == num || board[i][col] == num) return false;
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[startRow + i][startCol + j] == num) return false;
        }
    }
    return true;
}

bool SudokuGame::solveSudoku(vector<vector<int>>& grid) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    // Local validity check on grid copy
                    bool valid = true;
                    for (int i = 0; i < SIZE; i++) {
                        if (grid[row][i] == num || grid[i][col] == num) valid = false;
                    }
                    int startRow = row - row % 3;
                    int startCol = col - col % 3;
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            if (grid[startRow + i][startCol + j] == num) valid = false;
                        }
                    }
                    if (!valid) continue;

                    grid[row][col] = num;
                    if (solveSudoku(grid)) return true;
                    grid[row][col] = 0;
                }
                return false;
            }
        }
    }
    return true;
}

void SudokuGame::generateBoard() {
    srand(time(0));

    // Generate a full valid board using backtracking
    solveSudoku(board);

    // Make a copy of the fully solved board
    vector<vector<int>> solved = board;

    // Start removing cells while ensuring the puzzle is still uniquely solvable
    int removed = 0;
    while (removed < 51) { // Leave ~30–35 clues
        int r = rand() % SIZE;
        int c = rand() % SIZE;
        if (board[r][c] == 0) continue;

        int backup = board[r][c];
        board[r][c] = 0;

        // Make a copy and check if solution is still unique
        vector<vector<int>> copy = board;
        int solutions = 0;

        function<bool(int, int)> countSolutions = [&](int row, int col) -> bool {
            if (row == SIZE) {
                solutions++;
                return solutions > 1; // Stop if more than one solution
            }

            int nextRow = (col == SIZE - 1) ? row + 1 : row;
            int nextCol = (col + 1) % SIZE;

            if (copy[row][col] != 0) return countSolutions(nextRow, nextCol);

            for (int num = 1; num <= 9; num++) {
                bool valid = true;
                for (int i = 0; i < SIZE; i++) {
                    if (copy[row][i] == num || copy[i][col] == num) valid = false;
                }
                int startRow = row - row % 3;
                int startCol = col - col % 3;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (copy[startRow + i][startCol + j] == num) valid = false;
                    }
                }
                if (!valid) continue;

                copy[row][col] = num;
                if (countSolutions(nextRow, nextCol)) return true;
                copy[row][col] = 0;
            }
            return false;
        };

        solutions = 0;
        countSolutions(0, 0);

        if (solutions != 1) {
            board[r][c] = backup; // Restore if multiple solutions
        } else {
            removed++;
        }
    }

    // Mark fixed cells
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j] != 0)
                fixedCells[{i, j}] = true;

    original = board;
}

void SudokuGame::printBoard() {
    cout << "\n    Sudoku Board\n";
    for (int i = 0; i < SIZE; ++i) {
        if (i % 3 == 0) cout << " -------------------------\n";
        for (int j = 0; j < SIZE; ++j) {
            if (j % 3 == 0) cout << " |";
            if (board[i][j] == 0) cout << " .";
            else cout << " " << board[i][j];
        }
        cout << " |\n";
    }
    cout << " -------------------------\n";
}

void SudokuGame::giveHint() {
    vector<vector<int>> solved = board;
    solveSudoku(solved);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                cout << "Hint: At (" << i + 1 << "," << j + 1 << ") place " << solved[i][j] << endl;
                return;
            }
        }
    }
    cout << "Board already complete!\n";
}
void SudokuGame::play() {
    int row, col, val;
    string command;
    while (true) {
        printBoard();
        cout << "Enter move as 'row col value' (1-9), or type 'hint', 'reset', 'undo', 'exit': ";
        cin >> command;

        if (command == "hint") {
            giveHint();
        } else if (command == "reset") {
            board = original;
            cout << "Board reset.\n";
        } else if (command == "undo") {
            if (!history.empty()) {
                board = history.top();
                history.pop();
                cout << "↩️  Last move undone.\n";
            } else {
                cout << "⚠️  Nothing to undo.\n";
            }
        } else if (command == "exit") {
            break;
        } else {
            try {
                row = stoi(command); // command stores row
                cin >> col >> val;
            } catch (...) {
                cout << "Invalid input format!\n";
                cin.ignore();
                continue;
            }

            row--; col--;

            if (row < 0 || col < 0 || row >= SIZE || col >= SIZE || val < 1 || val > 9) {
                cout << "Invalid input range!\n";
                continue;
            }

            if (fixedCells[{row, col}]) {
                cout << "You can't change a fixed cell!\n";
                continue;
            }

            if (!isValid(row, col, val)) {
                cout << "This move is not valid.\n";
                continue;
            }

            history.push(board); // Save current state for undo
            board[row][col] = val;
        }

        // Check completion
        bool done = true;
        for (auto& row : board)
            for (auto& cell : row)
                if (cell == 0)
                    done = false;

        if (done) {
            printBoard();
            cout << "\n🎉 Congratulations! You completed the Sudoku!\n";
            break;
        }
    }
}

int main() {
    SudokuGame game;
    game.play();
    return 0;
}
