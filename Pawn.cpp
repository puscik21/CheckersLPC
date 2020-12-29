#include "Pawn.h"


Pawn::Pawn() {
    playerNumber = 0;
}

Pawn::Pawn(int playerNumber2) {
    playerNumber = playerNumber2;
}

int Pawn::getPlayerNumber() {
    return this->playerNumber;
}
