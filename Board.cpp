#include <iostream>
#include "Pawn.h"
#include "Board.h"

using namespace std;

Board::Board() {
    pawns = new int *[10];
    for (int i = 0; i < 10; i++) {
        pawns[i] = new int[10];
    }
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

void Board::printPawns() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cout << pawns[i][j] << "|";
        }
        cout << endl;
    }
}

int **Board::preparePawns() {
    addPawns(0, -1);
    for (int i = 4; i < 6; i++) {
        for (int j = 0; j < 10; j++) {
            pawns[i][j] = 0;
        }
    }
    addPawns(6, 1);
    return pawns;
}

int **Board::addPawns(int startRow, int pawnValue) {
    bool isPawnPosition = true;
    for (int i = startRow; i < 10; i++) {
        isPawnPosition = !isPawnPosition;
        for (int j = 0; j < 10; j++) {
            pawns[i][j] = isPawnPosition * pawnValue;
            isPawnPosition = !isPawnPosition;
        }
    }
    return pawns;
}
