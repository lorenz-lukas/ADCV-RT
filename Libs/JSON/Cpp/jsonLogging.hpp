#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>
#include <chrono>
#include <queue>
#include <iomanip>
#include <sstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <json.hpp>
#include <exitCodes.hpp>
#include <semUnion.hpp>
#include <cameraIds.hpp>

#ifndef _JSON_LOGGING_HPP_
#define _JSON_LOGGING_HPP_

using std::thread;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::queue;
using JSON = nlohmann::json;
using std::setw;
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;
using std::ofstream;

typedef struct JSONLoggingCamera {
    double frameTimeADCV;
    double loopTimeADCV;
    double loopTimeTCP;
    time_t absTime;
    int camId;
} JSONLoggingCamera;

class JSONLogging {
    JSON finalJSON;
    thread logThread;
    string filePath;
    string authorName;
    string testLocal;
    string projectName;
    time_t now;
    bool shouldExitLogging;
    int semId = 0;
    queue<JSON> jsonQueue;

    void logFunction(void);
    void setupSem(void);
    void tearDownSem(void);
    void semWaitForZeroAndGoUp(void);
    void semGoDown(void);
    bool isQueueEmpty(void);
    // string time, timeUTC;
    // char* getLocalTime(time_t now);
    // char* getUTCTime(time_t now);
public:
    JSONLogging();
    ~JSONLogging();
    JSONLogging& setFilePath(string __filePath);
    JSONLogging& setAuthorName(string __authorName);
    JSONLogging& setTestLocal(string __testLocal);
    JSONLogging& setProjectName(string __projectName);

    /**
     * Recebe um JSON no formato
     * 
     * {
     *  "cameraId": CAMX_ID,
     *  "x": double,
     *  "y": double,
     *  "z": double,
     *  "absTime": time_t
     * }
    */
    JSONLogging& adcvLogFrame(JSON frameData);
};

#endif /* _JSON_LOGGING_HPP_ */
