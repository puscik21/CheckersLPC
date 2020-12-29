#include <iostream>
#include <vector>
#include "Pawn.h"
#include "Board.h"

using namespace std;

Board::Board() {
    vector<vector<Pawn>> iDontKnowHowToMakeItInOneLine (10, vector<Pawn> (0));
    pawns = iDontKnowHowToMakeItInOneLine;
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
            cout << pawns[i][j].getPlayerNumber() << "|";
        }
        cout << endl;
    }
}

void Board::preparePawns() {
    addPawns(0, -1);
    for (int i = 4; i < 6; i++) {
        for (int j = 0; j < 10; j++) {
            pawns[i].push_back(Pawn(0));
        }
    }
    addPawns(6, 1);
}

void Board::addPawns(int startRow, int pawnValue) {
    bool isPawnPosition = true;
    for (int i = startRow; i < startRow + 4; i++) {
        isPawnPosition = !isPawnPosition;
        for (int j = 0; j < 10; j++) {
            int playerNumber = isPawnPosition * pawnValue;
            pawns[i].push_back(Pawn(playerNumber));
            isPawnPosition = !isPawnPosition;
        }
    }
}
