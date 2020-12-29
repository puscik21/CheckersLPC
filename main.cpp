#include <iostream>
#include <vector>
#include "Board.h"

using namespace std;

int main() {
    Board board = Board();
//    board.printBoard();
    board.prepareFields();

    board.makeEnemyMove(3, 0, 4, 1);
    board.makeEnemyMove(4, 1, 5, 2);
    board.printFields();

    cout << "is move available: " << endl;
    cout << board.isMoveAvailable(6, 1, 4, 3) << endl;
    board.makePlayerMove(6, 1, 4, 3);
    board.printFields();

    return 0;
}
