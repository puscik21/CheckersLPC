#ifndef PROJECT2ZABAWA_BOARD_H
#define PROJECT2ZABAWA_BOARD_H

using namespace std;

class Board {
    vector<vector<int>> pawns;
//    int ** pawns;
public:
    Board();

    void addPawns(int startRow, int pawnValue);

    string getField(bool isFieldBlack);

    void printBoard();

    void printPawns();

    void preparePawns();
};


#endif //PROJECT2ZABAWA_BOARD_H
