/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#define MAX_TASKS 3

SemaphoreHandle_t xSemaphores[MAX_TASKS];

void vTask();
void vTask2();
void vTask3();

void vTask3() {
    while(1) {
        if (xSemaphoreTake(xSemaphores[2], portMAX_DELAY) == pdTRUE) {
            printf("After 3 seconds...\n");
            printf("Wake up: semaphore 3\n");
            xSemaphoreGive(xSemaphores[0]);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
        vTask();
    }
}

void vTask2() {
    while(1) {
        if (xSemaphoreTake(xSemaphores[1], portMAX_DELAY) == pdTRUE) {
            printf("After 2 seconds...\n");
            printf("Wake up: semaphore 2\n");
            xSemaphoreGive(xSemaphores[2]);
        }

        vTaskDelay(pdMS_TO_TICKS(3000));
        vTask3();
    }
}

void vTask() {
    while(1) {
        if (xSemaphoreTake(xSemaphores[0], portMAX_DELAY) == pdTRUE) {
            printf("After 1 second...\n");
            printf("Wake up: semaphore 1\n");
            xSemaphoreGive(xSemaphores[1]);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
        vTask2();
    }
}

void demo1() {
    for (int i = 0; i < MAX_TASKS; i++) {
        xSemaphores[i] = xSemaphoreCreateMutex();
        if (xSemaphores[i] == NULL) {
            printf("ERROR: bad semaphore creation.\n");
        }
    }
    
    xTaskCreate(vTask, "Semaphore", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    /* Starting FreeRTOS scheduler */
    vTaskStartScheduler();
    
    for(;;);
}