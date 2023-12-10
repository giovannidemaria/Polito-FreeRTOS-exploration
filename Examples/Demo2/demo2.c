/* In this first demo, we use a timer to create a delay between output messages.
First, the delay is 1 second, and after each print, it is increased by 1.
In order to make it easier, we have defined a maximum number of reperition, 
but the same code, could be written for an undefined number of repetition of print.
*/

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"



void demo2() {

printf("Ciao Mondo!");


}