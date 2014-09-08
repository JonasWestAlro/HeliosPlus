#ifndef RTOSHooks
#define RTOSHooks

#include "stm32f4xx.h"

//Defines:
#define ENTER_CRITICAL()		__disable_irq();
#define EXIT_CRITICAL()			__enable_irq();

#define ASSERT(_test)			if(!_test){onAssert(x)} else{}
#define REQUIRE(_test)			ASSERT((_test))
#define ENSURE(_test)			ASSERT((_test))
#define INVARIANT(_test)		ASSERT((_test))

#define BSRR_VAL        0x0300

void vApplicationStackOverflowHook();
void vApplicationTickHook(void *p);
void vApplicationIdleHook(void *p);
void onAssert(void);
void vApplicationMallocFailedHook();
#endif
