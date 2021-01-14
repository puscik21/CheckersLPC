#include <Board_LED.h>
#include <Board_Joystick.h>
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include "TP_Open1768.h"

typedef struct MoveCords {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
} MoveCords;

typedef struct Point {
    int x;
    int y;
} Point;

// utils
int sleep();

void sentStr(char *str);

int abs(int value);

int min (int a, int b);

int max (int a, int b);

// TODO remove after tests
void pawnsToUart();

// TODO remove after tests
void sentToUART();

// definitions
void initTools();

void initUart();

void initTouchPad();

void touchPadCalibration();

void drawCalibrationPoint();

void setLcdPointer(int x, int y);

void preparePawns();

void addPawns(int startRow, int pawnValue);

int drawBoard();

int writeSquareLine(int length, uint16_t colorIndex);

void drawPawns();

void drawPawn(int row, int col, int playerNumber);

void setLcdPawnPosition(int row, int col);

Point getTouchpadPosition();

bool isItPlayerPawn(Point cords);

bool makePlayerMove(MoveCords moveCords);

bool isPlayerToEmptyMove(MoveCords moveCords);

bool isSimpleMoveAvailable(MoveCords moveCords);

void makeMove(MoveCords moveCords);

bool isJumpMoveAvailable(MoveCords moveCords);

void makeCaptureMove(MoveCords moveCords);

Point getBetweenFieldCords(MoveCords moveCords);

void drawPawnAndClear(MoveCords moveCords, int playerNumber);

void clearPawnFromLcd(int row, int col);

void setLcdFieldPosition(int row, int col);

void makeEnemyMove(char *cordText);

MoveCords makeCordsFromString(char *cordText);

MoveCords rotate(MoveCords moveCords);

int checkWinner();

int EINT3_IRQHandler();

void saveCalibrationPoint(int x, int y);

void makePlayerTurn(int x, int y);

void processPlayerToCords();

void processPlayerFromCords();


//////////////////////////////////////////    INITIALIZATION    //////////////////////////////////////////
int const SQUARE_SIZE_X = 24;
int const SQUARE_SIZE_Y = 32;
int pawns[10][10];
int numberOfTouches = 0;
Point calibration[4];
int isCalibrationOn = 0;
int xMaxTouch;
int yMaxTouch;
int xMinTouch;
int yMinTouch;
Point currentPosition;
Point previousPosition;
bool isPlayerTurn;
bool hasPreviousMove = false;


int sleep() {
    int times = 10000000;
    int i = 0;
    for (i = 0; i < times; i++) {
        i += 1;
    }
    return 0;
}

void sentStr(char *str) {
    while (*str != NULL) {
        if (LPC_UART0->LSR & 64) {
            LPC_UART0->THR = *str;
            str++;
        }
    }
}

int abs(int value) {
    if (value < 0) {
        return value * -1;
    } else {
        return value;
    }
}

int min (int a, int b) {
    if (a <= b) {
        return a;
    } else {
        return b;
    }
}

int max (int a, int b) {
    if (a >= b) {
        return a;
    } else {
        return b;
    }
}

void pawnsToUart() {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            char c = pawns[i][j] + '0';
            sentStr(&c);
        }
        sentStr("\r\n");
    }
}

void sentToUART() {
    int i;
    for (i = 0; i < 4; i++) {
        char buf[100];
        sprintf(buf, "x=%d, y=%d\r\n", calibration[i].x, calibration[i].y);
        sentStr(buf);
    }
}

void initTools() {
    initUart();
    lcdConfiguration();
    uint16_t val = lcdReadReg(OSCIL_ON);
    init_ILI9325();
    initTouchPad();
    touchPadCalibration();
}

void initUart() {
    LPC_UART0->LCR = 1 << 7;
    LPC_UART0->DLM = 0;
    LPC_UART0->DLL = 12;
    LPC_UART0->FDR = 15 << 4 | 2;
    LPC_UART0->LCR = 1 | 1 << 1 | 1 << 2;
    PIN_Configure(0, 2, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
    PIN_Configure(0, 3, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
}

void initTouchPad() {
    touchpanelInit();
    LPC_GPIOINT->IO0IntEnF = 1 << 19;
    NVIC_EnableIRQ(EINT3_IRQn);
    //NVIC_DisableIRQ(EINT3_IRQn);
}

void touchPadCalibration() {
    drawCalibrationPoint();
    isCalibrationOn = 1;
    while (numberOfTouches < 4) {

    }
    isCalibrationOn = 0;
    xMinTouch = (calibration[0].x + calibration[3].x) / 2;
    yMinTouch = (calibration[0].y + calibration[1].y) / 2;
    xMaxTouch = (calibration[1].x + calibration[2].x) / 2;
    yMaxTouch = (calibration[2].y + calibration[3].y) / 2;
}

void drawCalibrationPoint() {
    setLcdPointer(0, 0);
    int i;
    for (i = 0; i < 240 * 320; i++) {
        lcdWriteData(LCDWhite);
    }
    if (numberOfTouches == 0) {
        drawPawn(0, 0, -1);
    } else if (numberOfTouches == 1) {
        drawPawn(9, 0, -1);
    } else if (numberOfTouches == 2) {
        drawPawn(9, 9, -1);
    } else if (numberOfTouches == 3) {
        drawPawn(0, 9, -1);
    }
}

void setLcdPointer(int x, int y) {
    lcdWriteIndex(ADRX_RAM);
    lcdWriteData(x);
    lcdWriteIndex(ADRY_RAM);
    lcdWriteData(y);
    lcdWriteIndex(DATA_RAM);
}

void preparePawns() {
    addPawns(0, -1);
    for (int i = 4; i < 6; i++) {
        for (int j = 0; j < 10; j++) {
            pawns[i][j] = 0;
        }
    }
    addPawns(6, 1);
}

void addPawns(int startRow, int pawnValue) {
    bool isPawnPosition = false;
    for (int i = startRow; i < startRow + 4; i++) {
        isPawnPosition = !isPawnPosition;
        for (int j = 0; j < 10; j++) {
            int playerNumber = isPawnPosition * pawnValue;
            pawns[i][j] = playerNumber;
            isPawnPosition = !isPawnPosition;
        }
    }
}

int drawBoard() {
    setLcdPointer(0, 0);
    int i, j, k;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < SQUARE_SIZE_Y; j++) {
            for (k = 0; k < 5; k++) {
                if (i % 2 == 0) {
                    writeSquareLine(SQUARE_SIZE_X, LCDBlueSea);
                    writeSquareLine(SQUARE_SIZE_X, LCDWhite);
                } else {
                    writeSquareLine(SQUARE_SIZE_X, LCDWhite);
                    writeSquareLine(SQUARE_SIZE_X, LCDBlueSea);
                }
            }
        }
    }
}

int writeSquareLine(int length, uint16_t colorIndex) {
    int j = 0;
    for (j = 0; j < length; j++) {
        lcdWriteIndex(DATA_RAM);
        lcdWriteData(colorIndex);
    }
}

void drawPawns() {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            int playerNumber = pawns[i][j];
            if (playerNumber != 0) {
                drawPawn(i, j, playerNumber);
            }
        }
    }
}

void drawPawn(int row, int col, int playerNumber) {
    setLcdPawnPosition(row, col);
    int j = 0;
    for (j = 0; j < 16; j++) {
        if (playerNumber < 0) {
            writeSquareLine(16, LCDBlack);
        } else {
            writeSquareLine(16, LCDYellow);
        }
        lcdWriteIndex(ADRY_RAM);
        lcdWriteData(col * SQUARE_SIZE_Y + SQUARE_SIZE_Y / 4 + j);
    }
}

void setLcdPawnPosition(int row, int col) {
    int x = row * SQUARE_SIZE_X + SQUARE_SIZE_X / 5;
    int y = col * SQUARE_SIZE_Y + SQUARE_SIZE_Y / 4;
    setLcdPointer(x, y);
}

Point getTouchpadPosition(Point position) {
    int x, y;
    x = 10 * (position.x - xMinTouch) / (xMaxTouch - xMinTouch);
    y = 10 * (position.y - yMinTouch) / (yMaxTouch - yMinTouch);
    x = min(x, 9);
    x = max(x, 0);
    y = min(y, 9);
    y = max(y, 0);

    // TODO remove after tests
    char buf[100];
    sprintf(buf, "row=%d, col=%d\r\n", x, y);
    sentStr(buf);

    Point cords;
    cords.x = x;
    cords.y = y;
    return cords;
}

bool isItPlayerPawn(Point cords) {
    return pawns[cords.x][cords.y] > 0;
}

bool makePlayerMove(MoveCords moveCords) {
    if (!isPlayerToEmptyMove(moveCords)) {
        return false;
    }
    if (isSimpleMoveAvailable(moveCords)) {
        makeMove(moveCords);
    } else if (isJumpMoveAvailable(moveCords)) {
        makeCaptureMove(moveCords);
    } else {
        return false;
    }
    drawPawnAndClear(moveCords, 1);
    return true;
}

// TODO probably later it will be like isToEmptyFieldMove() - cuz only player pawn will be able to choose
bool isPlayerToEmptyMove(MoveCords moveCords) {
    int fromField = pawns[moveCords.fromRow][moveCords.fromCol];
    int toField = pawns[moveCords.toRow][moveCords.toCol];
    return fromField > 0 && toField == 0;
}

bool isSimpleMoveAvailable(MoveCords moveCords) {
    return moveCords.fromRow - moveCords.toRow == 1 &&
           abs(moveCords.fromCol - moveCords.toCol) == 1;
}

void makeMove(MoveCords moveCords) {
    int playerNumber = pawns[moveCords.fromRow][moveCords.fromCol];
    pawns[moveCords.fromRow][moveCords.fromCol] = 0;
    pawns[moveCords.toRow][moveCords.toCol] = playerNumber;
    drawPawnAndClear(moveCords, playerNumber);    // TODO TEST - czy po ruchu pionki przerysowuja sie
}

bool isJumpMoveAvailable(MoveCords moveCords) {
    if (moveCords.fromRow - moveCords.toRow == 2 && abs(moveCords.fromCol - moveCords.toCol) == 2) {
        Point betweenFieldCords = getBetweenFieldCords(moveCords);
        return pawns[betweenFieldCords.x][betweenFieldCords.y] < 0;
    }
    return false;
}

// TODO TEST - pisane w domu, wiec trzeba sprawdzic czy dziala, zwlaszcza czy czysci wrogiego pionka
void makeCaptureMove(MoveCords moveCords) {
    makeMove(moveCords);
    Point betweenFieldCords = getBetweenFieldCords(moveCords);
    pawns[betweenFieldCords.x][betweenFieldCords.y] = 0;
    clearPawnFromLcd(betweenFieldCords.x, betweenFieldCords.y);
}

Point getBetweenFieldCords(MoveCords moveCords) {
    int betweenCol;
    if (moveCords.fromCol > moveCords.toCol) {
        betweenCol = moveCords.fromCol - 1;
    } else {
        betweenCol = moveCords.fromCol + 1;
    }
    Point fieldCords;
    fieldCords.x = moveCords.fromRow - 1;
    fieldCords.y = betweenCol;
    return fieldCords;
}

void drawPawnAndClear(MoveCords moveCords, int playerNumber) {
    drawPawn(moveCords.toRow, moveCords.toCol, playerNumber);
    clearPawnFromLcd(moveCords.fromRow, moveCords.fromCol);
}

// TODO TEST - sprawdzic czy czyszczenie z planszy dziala
void clearPawnFromLcd(int row, int col) {
    setLcdFieldPosition(row, col);
    int i;
    for (i = 0; i < SQUARE_SIZE_Y; i++) {
        writeSquareLine(SQUARE_SIZE_X, LCDBlueSea);
        lcdWriteIndex(ADRY_RAM);
        lcdWriteData(col * SQUARE_SIZE_Y + i + 1);
    }
}

void setLcdFieldPosition(int row, int col) {
    int x = row * SQUARE_SIZE_X;
    int y = col * SQUARE_SIZE_Y;
    setLcdPointer(x, y);
}

// TODO TEST sprawdzic przerysowywanie, potem usunac sprintf()
void makeEnemyMove(char *cordText) {
    MoveCords moveCords = makeCordsFromString(cordText);
    moveCords = rotate(moveCords);
    char buf[100];
    sprintf(buf, "x=%d, y=%d, x2=%d, y2=%d\r\n", moveCords.fromRow, moveCords.fromCol, moveCords.toRow,
            moveCords.toCol);
    sentStr(buf);
    makeMove(moveCords);
}

MoveCords makeCordsFromString(char *cordText) {
    MoveCords moveCords;
    moveCords.fromRow = cordText[0] - 48;
    moveCords.fromCol = cordText[2] - 48;
    moveCords.toRow = cordText[4] - 48;
    moveCords.toCol = cordText[6] - 48;
    return moveCords;
}

MoveCords rotate(MoveCords moveCords) {
    moveCords.fromRow = 9 - moveCords.fromRow;
    moveCords.fromCol = 9 - moveCords.fromCol;
    moveCords.toRow = 9 - moveCords.toRow;
    moveCords.toCol = 9 - moveCords.toCol;
    return moveCords;
}

// TODO TEST - poprawiane w domu, wiec trzeba sprawdzic czy dziala
int checkWinner() {
    int playerPawns = 0;
    int enemyPawns = 0;
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (pawns[i][j] > 0) {
                playerPawns++;
            } else if (pawns[i][j] < 0) {
                enemyPawns++;
            }
        }
    }

    if (playerPawns == 0) {
        return -1;
    } else if (enemyPawns == 0) {
        return 1;
    } else {
        return 0;
    }
}

int EINT3_IRQHandler() {
    int x = touchpanelReadY();
    int y = touchpanelReadX();
    // TODO remove printing after tests
    char buf[100];
    sprintf(buf, "x=%d, y=%d\r\n", x, y);
    sentStr(buf);
    if (isCalibrationOn) {
        saveCalibrationPoint(x, y);
    } else if (isPlayerTurn) {
        makePlayerTurn(x, y);
    }
    sleep();
    LPC_GPIOINT->IO0IntClr = 1 << 19;
}

void saveCalibrationPoint(int x, int y) {
    calibration[numberOfTouches].x = x;
    calibration[numberOfTouches].y = y;
    numberOfTouches++;
    drawCalibrationPoint();
}

void makePlayerTurn(int x, int y) {
    previousPosition.x = currentPosition.x;
    previousPosition.y = currentPosition.y;
    currentPosition.x = x;
    currentPosition.y = y;
    if (hasPreviousMove) {
        processPlayerToCords();
    } else {
        processPlayerFromCords();
    }
}

void processPlayerToCords() {
    Point fromCord = getTouchpadPosition(previousPosition);
    Point toCord = getTouchpadPosition(currentPosition);
    MoveCords currentCords = {fromCord.x, fromCord.y, toCord.x, toCord.y};
    bool isMoveCorrect = makePlayerMove(currentCords);
    hasPreviousMove = false;
    if (isMoveCorrect) {
        //isPlayerTurn = false; // TODO enable after addition of enemy movement
    }
}

void processPlayerFromCords() {
    Point cords = getTouchpadPosition(currentPosition);
    int isPlayerPawn = isItPlayerPawn(cords);
    if (isPlayerPawn) {
        hasPreviousMove = true;
    }
    sprintf(buf, "isPlayerPawn=%d\r\n", isPlayerPawn);
    sentStr(buf);
}


int main() {
    initTools();
    preparePawns();
    drawBoard();
    drawPawns();
    isPlayerTurn = true;
    makeEnemyMove("6,0,5,1");
    MoveCords moveCords = {6, 0, 5, 1};
    makePlayerMove(moveCords);
    //pawnsToUart();

    while (1) {

    }
}
