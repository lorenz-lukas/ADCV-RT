// g++ --std=c++11 main.cpp tcp.cpp adcv.cpp -o exec -lopencv_highgui -lopencv_videoio -lopencv_core -lopencv_calib3d -lopencv_imgproc -lopencv_imgcodecs -lopencv_aruco
// ./exec -mode=0 -camZ
// #include <adcv.hpp>
// #include <jsonLogging.hpp>
// #include <iostream>
// #include <chrono>
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

// typedef struct ARGS{
//     bool z               = false;
//     bool all             = false;
//     bool rec             = false;
//     bool video           = false;
//     bool paralellization = false;
//     string z_video       = "";
//     string y_video       = "";
//     string x_video       = "";
// }ARGS;
char c = '\0'; // Key to quit

void vApplicationIdleHook(void);
TaskHandle_t xHandle = NULL;

// void initTCPServerConnection(TCPServer **srv, unsigned short port) {
//     try{
//       (*srv) = new TCPServer(port);
//     }
//     catch(runtime_error *e) {
//         cerr << e->what() << endl;
//         exit(1);
//     }
// }

// Connection* getConnection(TCPServer **srv){
//     Connection* con = NULL;
//     try {
//         con = (*srv)->accept_wait();
//     }
//     catch(runtime_error *e) {
//         cerr << e->what() << endl;
//     }
//     if (*srv != NULL) {
//        delete *srv;
//        *srv = NULL;
//     }
//     return con;
// }


int main ( void )
{
#ifdef CH3_TASKMANAGEMENT
    // adcv = new ADCV(args.x_video, args.y_video, args.z_video, args.rec);
    
    // Starting simulation:
    // TCPServer* srv = NULL;
    // Connection* con = NULL;

    // cout << "Waiting for TCP ADCV-RPI TestBed Connection!!" << endl << endl;
    // initTCPServerConnection(&srv, SERVER_PORT);
    // con = getConnection(&srv);

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
    // bool frameOk = true;
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();

    while(1){
        printf("Task1 \n");
        // start = chrono::high_resolution_clock::now();
        //frameOk = adcv->getFrame(cam);
        // end = chrono::high_resolution_clock::now();
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
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();
    while(1){
        printf("Task2 \n");
        // start = chrono::high_resolution_clock::now();
        // adcv->getAruco(cam);
        // end_aruco = chrono::duration_cast<chrono::nanoseconds>( chrono::high_resolution_clock::now() - start).count() * 1e-6;

        vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask3(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();
    while(1){
        printf("Task3 \n");
        // start = chrono::high_resolution_clock::now();
        // adcv->estimatePose(cam);
        // end_pose = chrono::duration_cast<chrono::nanoseconds>( chrono::high_resolution_clock::now() - start).count() * 1e-6;        
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask4(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();
    while(1){
        printf("Task4\n");
        // start = chrono::high_resolution_clock::now();
        // adcv->computeRodriguesVec(adcv->markerIdsZ.size(), adcv->rvecZ, adcv->camZ_id);
        // end_rodrigues = chrono::duration_cast<chrono::nanoseconds>( chrono::high_resolution_clock::now() - start).count() * 1e-6;
                
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask5(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();
    while(1){
        printf("Task5\n");
        // start = chrono::high_resolution_clock::now();
        // adcv->computeEulersVec(adcv->markerIdsZ.size(), adcv->rvecZ, adcv->camZ_id);
        // end_euler = chrono::duration_cast<chrono::nanoseconds>( chrono::high_resolution_clock::now() - start).count() * 1e-6;
                
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask6(void* parameter)
{
    // JSON toLog;
	TickType_t xLastWaketime = xTaskGetTickCount();
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();
    while(1){
        // toLog = JSON();
        printf("Task 6\n");
        // start = chrono::high_resolution_clock::now();
        // toLog["cameraId"] = cam;
        // toLog["z"] = 1.1f;
        // toLog["y"] = 2.2f;
        // toLog["x"] = 3.3f;
        // log->adcvLogFrame(toLog);
        // end_euler = chrono::duration_cast<chrono::nanoseconds>( chrono::high_resolution_clock::now() - start).count() * 1e-6;
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask7(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();
    while(1){
        printf("Task 7\n");
        // start = chrono::high_resolution_clock::now();
        // adcv->recVideo()
        // end_euler = chrono::duration_cast<chrono::nanoseconds>( chrono::high_resolution_clock::now() - start).count() * 1e-6;
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask8(void* parameter)
{
    // AdcvTcpData adcvTcpData;
    // memset(&adcvTcpData, 0, sizeof(AdcvTcpData));
	TickType_t xLastWaketime = xTaskGetTickCount();
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();
    while(1){
        printf("Task 8\n");
        // if (numCam == 3) {
        //     adcvTcpData.x = adcv->attitude[8][2];
        //     adcvTcpData.y = adcv->attitude[7][2];
        //     adcvTcpData.z = adcv->attitude[6][2];
        // }
        // else {
        //     adcvTcpData.x = adcv->attitude[6][0];
        //     adcvTcpData.y = adcv->attitude[6][1];
        //     adcvTcpData.z = adcv->attitude[6][2];
        // }
        // con->sendAdcvTcpData(&adcvTcpData);
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}


void vTask9(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();
    while(1){
        printf("Task 9\n");
        // start = chrono::high_resolution_clock::now();                
        // adcv->showImage();
        // end_show = chrono::duration_cast<chrono::nanoseconds>( chrono::high_resolution_clock::now() - start).count() * 1e-6;
    
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
