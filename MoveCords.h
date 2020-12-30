#ifndef PROJECT2ZABAWA_MOVECORDS_H
#define PROJECT2ZABAWA_MOVECORDS_H


class MoveCords {
private:
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
public:
    MoveCords(int fromRow, int fromCol, int toRow, int toCol);

    int getFromRow() const;

    int getFromCol() const;

    int getToRow() const;

    int getToCol() const;
};


#endif //PROJECT2ZABAWA_MOVECORDS_H
