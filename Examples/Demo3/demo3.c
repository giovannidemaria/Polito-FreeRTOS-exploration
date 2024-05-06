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
#define ROW 4
#define COL 4

typedef struct s_mat{
    int vA[ROW];            //Row of matrix A
    int vB[COL];            //Column of matrix B
    int result;
    int x;
    int i;
    int j;
} t_mat;

int A[ROW][COL];
int B[COL][ROW];
int C[ROW][ROW];

void vTaskProduct(void *data) {
    t_mat *p = (t_mat *)data;

    for(int i = 0; i < p->x; i++) {
        p->result += p->vA[i] * p->vB[i];
        printf("%d %d\n", p->vA[i], p->vB[i]);
    }
    C[p->i][p->j] = p->result;
    vTaskDelete(NULL);
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
    vTaskDelete(NULL);
}

void copyOnVett(int col, int row, int *v, int option){
    if(option == 0) {
        for(int i = 0; i < col; i++){
            v[i] = A[row][i];
        }
    }
    else{
        for(int i = 0; i < row; i++){
            v[i] = B[i][col];
        }
    }
}

void demo3() {
    t_mat data[ROW*ROW];

    //Populate matrix
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            A[i][j] = j + 1;
        }
    }

    for(int i = 0; i < COL; i++){
        for(int j = 0; j < ROW; j++){
            B[i][j] = j;
        }
    }

    for (int i = 0; i < ROW*ROW; i++)
        data[i].x = COL;
    int c = 0;

    for(int i = 0; i < ROW; i++){
        copyOnVett(data[c].x, i, data[c].vA, 0);
        for(int j = 0; j < COL; j++){
            copyOnVett(j, data[c].x, data[c].vB, 1);
            data[c].result = 0;
            data[c].i = i;
            data[c].j = j; 
            xTaskCreate(vTaskProduct, "Product", configMINIMAL_STACK_SIZE, (void *)&data[c], configMAX_PRIORITIES - 1, NULL);
            c++;
        }
    }

    xTaskCreate(vTaskPrint, "Print", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 2, NULL);
    
    /* Starting FreeRTOS scheduler */
    vTaskStartScheduler();

    for(;;);
}