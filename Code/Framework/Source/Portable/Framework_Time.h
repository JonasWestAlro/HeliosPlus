#ifndef FRAMEWORK_TIME
#define FRAMEWORK_TIME

#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

void Framework_Time_Init(void);
uint32_t Framework_Time_GetTimeUs(void);

#endif
