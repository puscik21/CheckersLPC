#include <iostream>
#include <vector>
#include <math.h>
#include "Field.h"
#include "Board.h"
#include "MoveCords.h"

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
bool Board::isPlayerToEmptyMove(MoveCords moveCords) {
    Field fromField = fields[moveCords.getFromRow()][moveCords.getFromCol()];
    Field toField = fields[moveCords.getToRow()][moveCords.getToCol()];
    return fromField.isItPlayerPawn() && toField.isItEmptyField();
}

bool Board::isSimpleMoveAvailable(MoveCords moveCords) {
    return moveCords.getFromRow() - moveCords.getToRow() == 1 &&
           abs(moveCords.getFromCol() - moveCords.getToCol()) == 1;
}

bool Board::isJumpMoveAvailable(MoveCords moveCords) {
    if (moveCords.getFromRow() - moveCords.getToRow() == 2 && abs(moveCords.getFromCol() - moveCords.getToCol()) == 2) {
        Field *betweenField = getBetweenField(moveCords);
        return betweenField->isItEnemyPawn();
    }
    return false;
}

Field *Board::getBetweenField(MoveCords moveCords) {
    int betweenCol;
    if (moveCords.getFromCol() > moveCords.getToCol()) {
        betweenCol = moveCords.getFromCol() - 1;
    } else {
        betweenCol = moveCords.getFromCol() + 1;
    }
    return &fields[moveCords.getFromRow() - 1][betweenCol];
}

bool Board::makePlayerMove(MoveCords moveCords) {
    if (!isPlayerToEmptyMove(moveCords)) {
        return false;
    }
    if (isSimpleMoveAvailable(moveCords)) {
        makeMove(moveCords);
    } else if (isJumpMoveAvailable(moveCords)) {
        makeCaptureMove(moveCords);
    } else {
        return false;
    }
    return true;
}

void Board::makeMove(MoveCords moveCords) {
    int playerNumber = fields[moveCords.getFromRow()][moveCords.getFromCol()].getPlayerNumber();
    fields[moveCords.getFromRow()][moveCords.getFromCol()].setPlayerNumber(0);
    fields[moveCords.getToRow()][moveCords.getToCol()].setPlayerNumber(playerNumber);
}

void Board::makeCaptureMove(MoveCords moveCords) {
    Field *betweenField = getBetweenField(moveCords);
    betweenField->setPlayerNumber(0);
    makeMove(moveCords);
}

void Board::makeEnemyMove(string cordText) {
    MoveCords moveCords = MoveCords::fromString(std::move(cordText));
    moveCords.rotate();
    makeMove(moveCords);
}

int Board::checkWinner() {
    int playerPawns = 0;
    int enemyPawns = 0;
    for (const vector<Field> &row : fields) {
        for (Field field : row) {
            if (field.getPlayerNumber() > 0) {
                playerPawns++;
            } else if (field.getPlayerNumber() < 0) {
                enemyPawns++;
            }
        }
    }

    if (playerPawns == 0) {
        return -1;
    } else if (enemyPawns == 0) {
        return 1;
    } else {
        return 0;
    }
}


