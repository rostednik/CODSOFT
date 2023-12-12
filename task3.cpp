#include <iostream>
using namespace std;

class TicTacToe {
private:
    char board[3][3];
    char currentPlayer;
    bool gameover;

public:
    TicTacToe() {
        currentPlayer = 'X';
        gameover = false;
        initializeBoard();
    }

    void initializeBoard() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                board[i][j] = ' ';
            }
        }
    }
    void displayBoard() {
        cout << "-------------" << endl;
        for (int i = 0; i < 3; ++i) {
            cout << "| ";
            for (int j = 0; j < 3; ++j) {
                cout << board[i][j] << " | ";
            }
            cout << endl << "-------------" << endl;
        }
    }

    bool checkWin(char player) {
        for (int i = 0; i < 3; ++i) {
            if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
                (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
                return true;
            }
        }
        if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
            (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
            return true;
        }
        return false;
    }
    bool checkDraw() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    void playGame() {
        cout << "TIC-TAC-TOE" << endl << endl;
        do {
            displayBoard();

            int row, col;
            cout << "\nPlayer " << currentPlayer << ", enter your move (row [1-3] and column [1-3]): ";
            cin >> row >> col;

            row--; col--;
            if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
                board[row][col] = currentPlayer;

                if (checkWin(currentPlayer)) {
                    displayBoard();
                    cout << "\nPlayer " << currentPlayer << " wins!" << endl;
                    gameover = true;
                } else if (checkDraw()) {
                    displayBoard();
                    cout << "\nIt's a draw!" << endl;
                    gameover = true;
                } else {
                    switchPlayer();
                }
            } else {
                cout << "Invalid move! Try again." << endl;
            }
        } while (!gameover);

        char playAgain;
        cout << "\nDo you want to play again? (y/n): ";
        cin >> playAgain;
        if (playAgain == 'y' || playAgain == 'Y') {
            gameover = false;
            initializeBoard();
            currentPlayer = 'X';
            playGame();
        } else {
            cout << "\nThank you for playing TIC-TAC-TOE!" << endl;
            cout << endl;
        }
    }
};

int main() {
    TicTacToe game;
    game.playGame();
    return 0;
}