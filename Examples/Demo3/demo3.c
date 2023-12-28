/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#define MAX_TASKS 3
#define MAX_COUNT 1

SemaphoreHandle_t xSemaphores[MAX_TASKS];

void vTask();
void vTask2();
void vTask3();

void vTask3() {
    while(1) {
        if (xSemaphoreTake(xSemaphores[2], portMAX_DELAY) == pdTRUE) {
            printf("After 3 seconds...\n");
            vTaskDelay(pdMS_TO_TICKS(3000));
            printf("Wake up: semaphore 3\n");
            xSemaphoreGive(xSemaphores[0]);
        }

    }
}

void vTask2() {
    while(1) {
        if (xSemaphoreTake(xSemaphores[1], portMAX_DELAY) == pdTRUE) {
            printf("After 2 seconds...\n");
            vTaskDelay(pdMS_TO_TICKS(2000));
            printf("Wake up: semaphore 2\n");
            xSemaphoreGive(xSemaphores[2]);
        }

    }
}

void vTask() {
    while(1) {
        if (xSemaphoreTake(xSemaphores[0], portMAX_DELAY) == pdTRUE) {
            printf("After 1 second...\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            printf("Wake up: semaphore 1\n");
            xSemaphoreGive(xSemaphores[1]);
        }

    }
}

void demo3() {
    for (int i = 0; i < MAX_TASKS; i++) {
        xSemaphores[i] = xSemaphoreCreateCounting(MAX_COUNT, 0);
        if (xSemaphores[i] == NULL) {
            printf("ERROR: bad semaphore creation.\n");
        }
    }
    xSemaphoreGive(xSemaphores[0]);
    
    xTaskCreate(vTask, "Semaphore", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vTask2, "Semaphore", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vTask3, "Semaphore", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    /* Starting FreeRTOS scheduler */
    vTaskStartScheduler();
    
    for(;;);
}


// modifica per vedere se riesco a creare il mio branch
