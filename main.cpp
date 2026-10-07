#include <iostream>
using namespace std;

const int boardW = 72, boardH = 24;
int iteration = 0, itrSinceEdit = 0, ptrX = 0, ptrY = 0;
bool wrapAround = true;
bool board[boardH][boardW] = {0};
bool boardNext[boardW * boardH] = {0};

short checkNeighborNoWa(int w, int h) {
    short c = 0;
    // check 3x3 area of targeted cell
    for (int i = -1; i <= 1; i++) { for (int j = -1; j <= 1; j++) {
        // check left/right board edge
        if (w + j < 0 || w + j >= boardW) {
            continue;
        }
        // check top/bot board edge
        if (h + i < 0 || h + i >= boardH) {
            continue;
        }
        c += board[h + i][w + j];
    }}

    // remove current cell's state from total
    c -= board[h][w];
    return c;
}

short checkNeighborWa(int w, int h) {
    short c = 0;
    // check 3x3 area of targeted cell
    for (int i = -1; i <= 1; i++) { for (int j = -1; j <= 1; j++) {
        // board index formula:
        // (boardLength + targetCellIdx + tileShift) mod boardLength
        // applies for both tile width & height
        // tileShift can be -1, 0, or 1
        c += board[(boardH + h + i) % boardH][(boardW + w + j) % boardW];
    }}
    // remove current cell's state from total
    c -= board[h][w];
    return c;
}

short checkNeighbor(int w, int h) {
    if (wrapAround) {
        return checkNeighborWa(w, h);
    } else {
        return checkNeighborNoWa(w, h);
    }
}

void step() {
    // make new board data
    for (int i = 0; i < boardH; i++) { for (int j = 0; j < boardW; j++) {
        short cn = checkNeighbor(j, i);
        bool set;
        if (board[i][j]) {
            set = cn == 2 || cn == 3;
        } else {
            set = cn == 3;
        }
        boardNext[i * boardW + j] = set;
    }}

    // overwrite board state
    for (int i = 0; i < boardH; i++) { for (int j = 0; j < boardW; j++) {
        board[i][j] = boardNext[i * boardW + j];
    }}

    iteration++; itrSinceEdit++;
}

void printBoard(int caller) {
    if (caller == -1) {
        cout << "== Iteration " << itrSinceEdit << " since last edit\n";
        cout << "== Iteration " << iteration << " since program start\n";
    }
    string txt;
    for (int i = 0; i < boardH; i++) {
        for (int j = 0; j < boardW; j++) {
            txt.insert(txt.length(), 1, (board[i][j] ? '#' : '.'));
            txt.insert(txt.length(), 1, (i * boardW + j == caller ? '<' : ' '));
        }
        txt.insert(txt.length(), 1, '\n');
    }
    cout << txt;
}


void editBoard() {
    string txt;
    bool loopBreak = false;
    do {
        printBoard(ptrY * boardW + ptrX);
        cout << "Character commands: \n";
        cout << "- < / ^ / > / v (move pointer)\n";
        cout << "- 0 / 1 / t (change cell state)\n";
        cout << "x (go to menu)\n";
        cout << "Input all characters to a string, program will read the commands left to right.\n";

        cout << "Input command: ";
        cin >> txt;
        for (int i = 0; i < txt.length(); i++) {
            switch (txt.at(i)) {
            case '<': itrSinceEdit = 0; ptrX = (boardW + ptrX - 1) % boardW; break;
            case '>': itrSinceEdit = 0; ptrX = (boardW + ptrX + 1) % boardW; break;
            case '^': itrSinceEdit = 0; ptrY = (boardH + ptrY - 1) % boardH; break;
            case 'v': itrSinceEdit = 0; ptrY = (boardH + ptrY + 1) % boardH; break;
            case 'V': itrSinceEdit = 0; ptrY = (boardH + ptrY + 1) % boardH; break;
            case '0': itrSinceEdit = 0; board[ptrY][ptrX] = 0; break;
            case '1': itrSinceEdit = 0; board[ptrY][ptrX] = 1; break;
            case 't': itrSinceEdit = 0; board[ptrY][ptrX] = !board[ptrY][ptrX]; break;
            case 'T': itrSinceEdit = 0; board[ptrY][ptrX] = !board[ptrY][ptrX]; break;
            case 'x': loopBreak = true; break;
            case 'X': loopBreak = true; break;
            default: break;
            }
        }
    } while (!loopBreak);
    // printBoard(ptrY*boardW+ptrX);
}

/*
TODO:
- edit
  char cmds:
  - > ^ < v/V (move)
  - 0 1 t/T (change cell)
  - x (exit)
- step space cmd input fix
*/
int main()
{
    // glider starting formation
    board[1][2] = 1;
    board[2][3] = 1;
    board[3][1] = 1;
    board[3][2] = 1;
    board[3][3] = 1;

    string inp = "1";
    cout << "## CONWAY'S GAME OF LIFE ##\n";
    do
    {
        if (inp == "1" || inp == "2" || inp == "3")
        {
            printBoard(-1);
            cout << '\n';
        }
        if (inp == "1" || inp == "2" || inp == "3")
        {
            cout << "Commands:\n";
            cout << "1. Step\n";
            cout << "2. Edit\n";
            cout << "3. Toggle Wrap (currently: " << (wrapAround ? "true" : "false") << ")\n";
            cout << "0. Quit\n";
        }

        cout << "Input: ";
        cin >> inp;
        if (inp.length() == 1)
        {
            if (inp == "1")
            {
                // while
                if (cin.peek() == 32)
                {
                    int loop;
                    cin >> loop;
                    cout << loop;
                    for (int i = 0; i < loop; i++)
                    {
                        step();
                        if (i + 1 != loop)
                        {
                            printBoard(-1);
                            cout << '\n';
                        }
                    }
                }
                else
                {
                    step();
                }
            }
            else if (inp == "2")
            {
                editBoard();
            }
            else if (inp == "3")
            {
                cin.ignore(10000, '\n');
                wrapAround = !wrapAround;
                cout << "Toggle Wrap set to " << (wrapAround ? "true" : "false") << ".\n";
            }
            else if (inp == "0")
            {
                break;
            }
            else
            {
                cin.ignore(10000, '\n');
                cout << "Unknown command.\n";
            }
        }
        else
        {
            cin.ignore(10000, '\n');
            cout << "Unknown command.\n";
        }
        cout << '\n';
    } while (true);
    cout << "Command: Exit. Click enter to exit.\n";
    cin.ignore(10000, '\n');
    cin.get();
    return 0;
}