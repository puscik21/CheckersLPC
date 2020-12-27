#include <iostream>

using namespace std;

int** addPawns(int **pawns, int startRow, int pawnValue);

string getField(bool isFieldBlack) {
    if (isFieldBlack) {
        return "##|";
    } else {
        return "  |";
    }
}

void printBoard() {
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

int **preparePawns() {
    int **pawns = new int *[10];
    for (int i = 0; i < 10; i++) {
        pawns[i] = new int[10];
    }
    addPawns(pawns, 0, -1);
    for (int i = 4; i < 6; i++) {
        for (int j = 0; j < 10; j++) {
            pawns[i][j] = 0;
        }
    }
    addPawns(pawns, 6, 1);
    return pawns;
}

int** addPawns(int **pawns, int startRow, int pawnValue) {
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

int main() {
    std::cout << "Hello, World!" << std::endl;
//    printBoard();
    int **pawns = preparePawns();

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cout << pawns[i][j] << "|";
        }
        cout << endl;
    }

    return 0;
}
