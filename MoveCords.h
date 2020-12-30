#ifndef PROJECT2ZABAWA_MOVECORDS_H
#define PROJECT2ZABAWA_MOVECORDS_H

#include <iostream>
#include <vector>

using namespace std;

class MoveCords {
private:
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
public:
    MoveCords(int fromRow, int fromCol, int toRow, int toCol);

    explicit MoveCords(vector<int> cordsVector);

    static MoveCords fromString(string cordText);

    int getFromRow() const;

    int getFromCol() const;

    int getToRow() const;

    int getToCol() const;
};


#endif //PROJECT2ZABAWA_MOVECORDS_H
