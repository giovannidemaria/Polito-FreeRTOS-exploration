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
#include <string.h>

#define MAX_TASKS 3
#define MAX_COUNT 1
#define RED_OPERATION_TICK (5 * configTICK_RATE_HZ)
#define ORANGE_OPERATION_TICK 4 
#define GREEN_OPERATION_TICK (3 * configTICK_RATE_HZ)


#define PATIENT 9
#define MAX_SALE 2

int greenPatients[PATIENT] = {2,6,12,14,18,22,26,28,32};
int redPatients[PATIENT] = {3,5,10,17,19,22,24,38,41};


/* Struttura per passare i parametri alla funzione di riempimento della coda */
typedef struct {
    QueueHandle_t queue;
    int dataArray[PATIENT];
} QueueFillParameters_t;

QueueFillParameters_t redParams;
QueueFillParameters_t greenParams;

// Funzione per riempire una coda 
void fillQueue( void *pvParameters ) {
    QueueFillParameters_t *params = (QueueFillParameters_t *)pvParameters;
    const char *taskName = pcTaskGetName(NULL);
    const char *message;
    if (strcmp(taskName, "GreenFill") == 0) {
        message = "verde";
    } else {
        message = "rosso";
    }

    // Tempo di inizio
    TickType_t xStartTime = xTaskGetTickCount(); 

        for (int i = 0; i < PATIENT; i++) {
        // Calcola il tempo di attesa in tick dal momento di inizio del task
        TickType_t delay = params->dataArray[i] * configTICK_RATE_HZ;
        TickType_t xNow = xTaskGetTickCount();
        TickType_t xWait = (xStartTime + delay > xNow) ? (xStartTime + delay - xNow) : 0;

        // Aspetta fino al momento specificato
        vTaskDelay(xWait);

        // Invia il dato alla coda
        if (xQueueSend(params->queue, &(params->dataArray[i]), portMAX_DELAY) != pdPASS) {
            printf("Errore nell'inviare i dati alla coda\n");
        }

        printf("Al secondo %d è arrivato un paziente catalogato come codice %s\n", params->dataArray[i], message);
    }


    for( ;; );
}

TimerHandle_t xTimers[MAX_SALE];
SemaphoreHandle_t xSemaphoreSaleOperatorie;

// Callback del timer che segnala il completamento di un'operazione
void operationCompleteCallback(TimerHandle_t xTimer) {
    int salaId = (int)pvTimerGetTimerID(xTimer);
    printf("Fine operazione nella sala %d\n", salaId);
    xSemaphoreGive(xSemaphoreSaleOperatorie);
}

void salaOperatoriaTask(void *pvParameters) {
    int paziente;
    char codicePaziente;

    for (;;) {
        // Imposta un flag per indicare se un paziente è stato trovato
        BaseType_t pazienteTrovato = pdFALSE;

        if (uxQueueMessagesWaiting(redParams.queue) > 0) {
            if (xQueueReceive(redParams.queue, &paziente, 0) == pdTRUE) {
                codicePaziente = 'R';
                pazienteTrovato = pdTRUE;
            }
        } else if (uxQueueMessagesWaiting(greenParams.queue) > 0) {
            if (xQueueReceive(greenParams.queue, &paziente, 0) == pdTRUE) {
                codicePaziente = 'G';
                pazienteTrovato = pdTRUE;
            }
        }

        // Avvia l'operazione solo se un paziente è stato effettivamente trovato
        if (pazienteTrovato && xSemaphoreTake(xSemaphoreSaleOperatorie, 0) == pdTRUE) {
            TickType_t operationTicks = (codicePaziente == 'R') ? RED_OPERATION_TICK : GREEN_OPERATION_TICK;
            
            for (int i = 0; i < MAX_SALE; i++) {
                if (xTimerIsTimerActive(xTimers[i]) == pdFALSE) {
                    printf("Inizio operazione su paziente %d, nella sala %d\n", paziente,i);
                    xTimerChangePeriod(xTimers[i], operationTicks, 0);
                    xTimerReset(xTimers[i], 0);
                    vTimerSetTimerID(xTimers[i], (void *)(intptr_t)i);
                    break;
                }
            }
        } else {
            // Se non ci sono pazienti, il task attende un breve periodo prima di riprovare
            vTaskDelay(100);
        }
    }
}

/*
void salaOperatoriaTask(void *pvParameters) {
    int paziente;
    for (;;) {
        // Aspetta finché non ci sono pazienti in una delle due code
        if (uxQueueMessagesWaiting(redParams.queue) == 0 && uxQueueMessagesWaiting(greenParams.queue) == 0) {
            vTaskDelay(100); // Attesa prima di controllare di nuovo
        }

        // Cerca prima un paziente nella coda rossa
        if (uxQueueMessagesWaiting(redParams.queue) > 0) {
            if (xQueueReceive(redParams.queue, &paziente, 0) == pdTRUE) {
                // Prova ad ottenere l'accesso a una sala operatoria
                if (xSemaphoreTake(xSemaphoreSaleOperatorie, portMAX_DELAY) == pdTRUE) {
                    printf("Inizio operazione su paziente rosso arrivato al secondo %d\n", paziente);
                    // Simula l'operazione...
                    vTaskDelay(RED_OPERATION_TICK);
                    printf("Fine operazione su paziente rosso arrivato al secondo %d\n", paziente);
                    xSemaphoreGive(xSemaphoreSaleOperatorie);
                }
            }
        }

        // Se non ci sono pazienti rossi, cerca nella coda verde
        else if (uxQueueMessagesWaiting(greenParams.queue) > 0) {
            if (xQueueReceive(greenParams.queue, &paziente, 0) == pdTRUE) {
                // Prova ad ottenere l'accesso a una sala operatoria
                if (xSemaphoreTake(xSemaphoreSaleOperatorie, portMAX_DELAY) == pdTRUE) {
                    printf("Inizio operazione su paziente verde arrivato al secondo %d\n", paziente);
                    // Simula l'operazione...
                    vTaskDelay(GREEN_OPERATION_TICK);
                    printf("Fine operazione su paziente verde arrivato al secondo %d\n", paziente);
                    xSemaphoreGive(xSemaphoreSaleOperatorie);
                }
            }
        }
    }
}

*/


int demo3( void ) {
    // Creazione delle code
    QueueHandle_t redQueue = xQueueCreate(PATIENT, sizeof(int));
    QueueHandle_t greenQueue = xQueueCreate(PATIENT, sizeof(int));

    if (redQueue == NULL || greenQueue == NULL) {
        printf("Errore nella creazione delle code\n");
        return 1;
    }

    // Assegnazione valore ai parametri
    redParams.queue = redQueue;
    greenParams.queue = greenQueue;

    // Copia i valori degli array
    memcpy(redParams.dataArray, redPatients, sizeof(redPatients));
    memcpy(greenParams.dataArray, greenPatients, sizeof(greenPatients));

    xSemaphoreSaleOperatorie = xSemaphoreCreateCounting(MAX_SALE, MAX_SALE);

    for (int i = 0; i < MAX_SALE; i++) {
        xTimers[i] = xTimerCreate("OperationTimer", pdMS_TO_TICKS(RED_OPERATION_TICK), pdFALSE, (void *)(intptr_t)i, operationCompleteCallback);
    }


    xTaskCreate( fillQueue, "RedFill", configMINIMAL_STACK_SIZE, &redParams, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( fillQueue, "GreenFill", configMINIMAL_STACK_SIZE, &greenParams, tskIDLE_PRIORITY + 1, NULL );

    xTaskCreate(salaOperatoriaTask, "SalaOper", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);


    vTaskStartScheduler();

    for( ;; );
}
