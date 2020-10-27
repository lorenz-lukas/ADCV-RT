// g++ --std=c++11 main.cpp tcp.cpp adcv.cpp -o exec -lopencv_highgui -lopencv_videoio -lopencv_core -lopencv_calib3d -lopencv_imgproc -lopencv_imgcodecs -lopencv_aruco
// ./exec -mode=0 -camZ

#include <adcv.hpp>
#include <jsonLogging.hpp>
#include <server.hpp>
#include <iostream>

#include <chrono>
#include <unistd.h>

// #include <stdio.h>
// #include <stdlib.h>

#include <FreeRTOS.h>		/* RTOS firmware */
#include <task.h>			/* Task */
#include <timers.h>

using namespace std;

//#include "queue.h"
/* Examples */
#define CH3_TASKMANAGEMENT
#define ESC_KEY 27 

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

typedef struct ARGS{
    bool z               = false;
    bool all             = false;
    bool rec             = false;
    bool video           = false;
    bool paralellization = false;
    string z_video       = "";
    string y_video       = "";
    string x_video       = "";
}ARGS;

char c = '\0'; // Key to quit
auto start_global = chrono::high_resolution_clock::now();
ADCV *adcv;
TCPServer* srv = NULL;
Connection* con = NULL;
bool frame_ready = false, ret = true;


void vApplicationIdleHook(void);
TaskHandle_t xHandle = NULL;

void initTCPServerConnection(TCPServer **srv, unsigned short port) {
    try{
      (*srv) = new TCPServer(port);
    }
    catch(runtime_error *e) {
        cerr << e->what() << endl;
        exit(1);
    }
}

Connection* getConnection(TCPServer **srv){
    Connection* con = NULL;
    try {
        con = (*srv)->accept_wait();
    }
    catch(runtime_error *e) {
        cerr << e->what() << endl;
    }
    if (*srv != NULL) {
       delete *srv;
       *srv = NULL;
    }
    return con;
}


int main ( void )
{
#ifdef CH3_TASKMANAGEMENT
    string x_video = "", y_video = "", z_video = "TEST_data/18_12_35_Z_RAW.avi"; //x_video = "TEST_data/18_12_35_X_RAW.avi", y_video = "TEST_data/18_12_35_Y_RAW.avi"
    // bool rec = false;
    bool rec = true;
    // ADCV *adcv;
    adcv = new ADCV(x_video, y_video, z_video, rec);
    
    cout << "Waiting for TCP ADCV-RPI TestBed Connection!!" << endl << endl;
    
    // initTCPServerConnection(&srv, SERVER_PORT);
    // con = getConnection(&srv);
	
    /* Creating Two Task Same Priorities and DelayUntil*/
	xTaskCreate( vTask1, "Task 1", 1000, NULL, 10, &xHandle);
	xTaskCreate( vTask2, "Task 2", 1000, NULL, 9, NULL );
    xTaskCreate( vTask3, "Task 3", 1000, NULL, 8, NULL );
	xTaskCreate( vTask4, "Task 4", 1000, NULL, 7, NULL );
    xTaskCreate( vTask5, "Task 5", 1000, NULL, 6, NULL );
	xTaskCreate( vTask6, "Task 6", 1000, NULL, 1, NULL );
    xTaskCreate( vTask7, "Task 7", 1000, NULL, 1, NULL );
	// xTaskCreate( vTask8, "Task 8", 1000, NULL, 1, NULL );
    xTaskCreate( vTask9, "Task 9", 1000, NULL, 1, NULL );
#endif
    start_global = chrono::high_resolution_clock::now();
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

void vTask1(void* parameter)
{   
	TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();

    while(c != ESC_KEY){
        start = chrono::high_resolution_clock::now();
        frame_ready = false;
        cout << "T1" << endl;
        ret = adcv->getFrame(CAMZ_ID);
        frame_ready  = true;
        end = chrono::high_resolution_clock::now();
        // cout << chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-6 << endl;
        cout << "i = " << i << " ;Global1: " << chrono::duration_cast<chrono::nanoseconds>(end - start_global).count() * 1e-6 << endl;
        i++;
        vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
        if(i == 100) break;
    }
    //usleep(16000);
    cout << "Exitig!" << endl;
    c = 27;
    ret = false;
    vTaskDelete(xHandle);
}
void vTask2(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    while(1){
        start = chrono::high_resolution_clock::now();
        usleep(3000);
        cout << "T2" << endl;
        adcv->getAruco(CAMZ_ID);
        end = chrono::high_resolution_clock::now();
        // cout << chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-6 << endl;
        cout << "Global2: " << chrono::duration_cast<chrono::nanoseconds>(end - start_global).count() * 1e-6 << endl;
        vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
        if(c == ESC_KEY) break;
        // i++;
    }
}

void vTask3(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    while(1){
        start = chrono::high_resolution_clock::now();
        cout << "T3" << endl;
        usleep(1000);
        adcv->estimatePose(CAMZ_ID);
        end = chrono::high_resolution_clock::now();
        cout << "Global3: " << chrono::duration_cast<chrono::nanoseconds>(end - start_global).count() * 1e-6 << endl;
        // i++;
        if(c == ESC_KEY) break;
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask4(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    while(1){
        start = chrono::high_resolution_clock::now();
        cout << "T4" << endl;
        usleep(1000);
        adcv->computeRodriguesVec(adcv->markerIdsZ.size(), adcv->rvecZ, adcv->camZ_id);
        end = chrono::high_resolution_clock::now();
        cout << "Global4: " << chrono::duration_cast<chrono::nanoseconds>(end - start_global).count() * 1e-6 << endl;
        // i++;       
		if(c == ESC_KEY) break;        
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask5(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    while(1){
        start = chrono::high_resolution_clock::now();
        cout << "T5" << endl;
        usleep(1000);
        adcv->computeEulersVec(adcv->markerIdsZ.size(), adcv->rvecZ, adcv->camZ_id);
        end = chrono::high_resolution_clock::now();
        cout << "Global5: " << chrono::duration_cast<chrono::nanoseconds>(end - start_global).count() * 1e-6 << endl;
        // i++;       
		if(c == ESC_KEY) break;        
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask6(void* parameter)
{
    JSON toLog;
	TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    while(1){
        // toLog = JSON();
        start = chrono::high_resolution_clock::now();
        cout << "T6" << endl;
        // toLog["cameraId"] = CAMZ_ID;
        // toLog["z"] = 1.1f;
        // toLog["y"] = 2.2f;c != ESC_KEY
        // log->adcvLogFrame(toLog);
        cout << adcv->attitude[6][0] <<  ";" << adcv->attitude[6][1] << ";" << adcv->attitude[6][2] << endl;
        end = chrono::high_resolution_clock::now();
        cout << "Global6: " << chrono::duration_cast<chrono::nanoseconds>(end - start_global).count() * 1e-6 << endl;
        // i++;       
		if(c == ESC_KEY) break;        
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}
void vTask7(void* parameter)
{   
    TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    while(1){
        start = chrono::high_resolution_clock::now();
        cout << "T7" << endl;
        adcv->recVideo();
        end = chrono::high_resolution_clock::now();
        cout << "Global7: " << chrono::duration_cast<chrono::nanoseconds>(end - start_global).count() * 1e-6 << endl;
        // i++;       
		if(c == ESC_KEY) break;        
		vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}

void vTask8(void* parameter)
{
    AdcvTcpData adcvTcpData;
    memset(&adcvTcpData, 0, sizeof(AdcvTcpData));
	TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    while(1){
        // if (numCam == 3) {
        //     adcvTcpData.x = adcv->attitude[8][2];
        //     adcvTcpData.y = adcv->attitude[7][2];
        //     adcvTcpData.z = adcv->attitude[6][2];
        // }
        start = chrono::high_resolution_clock::now();
        cout << "T8" << endl;
        adcvTcpData.x = adcv->attitude[6][0];
        adcvTcpData.y = adcv->attitude[6][1];
        adcvTcpData.z = adcv->attitude[6][2];
        con->sendAdcvTcpData(&adcvTcpData);
		end = chrono::high_resolution_clock::now();
        cout << "Global8: " << chrono::duration_cast<chrono::nanoseconds>(end - start_global).count() * 1e-6 << endl;
        // i++;       
		if(c == ESC_KEY) break;        
        vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
    }
}


void vTask9(void* parameter)
{
	TickType_t xLastWaketime = xTaskGetTickCount();
    int i = 0;
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    while(1){
        start = chrono::high_resolution_clock::now(); 
        cout << "T9" << endl;               
        adcv->showImage();
        c = (char)cv::waitKey(3);
        end = chrono::high_resolution_clock::now();
        cout << "i = " << i <<" ; Global9: " << chrono::duration_cast<chrono::nanoseconds>(end - start_global).count() * 1e-6 << endl;  
        if(c == ESC_KEY) break;
        vTaskDelayUntil(&xLastWaketime, pdMS_TO_TICKS(33));
        i++;
    }
}
#endif
/* CH3_TASKMANAGEMENT ends */


void vApplicationIdleHook(void)
{
    //printf("Idle\r\n");
}
/*-----------------------------------------------------------*/
