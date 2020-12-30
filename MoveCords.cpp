#include <sstream>
#include "MoveCords.h"

MoveCords::MoveCords(int fromRow, int fromCol, int toRow, int toCol) : fromRow(fromRow), fromCol(fromCol), toRow(toRow),
                                                                       toCol(toCol) {}

MoveCords::MoveCords(vector<int> cordsVector) {
    fromRow = cordsVector[0];
    fromCol = cordsVector[1];
    toRow = cordsVector[2];
    toCol = cordsVector[3];
}

MoveCords MoveCords::fromString(string cordText) {
    stringstream cordsStream(cordText);
    string segment;
    vector<string> segList;
    vector<int> cordsVector;

    while (getline(cordsStream, segment, ',')) {
        segList.push_back(segment);
    }
    cordsVector.reserve(segList.size());
    for (string number : segList) {
        cordsVector.push_back(atoi(number.c_str()));
    }
    return MoveCords(cordsVector);
}

int MoveCords::getFromRow() const {
    return fromRow;
}

int MoveCords::getFromCol() const {
    return fromCol;
}

int MoveCords::getToRow() const {
    return toRow;
}

int MoveCords::getToCol() const {
    return toCol;
}
