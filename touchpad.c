#include <Board_LED.h>
#include <Board_Joystick.h>
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include "TP_Open1768.h"

int sleep5() {
	int times = 10000000;
	int i = 0;
	for (i = 0; i < times; i++) {
		i += 1;
	}
	return 0;
}

void sent_str5(char* str) {
	while (*str != NULL) {
		if (LPC_UART0->LSR & 64) {
			LPC_UART0->THR = *str;
			str++;
		}
	}
}

int EINT3_IRQHandler6() {
	sent_str5("a");
	LPC_GPIOINT->IO0IntClr = 1 << 19;
}
	

int main5() {
	LPC_UART0->LCR = 1 << 7;
	LPC_UART0->DLM = 0;
	LPC_UART0->DLL = 12;
	LPC_UART0->FDR = 15 << 4 | 2;
	LPC_UART0->LCR = 1 | 1<<1 | 1<<2;
	PIN_Configure (0, 2, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
	PIN_Configure (0, 3, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
	
	touchpanelInit();
	
	
	LPC_GPIOINT->IO0IntEnF = 1 << 19;
	NVIC_EnableIRQ(EINT3_IRQn);
	//NVIC_DisableIRQ(EINT3_IRQn);
	
	
	while(1) {
		/*
		char buf[100];
		int x = touchpanelReadX();
		int y = touchpanelReadY();
		sprintf(buf, "x=%d, y=%d\r\n", x, y);
		sent_str(buf);
		sleep();
		*/
	}
	return 0;
}

