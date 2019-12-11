#include "stm32l4xx.h"

// initialize center button for input
// setup interrupt
void button_Init();

//initialize B3 as LED output
void LED_init();

// set up SysTick, we will be needing a delay of a second through out this lab
// set sysTick to 1KHz
void SysTick_Init();


// delay function
void delay (int T);

// msticks
uint32_t ms = 0;
// enable transmit
char txEn = 0;
int data[] = {1,0,1,1,0,0,1,0};

int main(void) {
	button_Init();
	LED_init();
	SysTick_Init();
	int size = sizeof(data)/4;
	while(1) {
		if (txEn == 1) {
			for (int i = 0; i <= size; i++) {
				if (txEn == 0) break;

				if (data[i] == 1) GPIOB->ODR |= 1<<3;
				else GPIOB->ODR &= ~(1UL<<3);

				delay(1000);
			}
			GPIOB->ODR &= ~(1UL<<3);
			txEn = 0;
		}
	}
}

void delay (int T) {
	uint32_t startTime = ms;
	while(ms - startTime < T);
	ms = 0;
}

void EXTI0_IRQHandler() {
	if ( (EXTI->PR1 & EXTI_PR1_PIF0) == EXTI_PR1_PIF0) {
		if (txEn == 0) txEn = 1;
		else txEn = 0;

		EXTI->PR1 |= EXTI_PR1_PIF0;
	}

}

void SysTick_Handler() {
	ms++;
}

void SysTick_Init() {
	SysTick->CTRL = 0;
	SysTick->LOAD = 4000000*.001 - 1;
	SysTick->VAL =0;
	NVIC_SetPriority(SysTick_IRQn, 1);
	NVIC_EnableIRQ(SysTick_IRQn);
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void LED_init() {
	RCC->AHB2ENR |= 2;
	GPIOB->MODER &= ~(3UL<<6);
	GPIOB->MODER |= (1<<6);
	GPIOB->OTYPER &= ~(1<<3);
	GPIOB->OSPEEDR &= ~(3UL<<6);
	GPIOB->OSPEEDR |= 2;
	GPIOB->PUPDR &= ~(3UL<<6);
}

void button_Init() {
	// initialize pin
	RCC->AHB2ENR |= 1;
	GPIOA->MODER &= ~3UL;	// output mode
	GPIOA->OSPEEDR |= 3;	// v fast
	GPIOA->PUPDR &= ~3UL;		// no pullup/pulldown

	// initialze interrupt
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
//	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA; // set exti0 to PA
//	SYSCFG->EXTICR[0] &= ~(0xFFF0);
	EXTI->RTSR1 |= EXTI_RTSR1_RT0; 				  // Enable Rising Edge
	EXTI->FTSR1 &= ~EXTI_FTSR1_FT0;				  // Disable Falling Edge
	EXTI->IMR1 |= EXTI_IMR1_IM0;				  // Enable interrupt mask
	NVIC_SetPriority(EXTI0_IRQn, 2);			  // set priority
	NVIC_EnableIRQ(EXTI0_IRQn);					  // enable interrupt
}
