// includes
/*************************************************************/
#include "stm32l4xx.h"
#include "LED.h"
#include "EXTI.h"
#include "input_init.h"
/*************************************************************/

/*
 * Outline:
 * The program will wait for a User to press the button
 * When the user does, The transmission will begin
 * Every bit will be prefaced with the preamble for 10 periods
 * Then the slow frequency will represent 0 and the high frequency will
 * represent 1, each sent for 10 periods
 */

// initializations
/*************************************************************/
//initialize B3 as the photoresistor input
void Input_init();

// set up SysTick set sysTick to 1KHz
void SysTick_Init();
/*************************************************************/

//global variables
/*************************************************************/
uint32_t ms = 0;					// counts up with systick			// data to be sent
char values[32]; //time between different each
int value;
delayCounter = 0;
uint32_t counter = 0;
int timerState = 0;
int registerTwo = 0;
/*************************************************************/

//linked variables
/*************************************************************/
extern uint32_t microSec;
extern int statusState;
/*************************************************************/

int main(void) {
	// initializations
	/*************************************************************/
	EXTI3_init();
	Input_init();
	SysTick_Init();
	EXTI_Init();
	LED_Init();
	/*************************************************************/

	/*Status State machine:
	 * 0.) Low
	 * 1.) High
	 * 2.) Preamble
	 * 3.)Standby
	 */

	statusState = 3;//set the state to standby
	int count = 0;//counter used to iterate through array
	int preambleCounter = 0;//counter to determine number of preambles detected

	while(1) {
		//Only receive signals if we haven't reached the end of the array
		if (count <32){
			//Preamble
			if (statusState == 2){//if receive a preamble signal
				//Red_LED_Off();
				//Green_LED_Off();
				Red_LED_On();//debug lights
				Green_LED_On();//debug lights
				preambleCounter++;//increment preamble counter
				statusState = 3;//reset the state to standby
			}
			//Low
			else if (statusState == 0){//if receive a low(0) signal
				Green_LED_Off();//debug lights
				Red_LED_On();//debug lights
				statusState = 3;//reset the state to standby
				if (preambleCounter>= 1)//if minimum number of preambles are met
					{
						values[count] = 0;//make the value in array = 0
						count++;//inc count
					}
			}
			//High
			else if (statusState == 1){//if receive a high(1) signal
				Red_LED_Off();//debug lights
				Green_LED_On();//debug lights
				statusState = 3;//reset the state to standby
				if (preambleCounter>= 1)//if minimum number of preambles are met
					{
						values[count] = 1;//make the value in array = 1
						count++;//inc count
					}
				}
			}
		//if we reached the end of the array(count >=32), convert the binary array into an integer and place the value in r0
		else
		{
			int multiplier = 1;//multiplier for conversion
			Red_LED_Off();//debug lights
			Green_LED_Off();//debug lights
			for (int i = 0; i <=31; i++ )//iterate through array
			{
				registerTwo += (multiplier * values[i]);//multiply array value and multiplier and add to registerTwo to get the integer value
				multiplier *= 2;//increase multiplier by a multiple of 2
			}
			write(registerTwo);//place the value in r0

			int x = 1+1;//fluff, the code is done :)
		}
	}
}

/*
 * Increments values every systick
 */
void SysTick_Handler() {
	ms++;
	microSec++;//value used by input_init.c
}

/*
 * Initializes the SysTick
 */
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

/*
 * Initializes the Input GPIO for the photocell
 */
void Input_init() {
	RCC->AHB2ENR |= 2;
	GPIOB->MODER &= ~(3UL<<6);
	GPIOB->PUPDR &= ~(3UL<<6);
}
