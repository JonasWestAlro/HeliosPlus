#include "RTOSHooks.h"

void vApplicationStackOverflowHook(){
	while(1);
}


void vApplicationTickHook(void *p)
{
	//++msTicks;
}

void vApplicationIdleHook(void *p){
	static int counter=0;

	for(;;)
	{
		counter++;
		//__WFI();
	}
}

void onAssert(){
	ENTER_CRITICAL();
	while(1){
	}
}

void vApplicationMallocFailedHook(){
	while(1);
}
