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

// utils
int sleep();

void sentStr(char *str);

int abs(int value);

void pawnsToUart(); // TODO to be removed after tests

// definitions
void toolsInit();

void uartInit();

void initTouchPad();

void touchPadCalibration();

void preparePawns();

void addPawns(int startRow, int pawnValue);

int drawBoard();

int writeSquareLine(int length, uint16_t colorIndex);

void drawPawns();

void drawPawn(int row, int col, int playerNumber);

void setLcdPawnPosition(int row, int col);

bool makePlayerMove(MoveCords moveCords);

bool isPlayerToEmptyMove(MoveCords moveCords);

bool isSimpleMoveAvailable(MoveCords moveCords);

void makeMove(MoveCords moveCords);

bool isJumpMoveAvailable(MoveCords moveCords);

void makeCaptureMove(MoveCords moveCords);

int getBetweenField(MoveCords moveCords);

void drawPawnAndClear(MoveCords moveCords, int playerNumber);

void setLcdFieldPosition(int row, int col);

void makeEnemyMove(char *cordText);

int checkWinner();

int EINT3_IRQHandler();


//////////////////////////////////////////    INITIALIZATION    //////////////////////////////////////////
int const SQUARE_SIZE_X = 24;
int const SQUARE_SIZE_Y = 32;
int pawns[10][10];
int numberOfTouches = 0;


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

void toolsInit() {
    uartInit();
    lcdConfiguration();
    uint16_t val = lcdReadReg(OSCIL_ON);
    init_ILI9325();
    initTouchPad();
    touchPadCalibration();
}

void uartInit() {
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
    drawPawn(0, 0, 1);
    while (numberOfTouches < 4) {

    }
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
    lcdWriteIndex(ADRX_RAM);
    lcdWriteData(0);
    lcdWriteIndex(ADRY_RAM);
    lcdWriteData(0);

    int i = 0;
    int j = 0;
    int k = 0;
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
    lcdWriteIndex(ADRX_RAM);
    lcdWriteData(row * SQUARE_SIZE_X + SQUARE_SIZE_X / 5);
    lcdWriteIndex(ADRY_RAM);
    lcdWriteData(col * SQUARE_SIZE_Y + SQUARE_SIZE_Y / 4);
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
}

bool isJumpMoveAvailable(MoveCords moveCords) {
    if (moveCords.fromRow - moveCords.toRow == 2 && abs(moveCords.fromCol - moveCords.toCol) == 2) {
        int betweenField = getBetweenField(moveCords);
        return betweenField < 0;
    }
    return false;
}

void makeCaptureMove(MoveCords moveCords) {
    /*
    int betweenField = getBetweenField(moveCords);
    betweenField->setPlayerNumber(0);
    makeMove(moveCords);
    */
}

int getBetweenField(MoveCords moveCords) {
    int betweenCol;
    if (moveCords.fromCol > moveCords.toCol) {
        betweenCol = moveCords.fromCol - 1;
    } else {
        betweenCol = moveCords.fromCol + 1;
    }
    return pawns[moveCords.fromRow - 1][betweenCol];
}

void drawPawnAndClear(MoveCords moveCords, int playerNumber) {
    drawPawn(moveCords.toRow, moveCords.toCol, playerNumber);
    setLcdFieldPosition(moveCords.fromRow, moveCords.fromCol);
    int i;
    for (i = 0; i < SQUARE_SIZE_Y; i++) {
        writeSquareLine(SQUARE_SIZE_X, LCDBlueSea);
        lcdWriteIndex(ADRY_RAM);
        lcdWriteData(moveCords.fromCol * SQUARE_SIZE_Y + i + 1);
    }
}

void setLcdFieldPosition(int row, int col) {
    lcdWriteIndex(ADRX_RAM);
    lcdWriteData(row * SQUARE_SIZE_X);
    lcdWriteIndex(ADRY_RAM);
    lcdWriteData(col * SQUARE_SIZE_Y);
}

void makeEnemyMove(char *cordText) {
    /*
    MoveCords moveCords = MoveCords::fromString(std::move(cordText));
    moveCords.rotate();
    makeMove(moveCords);
     */
}

int checkWinner() {
    /*
    int playerPawns = 0;
    int enemyPawns = 0;
    for (const vector<Field> &row : pawns) {
        for (Field field : row) {
            if (field.getPlayerNumber() > 0) {
                playerPawns++;
            } else if (field.getPlayerNumber() < 0) {
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
     */
}

int EINT3_IRQHandler() {
    char buf[100];
    int x = touchpanelReadX();
    int y = touchpanelReadY();
    sprintf(buf, "x=%d, y=%d\r\n", x, y);
    sent_str(buf);
    sleep();

    LPC_GPIOINT->IO0IntClr = 1 << 19;
}


int main() {
    toolsInit();
    preparePawns();
    drawBoard();
    drawPawns();
    MoveCords moveCords = {6, 0, 5, 1};
    makePlayerMove(moveCords);
    drawPawnAndClear(moveCords, 1);
    //pawnsToUart();

    while (1) {

    }
    return 0;
}
