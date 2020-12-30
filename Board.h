#ifndef PROJECT2ZABAWA_BOARD_H
#define PROJECT2ZABAWA_BOARD_H

#include "Field.h"

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

    bool isPlayerToEmptyMove(int fromRow, int fromCol, int toRow, int toCol);

    static bool isSimpleMoveAvailable(int fromRow, int fromCol, int toRow, int toCol);

    bool isJumpMoveAvailable(int fromRow, int fromCol, int toRow, int toCol);

    Field* getBetweenField(int fromRow, int fromCol, int toRow, int toCol);

    bool makePlayerMove(int fromRow, int fromCol, int toRow, int toCol);

    void makeMove(int fromRow, int fromCol, int toRow, int toCol);

    void makeCaptureMove(int fromRow, int fromCol, int toRow, int toCol);

    void makeEnemyMove(int fromRow, int fromCol, int toRow, int toCol);
};


#endif //PROJECT2ZABAWA_BOARD_H
