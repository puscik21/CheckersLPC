#ifndef PROJECT2ZABAWA_FIELD_H
#define PROJECT2ZABAWA_FIELD_H


class Field {
private:
    int playerNumber;
public:
    Field();
    explicit Field(int playerNumber2);
    int getPlayerNumber();
    bool isItPlayerPawn();
    bool isItEnemyPawn();
    bool isItEmptyField();
};


#endif //PROJECT2ZABAWA_FIELD_H
