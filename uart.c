#include <stdio.h>
#include <Board_LED.h>
#include <Board_Joystick.h>
#include <LPC17xx.h>
#include <PIN_LPC17xx.h>

int sleep3() {
	int times = 10000000;
	int i = 0;
	for (i = 0; i < times; i++) {
		i += 1;
	}
	return 0;
}

void sent_str2(char* str) {
	while (*str != NULL) {
		if (LPC_UART0->LSR & 64) {
			LPC_UART0->THR = *str;
			str++;
		}
	}
}

int main3() {
	LPC_UART0->LCR = 1 << 7;
	LPC_UART0->DLM = 0;
		
	LPC_UART0->DLL = 12;
	LPC_UART0->FDR = 15 << 4 | 2;
	LPC_UART0->LCR = 1 | 1<<1 | 1<<2;
	PIN_Configure (0, 2, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
	PIN_Configure (0, 3, PIN_FUNC_1, PIN_PINMODE_REPEATER, PIN_PINMODE_NORMAL);
	
	/*
	while(1) {
		if (LPC_UART0->LSR & 1) {
			char input = LPC_UART0->RBR;
			LPC_UART0->THR = input + 1;
		}
	}
	*/
	
	char str[] = "1234567890123456789012345";
	sent_str(str);
	
	return 0;
}

// RBR, THR, DLL, DLM, LCR
// flaga DLAB
// LCR = 0b_0000111

//LCR = 1 << 7
//DLM = 0
//DLL = 12
//
//DIV = 2 
//MUL = 15 
//FDR = MUL DIV