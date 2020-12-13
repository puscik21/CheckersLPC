#include <Board_LED.h>
#include <Board_Joystick.h>
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"

int sleep4() {
	int times = 10000000;
	int i = 0;
	for (i = 0; i < times; i++) {
		i += 1;
	}
	return 0;
}

int main4() {
	lcdConfiguration();
	uint16_t val = lcdReadReg(OSCIL_ON);
	init_ILI9325();
	
	uint8_t buffor[16];
	GetASCIICode(0, buffor, 'G');
	
	lcdWriteIndex(ADRX_RAM);
	lcdWriteData(0);
	lcdWriteIndex(ADRY_RAM);
	lcdWriteData(0);
	lcdWriteIndex(DATA_RAM);
	lcdWriteData(LCDWhite);
	
	int i = 0;
	int j = 0;
	int times = 240 * 320;
	for (int i = 0; i < times; i++) {
		lcdWriteData(LCDWhite);
	}
	
	for (i = 0; i < 16; i++) {
		lcdWriteIndex(ADRY_RAM);
		lcdWriteData(i);
		for (j = 0; j < 8; j++) {
			lcdWriteIndex(ADRX_RAM);
			lcdWriteData(j);
			if (buffor[i] & (256 >> j)) {
				lcdWriteIndex(DATA_RAM);
				lcdWriteData(LCDBlack);
			} 
		}
	}
	
	
	
	/*	
	lcdWriteIndex(ADRX_RAM);
	lcdWriteData(0);
	lcdWriteIndex(ADRY_RAM);
	lcdWriteData(160);
	lcdWriteIndex(DATA_RAM);
	lcdWriteData(LCDRed);
	
	int i = 0;
	int times = 240 * 320 / 2;
	for (int i = 0; i < times; i++) {
		lcdWriteData(LCDRed);
	}
	*/
	
	/*
	void lcdWriteIndex(uint16_t index);
void lcdWriteData(uint16_t index);
uint16_t lcdReadData(void);
	*/
	
	return 0;
}

