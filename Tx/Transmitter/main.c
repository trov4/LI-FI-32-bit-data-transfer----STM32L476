#include "main.h"

/*
 * Outline:
 * The program will wait for a User to press the button
 * When the user does, The transmission will begin
 * with a preamble frequency
 * then it will modulate the frequency based on
 * the data value
 */

extern uint32_t ms;						// counts up with systick
extern char txEn;						// txEn = 0: don't transmit txEn = 1: transmit -- toggled by button
extern int data;						// data to be sent
int dataToSend[32];

int main(void) {
	// initializations
	/*************************************************************/
	button_Init();					// initializes the button to start sending the the message
	LED_init();						// initializes PB.3 as the LED output
	SysTick_Init();					// initializes systick to tick every milliS
	int size = sizeof(data)/4;		// size of data

	// low_freq = 0b0, pre_freq = the frequency for preamble, high_freq = 0b1
	// these aren't used except for period calculations, mainly for reference
	double low_freq = 22;
	double pre_freq = 27;
	double high_freq = 33;

	// periods for above frequencies in milliseconds
	// note, period in milliS = 1000 * 1/freq = 1000/freq
	int low_period = 1000/(double)(low_freq);
	int pre_period = 1000/(double)(pre_freq);
	int high_period = 1000/(double)(high_freq);

	// set R0 and convert decimal number to binary
	decimalToBinary(data);								// convert to array of bits
	load();												// .s file -- load data value into R0
	/*************************************************************/
	int x = 0;											// used as a breakpoint -- arbitrary
	/*State machine:
	 * 1.) wait
	 * 2.) send preamble
	 * 3.) send number
	 * 4.) clear
	 */

	while(1) {
		// state 1 -- wait
		if (txEn == 1) {

			// state 2 -- preamble
			tx(pre_period);

			// state 3 -- send data
			for (int i = 0; i <= 32; i++) {
				int send = dataToSend[i];
				// if button pressed again -- hault
				if (txEn == 0) break;

				if (send == 0) tx(low_period);			// send 0
				else tx(high_period);					// or send 1
			}
			// state 4 -- clear
			LED_off();
			txEn = 0;
		}
	}
}

void decimalToBinary(int n)
{
	// convert to binary
	for (int i = 31; i >= 0; i--) {
	        int k = n >> i;
	        if (k & 1)
	        	dataToSend[i] =1;
	        else
	        	dataToSend[i] =0;
	    }
}


void delay (int T) {
	// delay in milliseconds
	uint32_t startTime = ms;
	while(ms - startTime < T);
	ms = 0;
}

void LED_on() {
	// on
	GPIOB->ODR |= 1<<3;
}

void LED_off() {
	// off
	GPIOB->ODR &= ~(1UL<<3);
}

void tx(int period) {
	// 50 % duty cycle given a period
	LED_on();
	delay(period/2);
	LED_off();
	delay(period/2);
}

void EXTI0_IRQHandler() {
	if ( (EXTI->PR1 & EXTI_PR1_PIF0) == EXTI_PR1_PIF0) {
		// swap send or clear state
		if (txEn == 0) txEn = 1;
		else txEn = 0;

		// clear out interrupt
		EXTI->PR1 |= EXTI_PR1_PIF0;
	}

}

void SysTick_Handler() {
	ms++;
}

void SysTick_Init() {
	SysTick->CTRL = 0;									// start at 0
	SysTick->LOAD = 4000000*.001 - 1;					// set period to 1 ms
	SysTick->VAL =0;									// start at 0
	NVIC_SetPriority(SysTick_IRQn, 2);					// priority after button
	NVIC_EnableIRQ(SysTick_IRQn);						// enable
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;			// enable tickint
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		// set clock source
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;			// enable that boi
}

void LED_init() {
	// initialize PB.3 as output
	RCC->AHB2ENR |= 2;
	GPIOB->MODER &= ~(3UL<<6);
	GPIOB->MODER |= (1<<6);				// output mode
	GPIOB->OTYPER &= ~(1<<3);			// set otype
	GPIOB->OSPEEDR &= ~(3UL<<6);
	GPIOB->OSPEEDR |= 2;				// pretty fast
	GPIOB->PUPDR &= ~(3UL<<6);			// no pull up/pull down
}

void button_Init() {
	// initialize pin
	RCC->AHB2ENR |= 1;
	GPIOA->MODER &= ~3UL;		// input mode
	GPIOA->OSPEEDR |= 3;		// v fast
	GPIOA->PUPDR &= ~3UL;		// no pullup/pulldown

	// initialze interrupt
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;			// heart beat
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	EXTI->RTSR1 |= EXTI_RTSR1_RT0; 				  	// Enable Rising Edge
	EXTI->FTSR1 &= ~EXTI_FTSR1_FT0;				  	// Disable Falling Edge
	EXTI->IMR1 |= EXTI_IMR1_IM0;				  	// Enable interrupt mask
	NVIC_SetPriority(EXTI0_IRQn, 1);			  	// set priority
	NVIC_EnableIRQ(EXTI0_IRQn);					  	// enable interrupt
}
