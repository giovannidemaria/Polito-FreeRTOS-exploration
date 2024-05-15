#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include <stdlib.h>

#define PATIENT_NUMBER 4

typedef struct {
    int patientCode;            // Unique identifier for the patient
    int arrivalTime;     // Time when the patient arrived
    int operationDuration; // Expected duration of the operation 
    int criticalTime;    // Time limit after which the patient's condition worsens
    int priority;   // Could be assigned
} PatientInfo_t;

PatientInfo_t patients[PATIENT_NUMBER] = {
    {1, 10, 15, 45, 0},
    {2, 10, 20, 40, 0}, 
    {3, 30, 20, 60, 0},  
    {4, 40, 5, 50, 0}
};
int ids[] = {0, 1, 2};

void taskPaziente(void *pvParameter) {
    PatientInfo_t *patient = (PatientInfo_t *)pvParameter;
    TickType_t xStart = xTaskGetTickCount();
    TickType_t xDelay = patient->operationDuration * configTICK_RATE_HZ;

    if(patient->criticalTime * configTICK_RATE_HZ <= xStart) {
        printf("    Paziente: %d e' deceduto a %d\n",patient->patientCode,xStart/configTICK_RATE_HZ);
        vTaskDelete(NULL);
    }

    printf("    Paziente: %d Inizio operazione:%d\n",patient->patientCode,xStart/configTICK_RATE_HZ);
    // Ciclo di attesa attiva
    while((xTaskGetTickCount() - xStart) < xDelay) {
        // Questo ciclo non fa nulla se non aspettare che passino secondi
    }

    printf("    Paziente: %d Fine operazione:%d\n",patient->patientCode,xTaskGetTickCount()/configTICK_RATE_HZ);    
    // Una volta completato, invia una notifica allo scheduler
    vTaskDelete(NULL);
}

void taskArrival(void *pvParameter) {
    (void) pvParameter;
    int startIndex = 0;
    TickType_t xPeriod = 1 * configTICK_RATE_HZ;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    TaskHandle_t xTaskHandle;
    int index;

    while(startIndex < PATIENT_NUMBER) {

        for(index = startIndex; index < PATIENT_NUMBER; index++) {
            if(patients[index].arrivalTime * configTICK_RATE_HZ > xLastWakeTime) {
                break;
            }
            printf("Patient %d arrived at time %d\n", patients[index].patientCode, xLastWakeTime/configTICK_RATE_HZ);
            #if ( configUSE_POLLING_SERVER == 1 )
            xTaskCreateAperiodic( taskPaziente,
                                "Patient Task",
                                (( unsigned short) 100),
                                (void *) &patients[index],
                                configMAX_PRIORITIES - patients[index].priority - 1,
                                patients[index].operationDuration,
                                patients[index].criticalTime * configTICK_RATE_HZ,
                                &xTaskHandle);
            #else // ( configUSE_POLLING_SERVER == 1)
                xTaskCreate(taskPaziente,
                        "Patient Task",
                        (( unsigned short ) 100 ),
                        &patients[index],
                        configMAX_PRIORITIES - patients[index].priority - 2,
                        &xTaskHandle);
            #endif // ( configUSE_POLLING_SERVER == 1)
        }
        if(index != startIndex) startIndex = index;

        xTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
    vTaskDelete(NULL);
}

int main_scheduler(){

    TaskHandle_t xTaskPeriodicHandle;
    #if ( configUSE_POLLING_SERVER == 1 )
        xTaskCreatePeriodic(taskArrival, "Task Arrival", (( unsigned short ) 100 ), NULL, configMAX_PRIORITIES - 2, 1*configTICK_RATE_HZ, &xTaskPeriodicHandle);
    #else // ( configUSE_POLLING_SERVER == 1 )
        xTaskCreate(taskArrival, "Task Arrival", (( unsigned short ) 100 ), NULL, configMAX_PRIORITIES - 2, &xTaskPeriodicHandle);
    #endif // ( configUSE_POLLING_SERVER == 1 )

    vTaskStartScheduler();
    for(;;);
}
