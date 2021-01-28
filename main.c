#include <Board_LED.h>
#include <Board_Joystick.h>
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include "TP_Open1768.h"
#include <string.h>

typedef struct MoveCords {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
} MoveCords;

typedef struct Point{
		int x;
		int y;
} Point;

// utils
int sleep(int multiply);

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

Point getTouchpadPosition();

bool makePlayerMove(MoveCords moveCords);

bool isPlayerToEmptyMove(MoveCords moveCords);

bool isSimpleMoveAvailable(MoveCords moveCords);

void makeMove(MoveCords moveCords);

bool isJumpMoveAvailable(MoveCords moveCords);

void makeCaptureMove(MoveCords moveCords);

void clearPawn(Point pawnCords);

void sendMoveToEnemy(MoveCords moveCords, Point betweenFieldCords);

Point getBetweenFieldCords(MoveCords moveCords);

void drawPawnAndClear(MoveCords moveCords, int playerNumber);

void clearPawnFromLcd(int row, int col);

void setLcdFieldPosition(int row, int col);

void makeEnemyMove(char *cordText);

void tryEnemyCaptureMove(char *cordText);

Point takeCaptureCordFromString(char *cordText);

void checkWinner();

int EINT3_IRQHandler();

bool listenToEnemyStartPress();

void listenToEnemyMove();

void writeLetter(char letter, int x, int y, uint16_t color);

void writeSentence(char* sent, int x, int y, uint16_t color);


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
bool hasPreviousMove = 0;
bool beforeGame= true;


int sleep(int multiply) {
    int times = multiply * 1000000;
    int i = 0;
    for (i = 0; i < times; i++) {
        i += 1;
    }
    return 0;
}

void sentStr(char *str) {
    while (*str != NULL) {
        if (LPC_UART2->LSR & 64) {
            LPC_UART2->THR = *str;
            str++;
        }
    }
}

void sentStrDebug(char *str) {
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
            sentStrDebug(&c);
        }
        sentStrDebug("\r\n");
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
	
		LPC_UART2->LCR = 1 << 7;
    LPC_UART2->DLM = 0;
    LPC_UART2->DLL = 12;
    LPC_UART2->FDR = 15 << 4 | 2;
    LPC_UART2->LCR = 1 | 1 << 1 | 1 << 2;
    PIN_Configure(0, 10, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
    PIN_Configure(0, 11, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
}

void initTouchPad() {
    touchpanelInit();
    LPC_GPIOINT->IO0IntEnF = 1 << 19;
    NVIC_EnableIRQ(EINT3_IRQn);
    //NVIC_DisableIRQ(EINT3_IRQn);
}

void sentToUART(){
	int i;
	for(i=0;i<4;i++){
		char buf[100];
		sprintf(buf, "x=%d, y=%d\r\n", calibration[i].x, calibration[i].y);
		sentStrDebug(buf);
	}
}

void drawCalibrationPoint(){
	lcdWriteIndex(ADRX_RAM);
	lcdWriteData(0);
	lcdWriteIndex(ADRY_RAM);
	lcdWriteData(0);
	lcdWriteIndex(DATA_RAM);
	int i;
	for (i = 0; i < 240*320; i++) {
		lcdWriteData(LCDWhite);
	}
	char buf[100];
	sprintf(buf, "Touchpad Configuration");
	writeSentence(buf,60,110,LCDBlack);
	if(numberOfTouches==0)
	{
		drawPawn(0, 0, -1);
	}
	else if(numberOfTouches==1)
	{
		drawPawn(9, 0, -1);
	}
	else if(numberOfTouches==2)
	{
		drawPawn(9, 9, -1);
	}
	else if(numberOfTouches==3)
	{
		drawPawn(0, 9, -1);
	}	
}

void touchPadCalibration() {
    drawCalibrationPoint();
		isCalibrationOn = 1;
    while (numberOfTouches < 4) {
				
    }
		isCalibrationOn = 0;
		xMinTouch = (calibration[0].x + calibration[3].x)/2;
		yMinTouch = (calibration[0].y + calibration[1].y)/2;
		xMaxTouch = (calibration[1].x + calibration[2].x)/2;
		yMaxTouch = (calibration[2].y + calibration[3].y)/2;
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

Point getTouchpadPosition(Point position){
		int x;
		int y;
		x = 10*(position.x - xMinTouch)/(xMaxTouch - xMinTouch);
		y = 10*(position.y - yMinTouch)/(yMaxTouch - yMinTouch);
		if(x < 0){
			x = 0;
		}
		if(y < 0){
			y = 0;
		}
		if(x > 9){
			x = 9;
		}
		if(y > 9){
			y = 9;
		}
		char buf[100];
		sprintf(buf, "row=%d, col=%d\r\n", x, y);
		sentStrDebug(buf);
		Point cords;
		cords.x = x;
		cords.y = y;
		return cords;
}

int isItPlayerPawn(Point cords){
		return pawns[cords.x][cords.y] > 0;
}

bool makePlayerMove(MoveCords moveCords) {
    if (!isPlayerToEmptyMove(moveCords)) {
        return false;
    }
    if (isSimpleMoveAvailable(moveCords)) {
        makeMove(moveCords);
				Point fakeCords = {-1, -1};
				sendMoveToEnemy(moveCords, fakeCords);
    } else if (isJumpMoveAvailable(moveCords)) {
        makeCaptureMove(moveCords);
				Point betweenFieldCords = getBetweenFieldCords(moveCords);
				sendMoveToEnemy(moveCords, betweenFieldCords);
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
	  drawPawnAndClear(moveCords, playerNumber);
}

bool isJumpMoveAvailable(MoveCords moveCords) {
    if (moveCords.fromRow - moveCords.toRow == 2 && abs(moveCords.fromCol - moveCords.toCol) == 2) {
        Point betweenFieldCords = getBetweenFieldCords(moveCords);
        return pawns[betweenFieldCords.x][betweenFieldCords.y] < 0;
    }
    return false;
}

void makeCaptureMove(MoveCords moveCords) {
	makeMove(moveCords);
	Point betweenFieldCords = getBetweenFieldCords(moveCords);
	clearPawn(betweenFieldCords);
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

void clearPawn(Point pawnCords) {
	pawns[pawnCords.x][pawnCords.y] = 0;
	clearPawnFromLcd(pawnCords.x, pawnCords.y);
}

void sendMoveToEnemy(MoveCords moveCords, Point betweenFieldCords) {
	char buf[100];
	if (betweenFieldCords.x == -1 || betweenFieldCords.y == -1) {
		sprintf(buf, "%d,%d,%d,%d|%c,%c \r\n$", moveCords.fromRow, moveCords.fromCol, moveCords.toRow, moveCords.toCol, 'x', 'x');
	} else {
		sprintf(buf, "%d,%d,%d,%d|%d,%d \r\n$", moveCords.fromRow, moveCords.fromCol, moveCords.toRow, moveCords.toCol, betweenFieldCords.x, betweenFieldCords.y);
	}
	sentStr(buf);
	isPlayerTurn = false;
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

void markPawnOnLcd(int row, int col, int playerNumber) {
    setLcdFieldPosition(row, col);
    int i;
    for (i = 0; i < SQUARE_SIZE_Y; i++) {
        writeSquareLine(SQUARE_SIZE_X, LCDRed);
        lcdWriteIndex(ADRY_RAM);
        lcdWriteData(col * SQUARE_SIZE_Y + i + 1);
    }
		drawPawn(row,col,playerNumber);
}

void setLcdFieldPosition(int row, int col) {
    lcdWriteIndex(ADRX_RAM);
    lcdWriteData(row * SQUARE_SIZE_X);
    lcdWriteIndex(ADRY_RAM);
    lcdWriteData(col * SQUARE_SIZE_Y);
}

MoveCords makeCordsFromString(char* cordText){
	MoveCords moveCords;
	moveCords.fromRow = cordText[0] - 48;
	moveCords.fromCol = cordText[2] - 48;
	moveCords.toRow = cordText[4] - 48;
	moveCords.toCol = cordText[6] - 48;
	return moveCords;
}

MoveCords rotate(MoveCords moveCords){
		moveCords.fromRow = 9 - moveCords.fromRow;
    moveCords.fromCol = 9 - moveCords.fromCol;
    moveCords.toRow = 9 - moveCords.toRow;
    moveCords.toCol = 9 - moveCords.toCol;
		return moveCords;
}

void makeEnemyMove(char *cordText) {
    MoveCords moveCords = makeCordsFromString(cordText);
    moveCords = rotate(moveCords);	
	char buf[100];
		sprintf(buf, "x=%d, y=%d, x2=%d, y2=%d\r\n", moveCords.fromRow, moveCords.fromCol, moveCords.toRow, moveCords.toCol);
		sentStrDebug(buf);

		tryEnemyCaptureMove(cordText);
    makeMove(moveCords);
}

void tryEnemyCaptureMove(char *cordText) {
		Point point = takeCaptureCordFromString(cordText);
		if (point.x < 10 && point.y < 10) {
			clearPawn(point);
		}
}

Point takeCaptureCordFromString(char *cordText) {
	Point capturePoint;
	capturePoint.x = cordText[8] - 48;
	capturePoint.y = cordText[10] - 48;
	
	if (capturePoint.x < 10 && capturePoint.y < 10) {
		capturePoint.x = 9 - capturePoint.x;
		capturePoint.y = 9 - capturePoint.y;
	}
	return capturePoint;
}


void checkWinner() {
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
        isPlayerTurn = false;
				writeSentence("You won!",100,110,LCDRed);
    } else if (enemyPawns == 0) {
        isPlayerTurn = false;
				writeSentence("Enemy won!",100,110,LCDRed);
    }
}

int EINT3_IRQHandler() {
			char buf[100];
			int x = touchpanelReadY();
			int y = touchpanelReadX();
			sprintf(buf, "x=%d, y=%d\r\n", x, y);
			sentStrDebug(buf);
    if(isCalibrationOn){
			calibration[numberOfTouches].x = x;
			calibration[numberOfTouches].y = y;
			numberOfTouches++;
			drawCalibrationPoint();
		}
		else if(isPlayerTurn){
			previousPosition.x = currentPosition.x;
			previousPosition.y = currentPosition.y;
			currentPosition.x = x;
			currentPosition.y = y;
			if(hasPreviousMove){
				Point fromCord = getTouchpadPosition(previousPosition);
				Point toCord = getTouchpadPosition(currentPosition);
				MoveCords currentCords = {fromCord.x, fromCord.y, toCord.x, toCord.y};
				bool isCorrectMove = makePlayerMove(currentCords);
				hasPreviousMove = false;
				if(isCorrectMove){
					isPlayerTurn = false;
				}
				else{
					clearPawnFromLcd(fromCord.x,fromCord.y);
					drawPawn(fromCord.x,fromCord.y,1);
				}
			}
			else{
				Point cords = getTouchpadPosition(currentPosition);
				int isPlayerPawn = isItPlayerPawn(cords);
				if(isPlayerPawn){
					hasPreviousMove = true;
					markPawnOnLcd(cords.x,cords.y,1);
				}
				
				sprintf(buf, "isPlayerPawn=%d\r\n", isPlayerPawn);
				sentStrDebug(buf);
			}
		} else if (beforeGame) {
			sprintf(buf, "1");
			sentStr(buf);
			beforeGame = false;
			isPlayerTurn = true;
		}
		sleep(10);
		LPC_GPIOINT->IO0IntClr = 1 << 19;
}

bool listenToEnemyStartPress() {
		char buf[100];
		while(1) {
		if (LPC_UART2->LSR & 1) {
			char input = LPC_UART2->RBR;
			strcat(buf, &input);
			sentStrDebug(buf);
			if (input == '1') {
				isPlayerTurn = false;
				beforeGame = false;
				buf[0] = '\0';
				sprintf(buf, "2");
				sentStr(buf);
				buf[0] = '\0';
				drawBoard();
				writeSentence("Enemy move!",100,110,LCDRed);
				return false;
			}
			else if(input == '2') {
				isPlayerTurn = true;
				beforeGame = false;
				buf[0] = '\0';
				drawBoard();
				writeSentence("Your move!",100,110,LCDRed);
				return false;
			}
			else {
				return true;
			}
		}
	}
}

void listenToEnemyMove() {
		char buf[100];
		while(1) {
		if (LPC_UART2->LSR & 1) {
			char input = LPC_UART2->RBR;
			if (input != '$') {
				strcat(buf, &input);
			} else {
				makeEnemyMove(buf);
				//makeEnemyMove("6,4,5,5|x,x \r\n");
				sentStrDebug(buf);
				isPlayerTurn = true;
				buf[0] = '\0';
				//sleep(50);
			}
		}
	}
}

void writeLetter(char letter, int x, int y, uint16_t color){
	uint8_t buffor[16];
	GetASCIICode(0, buffor, letter);
	
	int i,j,k;
	k = 0;
	for (i = 15; i >= 0; i--) {
		lcdWriteIndex(ADRX_RAM);
		lcdWriteData(k+y);
		k++;
		for (j = 8; j >= 0; j--) {
			lcdWriteIndex(ADRY_RAM);
			lcdWriteData(j+x);
			if (buffor[i] & (256 >> j)) {
				lcdWriteIndex(DATA_RAM);
				lcdWriteData(color);
			} 
		}
	}
}

void writeSentence(char* sent, int x, int y, uint16_t color){
	int i = 0;
	while(sent[i]!='\0'){
		writeLetter(sent[i],x+i*9,y,color);
		i++;
	}
}

int main() {
    toolsInit();
    preparePawns();
		drawBoard();
		writeSentence("Press to start!",90,110,LCDRed);
		//isPlayerTurn = false;
		
		
    //pawnsToUart();
		
		//bool beforeGame = true;
    while (beforeGame) {
			beforeGame = listenToEnemyStartPress();
    }
		sleep(50);
		drawBoard();
    drawPawns();
		sentStrDebug("wyszlo");
		while (1) {
			if (!isPlayerTurn) {
				listenToEnemyMove();
			}
		}
    return 0;
}
