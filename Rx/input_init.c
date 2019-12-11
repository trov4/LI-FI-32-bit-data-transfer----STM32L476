// includes
/*************************************************************/
#include "input_init.h"
#include "EXTI.h"
#include "LED.h"
/*************************************************************/

// linked variables
/*************************************************************/
//microSec = 0;
extern uint32_t microSec;
extern int statusState;
/*************************************************************/

// global variables
/*************************************************************/
uint32_t currFreq = 0, previousTime = 0;

//predetermined frequency values
uint32_t Preamble_Period = 37;
uint32_t Low_Period = 45;
uint32_t High_Period = 30;

//buffer to account for accuracy
uint32_t accBuffer = 3;
/*************************************************************/

//Configure pin for interrupt
void EXTI3_init() {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;				// heart beat
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;			// clear selector for EXTI3
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB;		// select pin B3
	EXTI->RTSR1 |= EXTI_RTSR1_RT3;						// enable rising edge trigger
	EXTI->FTSR1 &= ~(EXTI_FTSR1_FT3);						// disable falling
	EXTI->IMR1 |= EXTI_IMR1_IM3;						// enable mask register
	NVIC_SetPriority(EXTI3_IRQn, 3);					// set priority to 3 -- less than Systick Priority
	NVIC_EnableIRQ(EXTI3_IRQn);							// enable that boi
}

//Handle interrupt
void EXTI3_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF3) == EXTI_PR1_PIF3) {
		// do stuff here when input is triggered
		int currPeriod = microSec - previousTime;//determine difference in time from received signals
		currFreq = 1/currPeriod;//determine frequency

		if ((currPeriod <= Preamble_Period + accBuffer) && (currPeriod >= Preamble_Period  - accBuffer))//if the value is within the range for preamble
		{
			statusState = 2;
		}

		else if ((currPeriod <= High_Period + accBuffer)  && (currPeriod >= High_Period - accBuffer))//if the value is within the range for high(1)
		{
			statusState = 1;
		}

		else if ((currPeriod <= Low_Period + accBuffer)  && (currPeriod >= Low_Period - accBuffer))//if the value is within the range for low(0)
		{
			statusState = 0;
		}

		else//place the state in standby if none of the values are useful
		statusState = 3;

		previousTime = microSec;//set previous time to current time in SysTick


		// turn off interrupt
		EXTI->PR1 |= EXTI_PR1_PIF3;
	}
	/*else {
		//Red_LED_Off();
		statusState = 0;
	}*/
}
