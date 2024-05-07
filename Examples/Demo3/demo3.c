#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define ROW 10
#define COL 10

//define matrix dimensions
#define A_ROWS ROW
#define A_COLS COL
#define B_ROWS ROW
#define B_COLS ROW

int A[A_ROWS][A_COLS];
int B[B_ROWS][B_COLS];
int C[A_ROWS][B_COLS];

typedef struct {
    int row;
    int col;
} t_mat;

//task to perform multiplication of a row from matrix A and a column from matrix B
void vTaskProduct(void *data) {
    t_mat *params = (t_mat *)data;
    int row = params->row;
    int col = params->col;
   
    int sum = 0;
    for (int i = 0; i < A_COLS; i++) {
        sum += A[row][i] * B[i][col];
    }
    C[row][col] = sum;

    vTaskDelete(NULL);
}

//task to print the result matrix
void vTaskPrint() {
    printf("Result Matrix:\n");
    for (int i = 0; i < A_ROWS; i++) {
        for (int j = 0; j < B_COLS; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    vTaskDelete(NULL);
}

void demo3() {
    //populate matrix A
    for (int i = 0; i < A_ROWS; i++) {
        for (int j = 0; j < A_COLS; j++) {
            A[i][j] = j + 1;
        }
    }

    //populate matrix B
    for (int i = 0; i < B_ROWS; i++) {
        for (int j = 0; j < B_COLS; j++) {
            B[i][j] = j;
        }
    }

    //create tasks for each row of matrix A
    for (int i = 0; i < A_ROWS; i++) {
        for (int j = 0; j < B_COLS; j++) {
            t_mat *data = (t_mat *)pvPortMalloc(sizeof(t_mat));
            data->row = i;
            data->col = j;
            xTaskCreate(vTaskProduct, "product", configMINIMAL_STACK_SIZE, (void *)data, tskIDLE_PRIORITY + 1, NULL);
        }
    }

    //create task to print the result matrix
    xTaskCreate(vTaskPrint, "print", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    //start scheduler
    vTaskStartScheduler();

    for(;;);
}