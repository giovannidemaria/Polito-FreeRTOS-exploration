/* In this first demo, we use a timer to create a delay between output messages.
First, the delay is 1 second, and after each print, it is increased by 1.
In order to make it easier, we have defined a maximum number of reperition, 
but the same code, could be written for an undefined number of repetition of print.
*/

/* Standard includes. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#define DIM 4

typedef struct s_mat{
    int vA[DIM];            //Row of matrix A
    int vB[DIM];            //Column of matrix B
    int result;
    int x;              //Dimension
} t_mat;

int A[DIM][DIM];
int B[DIM][DIM];
int C[DIM][DIM];

void vTaskProduct(void *p) {
    t_mat *pn = (t_mat *)p;

    for(int i = 0; i < pn->x; i++)
        pn->result += pn->vA[i] * pn->vB[i];
}

void vTaskPrint() {
    int i, j;

    printf("\n");
    for(i=0; i < DIM; i++){
        for(j=0; j < DIM; j++){
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
}

void copyOnVett(int M[DIM][DIM], int dim, int *v, int option){
    if(option == 0){
        for(int i = 0; i < DIM; i++){
            v[i] = M[dim][i];
        }
    }
    else{
        for(int i = 0; i < DIM; i++){
            v[i] = M[i][dim];
        }
    }
}

void vTaskWrapper(void *p) {
    t_mat *data = (t_mat *)p; 
    for(int i = 0; i < DIM; i++){
        copyOnVett(A, i, data->vA, 0);
        for(int j = 0; j < DIM; j++){
            data->result = 0;
            copyOnVett(B, j, data->vB, 1);
            xTaskCreate(vTaskProduct, "Product", configMINIMAL_STACK_SIZE, (void *)&data, tskIDLE_PRIORITY + 1, NULL);
            C[i][j] = data->result;
        }
    }
    xTaskCreate(vTaskPrint, "Print", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL); 
}

void demo3() {
    t_mat data;

    //Populate matrix
    for(int i = 0; i < DIM; i++){
        for(int j = 0; j < DIM; j++){
            A[i][j] = j+1;
        }
    }

    for(int i = 0; i < DIM; i++){
        for(int j = 0; j < DIM; j++){
            B[i][j] = j;
        }
    }

    xTaskCreate(vTaskWrapper, "Wrapper", configMINIMAL_STACK_SIZE, (void *)&data, tskIDLE_PRIORITY, NULL);

    /* Starting FreeRTOS scheduler */
    vTaskStartScheduler();

    for(;;);
}