#include <iostream>

using namespace std;

int game_board[10][10];
//-8 - mine
//0-8 - cell
int user_game_board[10][10];
//-2 - close cell
//-1 - flag
//0-8 - opened

const int kRows = 9, kColumns = 9, kMinesCount = 10;

void print_game_board()
{
    system("cls");

    cout << " ";
    for (int i = 0; i < kColumns; i++) {
        if (i + 1 < 10) cout << " ";
        cout << " " << i + 1;
    }
    cout << endl;

    for (int i = 0; i < kRows; i++) {
        cout << i + 1 << " ";
        if (i + 1 < 10) cout << " ";

        for (int j = 0; j < kColumns; j++) {
            if (user_game_board[i][j] == -2) cout << "_  ";
            else if (user_game_board[i][j] == -1) cout << "F  ";
            else if (user_game_board[i][j] == -3) cout << "*  ";
            else cout << user_game_board[i][j] << "  ";
        }
        cout << endl;
    }
    cout << endl << endl;
}

void dfs(int x, int y)
{
    if (x < 1 || x >= kRows || y < 1 || y >= kColumns || user_game_board[x][y] > -2) return;

    user_game_board[x][y] = game_board[x][y];

    if (game_board[x][y] > 0) return;

    for (int x2 = x - 1; x2 <= x + 1; ++x2) {
        for (int y2 = y - 1; y2 <= y + 1; ++y2) {
            if (x2 != x || y2 != y) dfs(x2, y2);
        }
    }
}

bool open_cell(int x, int y)
{
    if (game_board[x][y] == -8) return false;

    if (game_board[x][y] > 0) {
        user_game_board[x][y] = game_board[x][y];
        return true;
    }

    dfs(x, y);
}

bool is_win()
{
    int opened_cell_count = 0;
    for (int i = 0; i < kRows; i++) {
        for (int j = 0; j < kColumns; j++) {
            if (user_game_board[i][j] >= 0) opened_cell_count++;
        }
    }

    return (kRows * kColumns - kMinesCount == opened_cell_count);
}

void end_game(bool is_win = false)
{
    for (int i = 0; i < kRows; i++) {
        for (int j = 0; j < kColumns; j++) {
            if (user_game_board[i][j] == -1) user_game_board[i][j] = -2;
            if (game_board[i][j] == -8) user_game_board[i][j] == -3;

        }
    }
    print_game_board();
    cout << (is_win ? "Congratulations - You WIN!" : "This is mine - You Lose!") << "\nIf you want start again enter any word";
    string str;
    cin >> str;
}

int main()
{
    while (true) {
        srand(time(0));
        //create game board
        for (int i = 0; i < kRows; i++) {
            for (int j = 0; j < kColumns; j++) {
                game_board[i][j] = 0;
                user_game_board[i][j] = -2;
            }
        }

        // generate mines on game board
        for (int i = 0; i < kMinesCount; i++) {
            while (true) {
                int x = rand() % kRows;
                int y = rand() % kColumns;

                if (game_board[x][y] != -8) {
                    game_board[x][y] = -8;
                    break;
                }
            }
        }

        //generte count for mines
        for (int i = 0; i < kRows; i++) {
            for (int j = 0; j < kColumns; j++) {
                if (game_board[i][j] != -8) {
                // game_board[i][j] = 0; //check this and clear later
                    for (int i2 = i - 1; i2 <= i + 1; ++i2) {
                        for (int j2 = j - 1; j2 <= j + 1; ++j2) {
                            if (
                                i2 >= 0 && i2 < kRows 
                                && j2 >= 0 && j2 < kColumns 
                                && game_board[i2][j2] == -8
                            ) ++game_board[i][j];
                        }
                    }
                }
            }
        }

        //output game board
        while (true) {
            print_game_board();
            cout << "Enter comand: ";
   
            string comand;
            cin >> comand;

            //o x y - open cell
            if (comand == "\\o") {
                int x, y;
                cin >> x >> y;
                --x;
                --y;
                if (x < 0 || x >= kRows || y < 0 || y >= kColumns || user_game_board[x][y] >= 0) continue;
                if (open_cell(x, y)) {
                    end_game();
                    break;
                }

                if (is_win()) {
                    end_game(true);
                    break;
                }
            }
            // \f x y - flag
            else if (comand == "\\f") {
                int x, y;
                cin >> x >> y;
                --x;
                --y;
                if (x < 0 || x >= kRows || y < 0 || y >= kColumns || user_game_board[x][y] >= 0) continue;

                if (user_game_board[x][y] == -1) user_game_board[x][y] = -2;
                else user_game_board[x][y] = -1;
            }
            //\n - restart game
            else if (comand == "\\n") {
                break;
            }
            // \q - quit game
            else if (comand == "\\q") {
                return 0;
            }
        }
    }
}
