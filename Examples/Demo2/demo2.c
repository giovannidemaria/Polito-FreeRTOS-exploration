/* In this third demo, we use a timer to create a delay between output messages.
First, the delay is 1 second, and after each print, it is increased by 1.
In order to make it easier, we have defined a maximum number of reperition, 
but the same code, could be written for an undefined number of repetition of print.
*/

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Definizione della dimensione del buffer per le informazioni sui task */
#define TASK_INFO_BUFFER_SIZE 1024

/* Prototipo della funzione per il task di monitoraggio */
void vSystemStatusTask(void *pvParameters);

int demo2(){
    /* Inizializzazione del sistema e delle periferiche... */

    /* Creazione del task di monitoraggio dello stato del sistema */
    xTaskCreate(vSystemStatusTask, "SysStatus", 1000, NULL, 1, NULL);

    /* Avvio dello scheduler */
    vTaskStartScheduler();

    for (;;);
    return 0;
}

void vSystemStatusTask(void *pvParameters){
    (void) pvParameters;

    char buffer[TASK_INFO_BUFFER_SIZE];

    while (1)
    {
        /* Pulizia del buffer */
        memset(buffer, 0, TASK_INFO_BUFFER_SIZE);

        /* Ottenimento delle informazioni sui task */
        vTaskList(buffer);
        printf("Task States:\n%s\n", buffer);

        /* Ottenimento dell'uso della memoria heap */
        printf("Free Heap Size: %u bytes\n", xPortGetFreeHeapSize());

        /* Aggiungi qui altre metriche di stato del sistema se necessario */

        /* Attendi per un po' prima del prossimo aggiornamento */
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}