#include <stdio.h>
#include <Board_LED.h>
#include <Board_Joystick.h>
#include <LPC17xx.h>

int sleep2() {
	int times = 10000000;
	int i = 0;
	for (i = 0; i < times; i++) {
		i += 1;
	}
	return 0;
}

int leds(int num) {
	//LED_SetOut(num);
	LPC_GPIO0->FIOPIN0 = ~num;
	sleep2();
	return 0;
}

int ledsDance() {
		while(1) {
			leds(0b1010);
			leds(0b0101);
		}
		return 0;
}

int setLedsOnJoy(int prevState) {
	int state = Joystick_GetState();
	//if (state != prevState) {
	//	state += 1;
	//	leds(state);
	//}
	leds(state);
	return state;
}

int init() {
	//LED_Initialize();
	LPC_GPIO0->FIODIR0 = 0b1111;
	Joystick_Initialize();
	return 0;
}


int main2() {
	init();
	int prevState = 0;
	int ledState = 0;
	while(1) {
		//prevState = setLedsOnJoy(prevState, ledState);
		setLedsOnJoy(prevState);
		
		

	//int state = Joystick_GetState();
	//if (state != prevState) {
	//	state += 1;
	//	leds(state);
	//}
	//return state;

		
		
					
	}
	return 0;
}
