#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include <stdlib.h>

#define PATIENT 9

TaskHandle_t xSchedulerTaskHandle;
/* Definizione strutture dati pazienti*/
typedef struct {
    int patientCode;            // Unique identifier for the patient
    int arrivalTime;     // Time when the patient arrived
    int operationDuration; // Expected duration of the operation 
    int criticalTime;    // Time limit after which the patient's condition worsens
    int priority;   // Could be assigned
} PatientInfo_t;

PatientInfo_t patients[] = {
    {1, 10, 15, 45, 0},
    {2, 10, 20, 30, 0}, 
    {3, 30, 20, 30, 0},  
    {4, 40, 5, 10, 0}
};

int comparePatients(const void *a, const void *b) {
    PatientInfo_t *patientA = (PatientInfo_t *)a;
    PatientInfo_t *patientB = (PatientInfo_t *)b;
    return patientA->priority - patientB->priority;
}

void taskPaziente(void *pvParameter) {
    PatientInfo_t *patient = (PatientInfo_t *)pvParameter;
    TickType_t xStart = xTaskGetTickCount();
    TickType_t xDelay = patient->operationDuration * configTICK_RATE_HZ;

    printf(" Paziente: %d Inizio operazione:%d\n",patient->patientCode,xStart/configTICK_RATE_HZ);
    // Ciclo di attesa attiva
    while((xTaskGetTickCount() - xStart) < xDelay) {
        // Questo ciclo non fa nulla se non aspettare che passino secondi
    }

    printf(" Paziente: %d Fine operazione:%d \n",patient->patientCode,xTaskGetTickCount()/configTICK_RATE_HZ);    
    // Una volta completato, invia una notifica allo scheduler
    xTaskNotifyGive(xSchedulerTaskHandle);
    vTaskDelete(NULL);
}

void eliminaPazientePrimaDellOperazione(PatientInfo_t *patient,int patientCode, int patientNum){
    for(int i = 0; i < patientNum; i++){
        if(patient[i].patientCode == patientCode){
            for(int j = i; j< patientNum -1 ; j++)
                patient[j]=patient[j+1];
            break;
        }
    }
}

void eliminaPaziente(int patientCode, int patientNum){
    for(int i = 0; i < patientNum; i++){
        if(patients[i].patientCode == patientCode){
            for(int j = i; j< patientNum -1 ; j++)
                patients[j]=patients[j+1];
            break;
        }
    }
}

// Questo task simula il centralino ospedaliero
void taskScheduler(void *pvParameter){
    int i;
    int j=0;
    int patientNum = 4;
    PatientInfo_t patientArrived[patientNum];

    // Tempo di inizio
    TickType_t xNow;

    while(1){

        // Finchè il tempo attuale è maggiore o uguale al tempo in cui il paziente è arrivato 
            for(i = 0; i < patientNum; i++){
                xNow = xTaskGetTickCount();
                        // printf("Il primo paziente arriverà a %d ora sono le %d \n", patients[i].arrivalTime,xNow/configTICK_RATE_HZ );
                // leggo il paziente e lo inserisco in un vettore
                if((patients[i].arrivalTime * configTICK_RATE_HZ ) > xNow)
                    break;
                else{
                    patientArrived[j] = patients[i];
                    j++;
                }
            }

        // Se almeno un paziente è arrivato
        if(j > 0){
            printf("Al tempo %d sono in attesa %d pazienti:\n",xNow/configTICK_RATE_HZ, j);

            // Calcolo la prorità tra i pazienti già arrivati
            for (int k = 0; k < j; k++) {
                patientArrived[k].priority = patientArrived[k].criticalTime - patientArrived[k].operationDuration + patientArrived[k].arrivalTime - xNow/configTICK_RATE_HZ;
                if(patientArrived[k].priority >= 0)
                    printf(" -  Il paziente %d muore se non si opera entro %d \n",patientArrived[k].patientCode,patientArrived[k].priority);
                else{
                    printf(" ALLERT: Il paziente %d è deceduto\n",patientArrived[k].patientCode);
                    eliminaPaziente(patientArrived[k].patientCode, patientNum);
                    eliminaPazientePrimaDellOperazione(patientArrived, patientArrived[k].patientCode, patientNum);
                    patientNum--;
                }
            }

            // Ordino il vettore appena creato per priorità
            qsort(patientArrived, j, sizeof(PatientInfo_t), comparePatients);

            // Estraggo dal vettore il paziente a priorità maggiore 
            printf("Il paziente %d inizia a essere operato\n",patientArrived[0].patientCode);

            // Avvio il suo task paziente 
            xTaskCreate(taskPaziente, "Paziente", configMINIMAL_STACK_SIZE, (void *)&patientArrived[0], 3, NULL);    
            
            // attendo che il task finisca
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

            // Elimino dal vettore pazienti il paziente
            eliminaPaziente(patientArrived[0].patientCode, patientNum);

            // Aggiorno dimenzione vettore pazienti ancora da operare
            patientNum--;

            // Azzero conteggio tra i pazienti arrivati e da schedulare
            j=0;
        }
    }
}

int main_scheduler(){

 // Crea il task scheduler con alta priorità
    xTaskCreate(taskScheduler, "Scheduler", ( ( unsigned short ) 1000 ), NULL, configMAX_PRIORITIES - 1, &xSchedulerTaskHandle);
    vTaskStartScheduler();
    for(;;);
}

