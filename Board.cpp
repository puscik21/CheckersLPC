#include <iostream>
#include <vector>
#include <math.h>
#include "Field.h"
#include "Board.h"

using namespace std;

Board::Board() {
    vector<vector<Field>> iDontKnowHowToMakeItInOneLine(10, vector<Field>(0));
    fields = iDontKnowHowToMakeItInOneLine;
}

void Board::printBoard() {
    bool isFieldBlack = true;
    for (int i = 0; i < 10; i++) {
        isFieldBlack = !isFieldBlack;
        for (int j = 0; j < 10; j++) {
            cout << getField(isFieldBlack);
            isFieldBlack = !isFieldBlack;
        }
        cout << endl;
    }
}

string Board::getField(bool isFieldBlack) {
    if (isFieldBlack) {
        return "##|";
    } else {
        return "  |";
    }
}

void Board::printFields() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cout << fields[i][j].getPlayerNumber() << "|";
        }
        cout << endl;
    }
}

void Board::prepareFields() {
    addFields(0, -1);
    for (int i = 4; i < 6; i++) {
        for (int j = 0; j < 10; j++) {
            fields[i].push_back(Field(0));
        }
    }
    addFields(6, 1);
}

void Board::addFields(int startRow, int pawnValue) {
    bool isPawnPosition = true;
    for (int i = startRow; i < startRow + 4; i++) {
        isPawnPosition = !isPawnPosition;
        for (int j = 0; j < 10; j++) {
            int playerNumber = isPawnPosition * pawnValue;
            fields[i].push_back(Field(playerNumber));
            isPawnPosition = !isPawnPosition;
        }
    }
}

bool Board::isMoveAvailable(int fromRow, int fromCol, int toRow, int toCol) {
    Field fromField = fields[fromRow][fromCol];
    Field toField = fields[toRow][toCol];
    if (!fromField.isItPlayerPawn() || !toField.isItEmptyField()) {
        return false;
    }
    return (isSimpleMoveAvailable(fromRow, fromCol, toRow, toCol)) || (isJumpMoveAvailable(fromRow, fromCol, toRow, toCol));
}

bool Board::isSimpleMoveAvailable(int fromRow, int fromCol, int toRow, int toCol) {
    return fromRow - toRow == 1 && abs(fromCol - toCol) == 1;
}

bool Board::isJumpMoveAvailable(int fromRow, int fromCol, int toRow, int toCol) {
    if (fromRow - toRow == 2 && abs(fromCol - toCol) == 2) {
        int betweenCol;
        if (fromCol > toCol) {
            betweenCol = fromCol - 1;
        } else {
            betweenCol = fromCol + 1;
        }
        Field betweenField = fields[fromRow - 1][betweenCol];
        return betweenField.isItEnemyPawn();
    }
    return false;
}


