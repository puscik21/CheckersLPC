#include <Board_LED.h>
#include <Board_Joystick.h>
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include "TP_Open1768.h"

int const SQUARE_SIZE_X = 24;
int const SQUARE_SIZE_Y = 32;

int sleep() {
	int times = 10000000;
	int i = 0;
	for (i = 0; i < times; i++) {
		i += 1;
	}
	return 0;
}

int uartInit() {
	LPC_UART0->LCR = 1 << 7;
	LPC_UART0->DLM = 0;
	LPC_UART0->DLL = 12;
	LPC_UART0->FDR = 15 << 4 | 2;
	LPC_UART0->LCR = 1 | 1<<1 | 1<<2;
	PIN_Configure (0, 2, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
	PIN_Configure (0, 3, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
}

void sent_str(char* str) {
	while (*str != NULL) {
		if (LPC_UART0->LSR & 64) {
			LPC_UART0->THR = *str;
			str++;
		}
	}
}

int EINT0_IRQHandler () {
	LPC_SC->EXTINT = 1;
	sent_str("a");
}

int writeSquareLine(uint16_t colorIndex) {
	int j = 0;
	for (j = 0; j < SQUARE_SIZE_X; j++) {
		lcdWriteIndex(DATA_RAM);
		lcdWriteData(colorIndex);
	}
}

int prepareBoard() {
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
					writeSquareLine(LCDBlueSea);
					writeSquareLine(LCDWhite);
				} else {
					writeSquareLine(LCDWhite);
					writeSquareLine(LCDBlueSea);
				}
			}
		}
	}
}

int main() {
	uartInit();
	lcdConfiguration();
	uint16_t val = lcdReadReg(OSCIL_ON);
	init_ILI9325();
	prepareBoard();
	
	while(1) {

	}
	return 0;
}

