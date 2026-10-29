#include <iostream>
#include <string>
using namespace std;

const int BOARD_LENGTH = 72, BOARD_WIDTH = 24;
int iteration = 0, itrSinceEdit = 0, ptrX = 0, ptrY = 0;
bool wrapAround = true;
bool board[BOARD_WIDTH][BOARD_LENGTH] = {0};
bool boardNext[BOARD_LENGTH * BOARD_WIDTH] = {0};

short checkNeighborWa(int w, int h) {
    short c = 0;
    // check 3x3 area of targeted cell
    for (int i = -1; i <= 1; i++) { for (int j = -1; j <= 1; j++) {
        // board index formula:
        // (boardSize + targetCellIdx + tileShift) mod boardSize
        // applies for both tile boardLength & boardWidth
        // tileShift can be -1, 0, or 1
        c += board[(BOARD_WIDTH + h + i) % BOARD_WIDTH][(BOARD_LENGTH + w + j) % BOARD_LENGTH];
    }}

    // remove current cell's state from total
    c -= board[h][w];
    return c;
}

short checkNeighborNoWa(int w, int h) {
    short c = 0;
    // check 3x3 area of targeted cell
    for (int i = -1; i <= 1; i++) { for (int j = -1; j <= 1; j++) {
        // check left/right board edge
        if (w + j < 0 || w + j >= BOARD_LENGTH) {
            continue;
        }
        // check top/bottom board edge
        if (h + i < 0 || h + i >= BOARD_WIDTH) {
            continue;
        }
        c += board[h + i][w + j];
    }}

    // remove current cell's state from total
    c -= board[h][w];
    return c;
}

short checkNeighbor(int w, int h) {
    // checkNeighborWa() and checkNeighborNoWa() are split for code readability
    if (wrapAround) {
        return checkNeighborWa(w, h);
    } else {
        return checkNeighborNoWa(w, h);
    }
}

void step() {
    // make new board data
    for (int i = 0; i < BOARD_WIDTH; i++) { for (int j = 0; j < BOARD_LENGTH; j++) {
        short cn = checkNeighbor(j, i);
        bool set;
        if (board[i][j]) {
            // if this cell is alive, exactly 2 or 3 cells around it must be alive to stay alive; otherwise this cell turns dead
            set = cn == 2 || cn == 3;
        } else {
            // if this cell is dead, exactly 3 cells around it must be alive to turn alive; otherwise this cell stays dead
            set = cn == 3;
        }
        // board -> boardNext
        // 0 1 2
        // 3 4 5 -> 0 1 2 3 4 5 6 7 8
        // 6 7 8
        boardNext[i * BOARD_LENGTH + j] = set;
    }}

    // overwrite board state
    for (int i = 0; i < BOARD_WIDTH; i++) { for (int j = 0; j < BOARD_LENGTH; j++) {
        board[i][j] = boardNext[i * BOARD_LENGTH + j];
    }}

    iteration++; itrSinceEdit++;
}

// caller -1 -> default show board state (no pointer)
// caller 0+ -> editing board state (shows pointer at the cell currently targeted)
void printBoard(int caller) {
    if (caller == -1) {
        cout << "== Iteration " << itrSinceEdit << " since last edit\n";
        cout << "== Iteration " << iteration << " since program start\n";
    } else {
        cout << "== Edit Board\n";
    }

    string txt;
    // uses string append instead of to print because printing to display is slow
    // (especially when doing step 100 command)
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            txt.insert(txt.length(), 1, (board[i][j] ? '#' : '.'));
            txt.insert(txt.length(), 1, (caller == i * BOARD_LENGTH + j ? '<' : ' '));
        }
        txt.insert(txt.length(), 1, '\n');
    }
    cout << txt;
}

void runBoard(int numStep) {
    for (int i = 0; i < numStep; i++) {
        step();
        if (i + 1 != numStep) {
            cout << '\n';
            printBoard(-1);
        }
    }
}

// split from main() for code readability
void editBoard() {
    string txt;
    bool loopBreak = false;
    do {
        cout << '\n';
        printBoard(ptrY * BOARD_LENGTH + ptrX);
        cout << "\nCharacter commands: \n";
        cout << "- \"<\" / \"^\" / \">\" / \"v\" (move pointer)\n";
        cout << "- \"0\" / \"1\" / \"t\" (change cell state)\n";
        cout << "- \"x\" (go to menu)\n";
        cout << "Input all characters to a string, program will read the commands left to right.\n";
        cout << "(other characters will be ignored)\n";

        cout << "Input command: ";
        cin >> txt;
        for (int i = 0; i < txt.length(); i++) {
            if (loopBreak) { break; }
            switch (txt.at(i)) {
                case '<': itrSinceEdit = 0; ptrX = (BOARD_LENGTH + ptrX - 1) % BOARD_LENGTH; break;
                case '>': itrSinceEdit = 0; ptrX = (BOARD_LENGTH + ptrX + 1) % BOARD_LENGTH; break;
                case '^': itrSinceEdit = 0; ptrY = (BOARD_WIDTH + ptrY - 1) % BOARD_WIDTH; break;
                case 'v':
                case 'V': itrSinceEdit = 0; ptrY = (BOARD_WIDTH + ptrY + 1) % BOARD_WIDTH; break;
                case '0': itrSinceEdit = 0; board[ptrY][ptrX] = 0; break;
                case '1': itrSinceEdit = 0; board[ptrY][ptrX] = 1; break;
                case 't':
                case 'T': itrSinceEdit = 0; board[ptrY][ptrX] = !board[ptrY][ptrX]; break;
                case 'x':
                case 'X': loopBreak = true; cin.ignore(10000, '\n'); break;
                default: break;
            }
        }
    } while (!loopBreak);
}

int readInputInt() {
    string inp; cin >> inp;
    try {
        return stoi(inp);
    } 
    catch (invalid_argument&) {} 
    catch (out_of_range&) {}
    return -1;
}

int main() {
    // glider starting formation
    board[1][2] = 1;
    board[2][3] = 1;
    board[3][1] = 1;
    board[3][2] = 1;
    board[3][3] = 1;

    int inp = 1;
    bool loopBreak = false;
    cout << "#################################\n";
    cout << "##                             ##\n";
    cout << "##  CLI CONWAY'S GAME OF LIFE  ##\n";
    cout << "##                             ##\n";
    cout << "#################################\n\n";
    do {
        if (inp == 1 || inp == 2 || inp == 3) {
            printBoard(-1);
            cout << '\n';
            if (inp == 3) {
                cout << "Toggle Wrap set to " << (wrapAround ? "true" : "false") << ".\n";
            }
            cout << "Commands:\n";
            cout << "1. Step\n";
            cout << "2. Edit\n";
            cout << "3. Toggle Wrap (currently: " << (wrapAround ? "true" : "false") << ")\n";
            cout << "0. Quit\n";
        }

        cout << "Input: "; inp = readInputInt();
        switch (inp) {
            case 1: 
                // checks if there is another number being inputted
                if (cin.peek() == 32) {
                    int loop = readInputInt();
                    if (loop <= 0) {
                        inp = loop;
                        cin.ignore(10000, '\n');
                        cout << "Invalid loop count.\n\n";
                        continue;
                    }
                    runBoard(loop);
                } else {
                    runBoard(1);
                }
                break;
            case 2:
                editBoard();
                break;
            case 3: 
                cin.ignore(10000, '\n');
                wrapAround = !wrapAround;
                break;
            case 0: 
                loopBreak = true;
                break;
            default:
                cin.ignore(10000, '\n');
                cout << "Unknown command.\n";
                break;
        }
        cout << '\n';
    } while (!loopBreak);

    cout << "Command: Exit. Click enter to exit.\n";
    cin.ignore(10000, '\n');
    cin.get();

    return 0;
}