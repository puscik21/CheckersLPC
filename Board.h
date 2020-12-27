#ifndef PROJECT2ZABAWA_BOARD_H
#define PROJECT2ZABAWA_BOARD_H

using namespace std;

class Board {
    int ** pawns;
public:
    Board();

    int **addPawns(int startRow, int pawnValue);

    string getField(bool isFieldBlack);

    void printBoard();

    void printPawns();

    int **preparePawns();
};


#endif //PROJECT2ZABAWA_BOARD_H
