#include "stm32l4xx.h"

// initializations
/*************************************************************/
// initialize center button for input
// setup interrupt
void button_Init();

//initialize B3 as LED output &light on/ off functions
void LED_init();

// set up SysTick set sysTick to 1KHz
void SysTick_Init();

// convert decimal to binary array
void decimalToBinary(int n);
/*************************************************************/

// functions used to transmit
/*************************************************************/
void LED_on();
void LED_off();

// 10x light on for period/2 -> light off for period/2
void tx(int period);

// delay function
void delay (int T);
/*************************************************************/


//global variables
/*************************************************************/
uint32_t ms = 0;
char txEn = 0;

// this is where data is set
// the values below are the different values used in our demo video
//int data = 0b10001111010100001010111101011011;
//int data = 0;
//int data = 0xFFFFFFFF;
int data = 0x8A7B9F10;
/*************************************************************/
