/*  Demo 3
    Gestione pronto soccorso
    
    L'ospedale ha "MAX_SALE" disponibili per operare i pazienti
    ogni sala opera in parallelo rispetto alle altre.
    
    Ai pazienti viene assegnato un colore che identifica la gravità e un tempo entro il quale
    se l'operazione non viene conclusa la sitauzione del paziente si aggrava:
    
            codice verde --> codice arancione (10 secondi)
            codice arancione --> codice rosso (5 secondi)
            codice rosso --> paziente morto (2.5 secondi)

    
    Ipotesi: 
        - quando un paziente entra in una delle sale operatorie, non può uscire prima che 
    l'operazione sia terminata
        - per semplicità ad ogni codice è assegnato un tempo di durata dell'operazione 
        - i pazienti vengono operati per prorità di colore e differenza tra tempo necessario all'operazione
        di operazione e tempo di arrivo 



    Le sale operatorie sono identificate come una coda di MAX_SALE elementi

    I pazienti sono rappresentati con tre code (in base al colore associato al paziente)

    Attraverso i semafori si gestisce quale delle code dei pazienti entra in sala operatoria.
*/

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"

#define MAX_TASKS 3
#define MAX_COUNT 1
#define MAX_SALE 2
#define PATIENT 9

#define RED_OPERATION_TICK 5
#define ORANGE_OPERATION_TICK 4
#define GREEN_OPERATION_TICK 3

// codice, tempo di arrivo
int patientMatrix[PATIENT][2] = {
    {0, 2},
    {1, 6},
    {2, 12},
    {0, 14},
    {0, 18},
    {2, 22},
    {1, 26},
    {2, 28},
    {0, 18},
};


// la coda identifica le sale operatorie dell'ospedale

/* creazione e riempimento code pazienti */
void v0Task( void *pvParameters )
{
QueueHandle_t xQueue[3];

    /* Create a queue capable of containing 10 unsigned long values. */
    xQueue[0] = xQueueCreate( 10, sizeof( int ) );
    xQueue[1] = xQueueCreate( 10, sizeof( int ) );
    xQueue[2] = xQueueCreate( 10, sizeof( int ) );

    if( xQueue[0] == NULL || xQueue[1] == NULL  || xQueue[2] == NULL)
    {
        /* Queue was not created and must not be used. */
        printf("ERROR: bad queue creation.\n");
        exit(-1);
    }

    /* Riempimento coda */
    for(int i=0; i < PATIENT; i++){
         /* Send an integer. Wait for 10 ticks for space to become available if necessary. */
        if(xQueueSendToBack( xQueue[patientMatrix[i][0]], ( void * ) &patientMatrix[i][1], ( TickType_t ) 10 ) != pdPASS )
                printf("ERROR: impossibile inserire valore in coda.\n");
        
    }

    /* Ottieni il numero di pazienti in coda nella coda */
    UBaseType_t spaces[3];
    spaces[0] = uxQueueSpacesAvailable(xQueue[0]);
    spaces[1] = uxQueueSpacesAvailable(xQueue[1]);
    spaces[2] = uxQueueSpacesAvailable(xQueue[2]);

    printf("Pazienti in coda con codice verde: %u\n", 10 - (unsigned long)spaces[0]);
    printf("Pazienti in coda con codice arancione: %u\n", 10 - (unsigned long)spaces[1]);
    printf("Pazienti in coda con codice rosso: %u\n", 10 - (unsigned long)spaces[2]);
 }


// controlla il rosso
    //finchè la coda rossa non è vuota
        //preleva rosso 


// controlla coda arancione
    //finchè la coda rossa è vuota preleva arancione
        //preleva arancione


// controlla coda verde
        //finchè la coda rossa è vuota e la coda arancione è vuota
        //preleva verde
















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
/*
void vTimerCallback(TimerHandle_t xTimer) {
    if (repetitions < MAX_REPS) {
        printf("Message delay: %d\n", currentDelay);
        repetitions++;
        currentDelay += 1000;
        xTimerChangePeriod(xTimer, pdMS_TO_TICKS((currentDelay)), 0);
    } else {
        xTimerStop(xTimer, 0);
    }
}
*/


void vTask4() {
    TickType_t currentTicks;
    while(1) {
        currentTicks = xTaskGetTickCount();
    printf("Tempo trascorso in tick dall'avvio dello scheduler: %u\n", currentTicks);
    }
}

void someFunction() {

}

void demo3() {
    for (int i = 0; i < MAX_TASKS; i++) {
        xSemaphores[i] = xSemaphoreCreateCounting(MAX_COUNT, 0);
        if (xSemaphores[i] == NULL) {
            printf("ERROR: bad semaphore creation.\n");
        }
    }
    xSemaphoreGive(xSemaphores[0]);
    
    //xTaskCreate(vTask, "Semaphore", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    //xTaskCreate(vTask2, "Semaphore", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    //xTaskCreate(vTask3, "Semaphore", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(v0Task, "Queue", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vTask4, "Timer", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    /* Starting FreeRTOS scheduler */
    vTaskStartScheduler();
    
    for(;;);
}





/*
Esempio per implementare un ulteriore feature: coda --> vettore ordinato

L'ospedale è pieno al tempo t1 10 sec e t2 15 sec arrivano due codici rossi che sono  in coda
il paziente 1 ha bisogno di un operazione della durata di 10 seoncdi, e il paziente 2 di un 
operazione della durata di 20 secondi.

Ipotesi se il paziente 1 muore se non è curato in 50 secondi il paziente 2 muore se non curato in 30 secondi

Al tempo 20 secondi si libera una sala:
    - p1: 20 + 10 --> fine operazione: 30 sec (30-10=20 < 50) -->vive
    - p2: 30 + 20 --> fine operazione: 50 sec (50-15=35 > 30) -->muore

    - p2: 20 + 20 --> fine operazione: 40 sec (40-15=25 < 30) -->vive
    - p1: 40 + 10 --> fine operazione: 50 sec (50-10=40 < 50) -->vive


Questo esempio mette il luce che se ad ogni paziente oltre che a un tempo di durata dell'operazione
viene assegnato un particolare tempo trascorso il quale il paziente muore allora la coda può essere migliorata
(perdendo il significato di coda e diventando un vettore ordinato)
*/
