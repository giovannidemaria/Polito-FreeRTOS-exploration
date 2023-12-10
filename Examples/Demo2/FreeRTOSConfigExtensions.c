/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdio.h>
#include <string.h>


void portCONFIGURE_TIMER_FOR_RUN_TIME_STATS(void){
    /* Example using SysTick as the timer */
    /* Configure SysTick to interrupt at a specific frequency */
    SysTick_Config(SystemCoreClock / 1000); // Example: SysTick at 1 ms
}

unsigned long portGET_RUN_TIME_COUNTER_VALUE(void){
    /* Return the SysTick current value. */
    return SysTick->VAL;
}