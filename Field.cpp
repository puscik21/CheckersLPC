#include "Field.h"


Field::Field() {
    playerNumber = 0;
}

Field::Field(int playerNumber) {
    this->playerNumber = playerNumber;
}

int Field::getPlayerNumber() {
    return playerNumber;
}

bool Field::isItPlayerPawn() {
    return playerNumber > 0;
}

bool Field::isItEnemyPawn() {
    return playerNumber < 0;
}

bool Field::isItEmptyField() {
    return playerNumber == 0;
}

void Field::setPlayerNumber(int playerNumber) {
    this->playerNumber = playerNumber;
}
