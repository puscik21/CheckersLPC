#include "MoveCords.h"

MoveCords::MoveCords(int fromRow, int fromCol, int toRow, int toCol) : fromRow(fromRow), fromCol(fromCol), toRow(toRow),
                                                                       toCol(toCol) {}

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
