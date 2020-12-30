#include <iostream>
#include <vector>
#include "Board.h"

using namespace std;

int main() {
    Board board = Board();
//    board.printBoard();
    board.prepareFields();

    board.makeEnemyMove(3, 0, 4, 1);
    board.printFields();
    board.makeEnemyMove(4, 1, 5, 2);
    board.printFields();

    bool wasMoveAvailable = board.makePlayerMove(6, 1, 4, 3);
    cout << "wasMoveAvailable: " << wasMoveAvailable << endl;
    board.printFields();

    return 0;
}
