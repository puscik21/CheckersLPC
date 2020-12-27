#include <iostream>
#include "main.h"
#include "Board.h"

using namespace std;

int main() {
    Board board = Board();
    board.printBoard();
    board.preparePawns();
    board.printPawns();
    return 0;
}
