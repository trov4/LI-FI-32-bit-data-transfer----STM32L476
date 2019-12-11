#ifndef __STM32L476G_DISCOVERY_EXTI_H
#define __STM32L476G_DISCOVERY_EXTI_H

#include "stm32l476xx.h"

char txEn;					// txEn = 0: don't transmit txEn = 1: transmit -- toggled by button

void EXTI_Init(void);

#endif /* __STM32L476G_DISCOVERY_DMA_H */
