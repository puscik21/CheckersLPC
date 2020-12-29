#include <iostream>
#include <vector>
#include "Board.h"

using namespace std;

int main() {
    Board board = Board();
//    board.printBoard();
    board.preparePawns();
    board.printPawns();
//    vector<int> a;
//    a.push_back(2);
//    cout << a[0];
    return 0;
}
