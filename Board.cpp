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
    cout << endl;
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

// TODO probably later it will be like isToEmptyFieldMove() - cuz only player pawn will be able to choose
bool Board::isPlayerToEmptyMove(int fromRow, int fromCol, int toRow, int toCol) {
    Field fromField = fields[fromRow][fromCol];
    Field toField = fields[toRow][toCol];
    return fromField.isItPlayerPawn() && toField.isItEmptyField();
}

bool Board::isSimpleMoveAvailable(int fromRow, int fromCol, int toRow, int toCol) {
    return fromRow - toRow == 1 && abs(fromCol - toCol) == 1;
}

bool Board::isJumpMoveAvailable(int fromRow, int fromCol, int toRow, int toCol) {
    if (fromRow - toRow == 2 && abs(fromCol - toCol) == 2) {
        Field *betweenField = getBetweenField(fromRow, fromCol, toRow, toCol);
        return betweenField->isItEnemyPawn();
    }
    return false;
}

Field *Board::getBetweenField(int fromRow, int fromCol, int toRow, int toCol) {
    int betweenCol;
    if (fromCol > toCol) {
        betweenCol = fromCol - 1;
    } else {
        betweenCol = fromCol + 1;
    }
    return &fields[fromRow - 1][betweenCol];
}

bool Board::makePlayerMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (!isPlayerToEmptyMove(fromRow, fromCol, toRow, toCol)) {
        return false;
    }
    if (isSimpleMoveAvailable(fromRow, fromCol, toRow, toCol)) {
        makeMove(fromRow, fromCol, toRow, toCol);
    } else if (isJumpMoveAvailable(fromRow, fromCol, toRow, toCol)) {
        makeCaptureMove(fromRow, fromCol, toRow, toCol);
    } else {
        return false;
    }
    return true;
}

void Board::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    int playerNumber = fields[fromRow][fromCol].getPlayerNumber();
    fields[fromRow][fromCol].setPlayerNumber(0);
    fields[toRow][toCol].setPlayerNumber(playerNumber);
}

void Board::makeCaptureMove(int fromRow, int fromCol, int toRow, int toCol) {
    Field *betweenField = getBetweenField(fromRow, fromCol, toRow, toCol);
    betweenField->setPlayerNumber(0);
    makeMove(fromRow, fromCol, toRow, toCol);
}

void Board::makeEnemyMove(int fromRow, int fromCol, int toRow, int toCol) {
    // TODO it will probably comes with numbers that see enemy - make method to changed numbers by some symmetry
    makeMove(fromRow, fromCol, toRow, toCol);
}


