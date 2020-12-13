#include <Board_LED.h>
#include <Board_Joystick.h>
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include "TP_Open1768.h"

int sleep6() {
	int times = 10000000;
	int i = 0;
	for (i = 0; i < times; i++) {
		i += 1;
	}
	return 0;
}

void sent_str6(char* str) {
	while (*str != NULL) {
		if (LPC_UART0->LSR & 64) {
			LPC_UART0->THR = *str;
			str++;
		}
	}
}

int TIMER0_IRQHandler() {
	LPC_TIM0->IR = 1;
	sent_str6("a");
}

int main6() {
	LPC_UART0->LCR = 1 << 7;
	LPC_UART0->DLM = 0;
	LPC_UART0->DLL = 12;
	LPC_UART0->FDR = 15 << 4 | 2;
	LPC_UART0->LCR = 1 | 1<<1 | 1<<2;
	PIN_Configure (0, 2, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
	PIN_Configure (0, 3, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
	
	
	LPC_TIM0->MR0 = 12500000;
	LPC_TIM0->MCR = 1 << 1 | 1 << 0;
	LPC_TIM0->TCR = 1;
	
	NVIC_EnableIRQ(TIMER0_IRQn);
	
	while(1) {

	}
	return 0;
}

