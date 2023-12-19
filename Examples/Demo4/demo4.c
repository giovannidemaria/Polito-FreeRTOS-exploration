#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>

#define MATRIX_SIZE 8

int matrixA[MATRIX_SIZE][MATRIX_SIZE] = {{1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1}};

int matrixB[MATRIX_SIZE][MATRIX_SIZE] = {{1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},
                                         {1, 1, 1, 1, 1, 1, 1, 1},};
int result[MATRIX_SIZE][MATRIX_SIZE];

typedef struct {
    int row;
    int col;
} TaskParams;

void matrixMul(void *parameters) {
    TaskParams *params = (TaskParams *)parameters;

    int row = params->row;
    int col = params->col;

    result[row][col] = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        result[row][col] += matrixA[row][i] * matrixB[i][col];
    }

    vTaskDelete(NULL);
}

void printRes(void *parameters) {
    vTaskDelay(pdMS_TO_TICKS(100));

    printf("Result Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    vTaskDelete(NULL);
}

void demo4() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            TaskParams *params = (TaskParams *)pvPortMalloc(sizeof(TaskParams));
            params->row = i;
            params->col = j;

            xTaskCreate(matrixMul, "MatrixMultiplyTask", configMINIMAL_STACK_SIZE, params, 2, NULL);
        }
    }

    xTaskCreate(printRes, "printRes", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();
    
    for(;;);
}