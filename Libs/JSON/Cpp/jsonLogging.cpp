#include <jsonLogging.hpp>

#ifndef _JSON_LOGGING_CPP_
#define _JSON_LOGGING_CPP_

void JSONLogging::logFunction(void) {
    finalJSON = JSON();
    finalJSON["LocalTime"] = asctime(localtime(&now));
    finalJSON["UTCTime"] = asctime(gmtime(&now));
    finalJSON["AttitudeADCV"] = {
        {"CAMZ", JSON::array()},
        {"CAMY", JSON::array()},
        {"CAMX", JSON::array()}
    };

    JSON tmp, aux;
    while(!isQueueEmpty() || !shouldExitLogging) {
        if (isQueueEmpty()) continue;
        semWaitForZeroAndGoUp();
        tmp = jsonQueue.front();
        jsonQueue.pop();
        semGoDown();
        
        aux = JSON();
        aux["x"] = tmp["x"];
        aux["y"] = tmp["y"];
        aux["z"] = tmp["z"];
        aux["absTime"] = tmp["absTime"];

        switch (tmp["cameraId"].get<int>()) {
        case CAMZ_ID:
            finalJSON["AttitudeADCV"]["CAMZ"].push_back(aux);
            break;

        case CAMY_ID:
            finalJSON["AttitudeADCV"]["CAMY"].push_back(aux);
            break;

        case CAMX_ID:
            finalJSON["AttitudeADCV"]["CAMX"].push_back(aux);
            break;
        }
    }

    if (authorName.empty()) {
        authorName = string("LODESTAR STAFF");
    }

    if (testLocal.empty()) {
        testLocal = string("Brasília, UnB, SG11 - LODESTAR");
    }

    if (projectName.empty()) {
        projectName = string("Adcv Testbed integration");
    }

    finalJSON["Author"] = authorName;
    finalJSON["Local"] = testLocal;
    finalJSON["Projeto"] = projectName;

    if (filePath.empty()) {
        /**
         * Set file name in format:
         * "LogFile_dd_hh_mm.json"
         * 
         * or
         * 
         * "LogFile_dd_mm_yyyy_hh_mm.json"
        */
        tm *ltm = localtime(&now);
        stringstream filePathString;
        filePathString << "LogFile_" << ltm->tm_mday << "_" << ltm->tm_hour << "_" << ltm->tm_min << ".json";
        filePath = filePathString.str();    
    }

    ofstream logFile;
    logFile.open(filePath, std::ios::out | std::ios::trunc);
    logFile << setw(4) << finalJSON;
    logFile.close();
}

void JSONLogging::setupSem(void) {
    union semun semValue;
    semValue.val = 0;

    semId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (semId < 0) {
        cerr << "JSONLogging::setupSem: " << strerror(errno) << endl;
        exit(ADCV_SEMGET_FAIL);
    }

    if (semctl(semId, 0, SETVAL, semValue) < 0) {
        cerr << "JSONLogging::setupSem: " << strerror(errno) << endl;
        exit(ADCV_SEMCTL_FAIL);
    }
}

void JSONLogging::tearDownSem(void) {
    if ((semId > 0) && (semctl(semId, 0, IPC_RMID) < 0)) {
        cerr << "JSONLogging::tearDownSem: " << strerror(errno) << endl;
        exit(ADCV_SEMCTL_FAIL);
    }
}

void JSONLogging::semWaitForZeroAndGoUp(void) {
    struct sembuf localSemOp[2];
    localSemOp[0].sem_num = 0;
    localSemOp[0].sem_op  = 0;
    localSemOp[0].sem_flg = SEM_UNDO;
    localSemOp[1].sem_num = 0;
    localSemOp[1].sem_op  = 1;
    localSemOp[1].sem_flg = SEM_UNDO;
    if (semop(semId, localSemOp, 2) < 0) {
        cerr << "JSONLogging::semWaitForZeroAndGoUp: " << strerror(errno) << endl;
        exit(ADCV_SEMOP_FAIL);
    }
}

void JSONLogging::semGoDown(void) {
    struct sembuf localSemOp;
    localSemOp.sem_num = 0;
    localSemOp.sem_op  = -1;
    localSemOp.sem_flg = SEM_UNDO;
    if (semop(semId, &localSemOp, 1) < 0) {
        cerr << "JSONLogging::semGoDown: " << strerror(errno) << endl;
        exit(ADCV_SEMOP_FAIL);
    }
}

bool JSONLogging::isQueueEmpty(void) {
    bool isEmpty;
    semWaitForZeroAndGoUp();
    isEmpty = jsonQueue.empty();
    semGoDown();
    return isEmpty;
}

// char* JSONLogging::getLocalTime(time_t now){
//     char* dt = ctime(&now);
//     return dt;
// }

// char* JSONLogging::getUTCTime(time_t now){
//     tm *gmtm = gmtime(&now);
//     char* utc = asctime(gmtm);
//     return utc;
// }


JSONLogging::JSONLogging() {
    now = time(NULL);
    setupSem();
    jsonQueue = queue<JSON>();
    shouldExitLogging = false;
    logThread = thread(&JSONLogging::logFunction, this);
}

JSONLogging::~JSONLogging() {
    shouldExitLogging = true;
    logThread.join();
    tearDownSem();
}

JSONLogging& JSONLogging::setFilePath(string __filePath) {
    filePath = __filePath;
    return *this;
}

JSONLogging& JSONLogging::setAuthorName(string __authorName) {
    authorName = __authorName;
    return *this;
}

JSONLogging& JSONLogging::setTestLocal(string __testLocal) {
    testLocal = __testLocal;
    return *this;
}

JSONLogging& JSONLogging::setProjectName(string __projectName) {
    projectName = __projectName;
    return *this;
}

JSONLogging& JSONLogging::adcvLogFrame(JSON frameData) {
    /**
     * Both json and database friendly time format.
     * 64bit Unix time in milliseconds
    */
    frameData["absTime"] = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    semWaitForZeroAndGoUp();
    jsonQueue.push(frameData);
    semGoDown();
    return *this;
}

#endif /* _JSON_LOGGING_CPP_ */
