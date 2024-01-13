#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include <stdlib.h>

QueueHandle_t xQueue;

int cmp_value(const void* v1, const void* v2) {
    return *(int*)v1-*(int*)v2;
}

int valori[] = {17, 8, 10};

int taskProva(void *pvParameter) {
    const char *name = pcTaskGetName(NULL);
    printf("Esecuzione Task %s", name);
}

int provaScheduler() {
    xQueue = xQueueCreate(3, sizeof(int));

    qsort(valori, 3, sizeof(int), cmp_value);
    char name[16];

    for(int i = 0; i < 3; i++) {
        sprintf(name, "Task%d", i);
        xTaskCreate(taskProva, name, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2 - i, NULL);
    }

    vTaskStartScheduler();

    for(;;);
}

