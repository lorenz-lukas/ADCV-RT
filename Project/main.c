/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"		/* RTOS firmware */
#include "task.h"			/* Task */
#include "timers.h"
//#include "queue.h"
/* Examples */
#define CH3_TASKMANAGEMENT

/* --------------------------------------------- */
#ifdef CH3_TASKMANAGEMENT
void vTask1(void*);

void vTask2(void*);
void vTask3(void*);
void vTask4(void*);
void vTask5(void*);
void vTask6(void*);
void vTask7(void*);
void vTask8(void*);
void vTask9(void*);

#endif

void vApplicationIdleHook(void);
TaskHandle_t xHandle = NULL;

int main ( void )
{
#ifdef CH3_TASKMANAGEMENT
	/* Creating Two Task Same Priorities and Delay*/
//	xTaskCreate( vTask1, "Task 1", 1000, NULL, 1, NULL );
//	xTaskCreate( vTask2, "Task 2", 1000, NULL, 1, NULL );
	/* Creating Two Task Same Priorities and DelayUntil*/
	xTaskCreate( vTask1, "Task 1", 1000, NULL, 10, &xHandle);
	// xTaskCreate( vTask2, "Task 2", 1000, NULL, 9, NULL );
    // xTaskCreate( vTask3, "Task 3", 1000, NULL, 8, NULL );
	// xTaskCreate( vTask4, "Task 4", 1000, NULL, 7, NULL );
    // xTaskCreate( vTask5, "Task 5", 1000, NULL, 6, NULL );
	// xTaskCreate( vTask6, "Task 6", 1000, NULL, 1, NULL );
    // xTaskCreate( vTask7, "Task 7", 1000, NULL, 1, NULL );
	// xTaskCreate( vTask8, "Task 8", 1000, NULL, 1, NULL );
    // xTaskCreate( vTask9, "Task 9", 1000, NULL, 1, NULL );
#endif
	vTaskStartScheduler();
    printf("FINISH\n");
	return 0;
}

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
 	taskENTER_CRITICAL();
	{
        printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
        fflush(stdout);
	}
	taskEXIT_CRITICAL();
	exit(-1);
}


#ifdef CH3_TASKMANAGEMENT
// void vTask1(void* parameter)
// {
//     while(1){
//         printf("Task 1\n");
// 		vTaskDelay(pdMS_TO_TICKS(250));
//     }
// }
// void vTask2(void* parameter)
// {
//     while(1){
//         printf("Task 2\n");
//         vTaskDelay(pdMS_TO_TICKS(250));
//     }
// }

void vTask1(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    while(1){
        printf("Task 1\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
        i++;
        if(i == 10){    
            break;
        }
    }
    vTaskDelete(xHandle);
}
void vTask2(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 2\n");
        vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask3(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 3\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask4(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 4\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask5(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 5\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask6(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 6\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask7(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 7\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask8(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 8\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}


void vTask9(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    while(1){
        printf("Task 9\n");
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}
#endif
/* CH3_TASKMANAGEMENT ends */


void vApplicationIdleHook(void)
{
    //printf("Idle\r\n");
}
/*-----------------------------------------------------------*/
