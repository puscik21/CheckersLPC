#include <iostream>
#include <vector>
#include "Board.h"

using namespace std;

int main() {
    Board board = Board();
//    board.printBoard();
    board.prepareFields();
    board.printFields();

    cout << "is move available: " << endl;
    cout << board.isMoveAvailable(6, 1, 4, 3) << endl;
    return 0;
}
