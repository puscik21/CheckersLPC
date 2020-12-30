#ifndef PROJECT2ZABAWA_BOARD_H
#define PROJECT2ZABAWA_BOARD_H

#include "Field.h"
#include "MoveCords.h"

using namespace std;

class Board {
    vector<vector<Field>> fields;
public:
    Board();

    void addFields(int startRow, int pawnValue);

    string getField(bool isFieldBlack);

    void printBoard();

    void printFields();

    void prepareFields();

    bool isPlayerToEmptyMove(MoveCords moveCords);

    static bool isSimpleMoveAvailable(MoveCords moveCords);

    bool isJumpMoveAvailable(MoveCords moveCords);

    Field *getBetweenField(MoveCords moveCords);

    bool makePlayerMove(MoveCords moveCords);

    void makeMove(MoveCords moveCords);

    void makeCaptureMove(MoveCords moveCords);

    void makeEnemyMove(MoveCords moveCords);
};


#endif //PROJECT2ZABAWA_BOARD_H
