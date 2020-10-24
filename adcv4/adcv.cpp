#include <adcv.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <ctime>
#include <signal.h>

#ifndef _ADCV_CPP_
#define _ADCV_CPP_

#define DETECTOR_PARAM_FILE       "adcv4/data/files/detector_params.yml"
#define CAM_DISTORTION            "adcv4/data/files/embended_distortion_matrix.xml"
#define CAM_INTRINSIC             "adcv4/data/files/embended_intrinsic_parameters.xml"
#define DEFAULT_DICTIONARY        DICT_4X4_50

#define DEFAULT_Z_VIDEO           "adcv4/data/videos/Zadcv2.avi"
#define DEFAULT_X_VIDEO           "adcv4/data/videos/Xadcv2.avi"
#define DEFAULT_Y_VIDEO           "adcv4/data/videos/Yadcv2.avi"

#define MAX_MODE                  0x003f
#define FRAME_WIDTH               640
#define FRAME_HEIGHT              480
#define MARKER_LENGTH             0.03f

#define OFFSET_ROTATION_RAW  3
#define OFFSET_RODRIGUES     6

using namespace std;
using namespace cv;
using namespace aruco; // aruco 180 degrees == 0 graus of reference system

ADCV::ADCV(bool All, bool rec){
    this->aruco_dictionary = getPredefinedDictionary(DEFAULT_DICTIONARY);
    this->aruco_parameters = DetectorParameters::create();
    bool init = true;
    this->rec = rec;

    this->camZ_on = true;
    if(All){
      this->camX_on = true;
      this->camY_on = true;
    }
    
    time_t now = showDate();

    init = this->initCamera();
    if(init == false){
      cout << "[ERROR] Unable to open camera(s)!" << endl;
      exit(1);
    }

    if(rec)  this->initRecVideo(now);

    init = this->setup();
    if(init == false){ // kill code if init is not right
      cout << "[ERROR] Unable to open Aruco and Camera files. Please check the path." << endl;
      exit(1);
    }
}

ADCV::ADCV(std::string X_file_name, std::string Y_file_name, std::string Z_file_name, bool rec){
    this->aruco_dictionary = getPredefinedDictionary(DEFAULT_DICTIONARY);
    this->aruco_parameters = DetectorParameters::create();
    this-> video = true;

    if(X_file_name != "") {
      this->camX_name = X_file_name;
      this->camX_on = true;
    }
    if(Y_file_name != ""){
      this->camY_name = Y_file_name;
      this->camY_on = true;
    }
    if(Z_file_name != ""){
      this->camZ_name = Z_file_name;
      this->camZ_on = true;

    }

    this->rec = rec;
    time_t now = showDate();

    bool init = this->initCamera();
    if(!init){
      cout << "[ERROR] Unable to open video file(s)!!" << endl << "\tCheck the given path." << endl;
      exit(1);
    }

    if(rec)  this->initRecVideo(now);

    init = this->setup();
    if(init == false){ // kill code if init is not right
      cout << "[ERROR] Unable to open Aruco and Camera files. Please check the path." << endl;
      exit(1);
    }
}

ADCV::~ADCV(){
    destroyAllWindows();
}

bool ADCV::initCamera(){
    bool init = true;
    if(this->camZ_on && !this->video) this->captureZ = openCamera(this->camZ_id);
    if(this->camY_on && !this->video) this->captureY = openCamera(this->camY_id);
    if(this->camX_on && !this->video) this->captureX = openCamera(this->camX_id);
    if(this->camZ_on && this->video) this->captureZ = openCamera(this->camZ_name);
    if(this->camY_on && this->video) this->captureY = openCamera(this->camY_name);
    if(this->camX_on && this->video) this->captureX = openCamera(this->camX_name);
    if((this->camZ_on && !this->captureZ.isOpened()) || (this->camY_on && !this->captureY.isOpened()) || (this->camX_on && !this->captureX.isOpened())) init = false;
    return init;
}

cv::VideoCapture ADCV::openCamera(int camID){
    cv::VideoCapture capture;
    bool ready = false;
    cout << endl << "Trying to connect Camera:" <<  camID << endl;
    ready = capture.open(camID);
    if (!ready)
        throw runtime_error("Could not open camera id: " + to_string(camID));
    capture.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    return capture;
}

cv::VideoCapture ADCV::openCamera(std::string camName){
    cv::VideoCapture capture;
    capture.open(camName);
    capture.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    cout << "Video path: " << camName << endl;
    return capture;
}

bool ADCV::getFrame(int cam){
    bool ret = true;
    if((cam == CAMZ_ID) && this->camZ_on) this->captureZ.read(this->rawZ);
    if((cam == CAMY_ID) && this->camY_on) this->captureY.read(this->rawY);
    if((cam == CAMX_ID) && this->camX_on) this->captureX.read(this->rawX);
    if((this->camZ_on && !this->captureZ.isOpened()) || (this->camY_on && !this->captureY.isOpened()) || (this->camX_on && !this->captureX.isOpened())) ret = false;
    if((this->camZ_on && this->rawZ.empty()) || (this->camY_on && this->rawY.empty()) || (this->camX_on && this->rawX.empty())) ret = false;
    return ret;
}

void ADCV::showImage(){
    if(this->camX_on && !this->rawX.empty()){
      namedWindow("X", WINDOW_AUTOSIZE);
      moveWindow( "X", 2*FRAME_WIDTH, 0);
      if((this->markerIdsX.size() == 1)){
          this->drawAruco(0);
          imshow("X", this->imgX);
      }else imshow("X", this->rawX);
    }

    if(this->camY_on && !this->rawY.empty()){
      namedWindow("Y", WINDOW_AUTOSIZE);
      moveWindow( "Y", FRAME_WIDTH, 0);
      if((this->markerIdsY.size() == 1)){
          this->drawAruco(1);
          imshow("Y", this->imgY);
      }else imshow("Y", this->rawY);
    }

    if(this->camZ_on && !this->rawZ.empty()){
      namedWindow("Z", WINDOW_AUTOSIZE);
      moveWindow( "Z", 0          , 0);
      if(this->markerIdsZ.size() == 1){
          this->drawAruco(2);
          imshow("Z", this->imgZ);
      }else imshow("Z", this->rawZ);
    }
}
void ADCV::drawAruco(int axis){
  if(axis == 0){ // X
    this->imgX = this->rawX.clone();
    drawDetectedMarkers(this->imgX, this->markerCornersX, this->markerIdsX);
  }
  if(axis == 1){ //Y
    this->imgY = this->rawY.clone();
    drawDetectedMarkers(this->imgY, this->markerCornersY, this->markerIdsY);
  }
  if(axis == 2){ // Z
    this->imgZ = this->rawZ.clone();
    drawDetectedMarkers(this->imgZ, this->markerCornersZ, this->markerIdsZ);
  }
  for(int i =0; (i < this->markerIdsZ.size()) || (i < this->markerIdsY.size()) || (i < this->markerIdsX.size()); i++ ){
     if(this->camX_on && (markerIdsX.size() == 1))
        drawAxis(this->imgX, this->intrinsics, this->distortion, this->attitude[0], this->tVec[0], this->markerLength * 0.5f);
     if(this->camY_on && (this->markerIdsY.size() == 1))
        drawAxis(this->imgY, this->intrinsics, this->distortion, this->attitude[1], this->tVec[1], this->markerLength * 0.5f);
     if(this->camZ_on && (this->markerIdsZ.size() == 1))
        drawAxis(this->imgZ, this->intrinsics, this->distortion, this->attitude[2], this->tVec[2], this->markerLength * 0.5f);
  }
}
time_t ADCV::showDate(){
  time_t now = time(0);
  cout << "\t\t###########################################"<< endl;
  cout << "\t\t###  ADCV 4 on OPENCV " << CV_VERSION << "\t\t###"<< endl;
  cout << "\t\t###  System developed by: Lukas Lorenz  ###"<< endl;
  cout << "\t\t###########################################"<< endl;
  cout << endl;
  char* dt = ctime(&now);
  cout << "\tThe local date and time is: " << dt << endl;
  tm *gmtm = gmtime(&now);
  char* utc = asctime(gmtm);
  cout << "\tThe UTC date and time is:   " << utc << endl;
  return now;
}

void ADCV::initRecVideo(time_t now){
    tm *ltm = localtime(&now);
    cout << "[WARNING] REC mode on!!" << endl << endl;
    this->recFile_name += (to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min));
    if(this->camX_on){
     this->outX.open(this->recFile_name + "_X" + ".avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(640, 480), true);
     this->outX_raw.open( this->recFile_name  + "_X_RAW" + ".avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(640, 480), true);
    }
    if(this->camY_on){
     this->outY.open(this->recFile_name + "_Y" + ".avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(640, 480), true);
     this->outY_raw.open( this->recFile_name + "_Y_RAW" + ".avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(640, 480), true);
    }
    if(this->camZ_on){
     this->outZ.open(this->recFile_name + "_Z" + ".avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(640, 480), true);
     this->outZ_raw.open(this->recFile_name + "_Z_RAW" + ".avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(640, 480), true);
   }
}

void ADCV::recVideo(){
  //this->dataLog.push_back(this->attitude);
  if(this->camX_on && !this->rawX.empty()){
    if(this->imgX.empty()) this->imgX = this->rawX.clone();
    this->outX.write(this->imgX);
    this->outX_raw.write(this->rawX);
  }
  if(this->camY_on && !this->rawY.empty()){
    if(this->imgY.empty()) this->imgY = this->rawY.clone();
    this->outY.write(this->imgY);
    this->outY_raw.write(this->rawY);
  }
  if(this->camZ_on){
    if(this->imgZ.empty()) this->imgZ = this->rawZ.clone();
    this->outZ.write(this->imgZ);
    this->outZ_raw.write(this->rawZ);
  }
}

bool ADCV::readCameraParameters()
{
   string filename = CAM_DISTORTION;
   FileStorage fs(filename, FileStorage::READ);
   if(!fs.isOpened()){
     fs.release();
     cout<< "[ERROR] Unable to open camera distortion file!" << endl;
     return false;
   }
  fs["distortion"] >> this->distortion;
  fs.release();

  filename = CAM_INTRINSIC;
  FileStorage f(filename, FileStorage::READ);
  if(!f.isOpened()){
    f.release();
    cout<< "[ERROR] Unable to open camera intrinsics file!" << endl;
    return false;
  }

  f["intrinsics"] >> this->intrinsics;
  f.release();
   return true;
}

bool ADCV::readDetectorParameters(Ptr<aruco::DetectorParameters> &params)
{
   string filename = DETECTOR_PARAM_FILE;
   FileStorage fs(DETECTOR_PARAM_FILE, FileStorage::READ);
   if(!fs.isOpened()){
     cout << "oi" << endl;
     fs.release();
     return false;
   }
   if(params == NULL) cout << "oi" << endl;
   fs["adaptiveThreshWinSizeMin"] >> params->adaptiveThreshWinSizeMin;
   fs["adaptiveThreshWinSizeMax"] >> params->adaptiveThreshWinSizeMax;
   fs["adaptiveThreshWinSizeStep"] >> params->adaptiveThreshWinSizeStep;
   fs["adaptiveThreshConstant"] >> params->adaptiveThreshConstant;
   fs["minMarkerPerimeterRate"] >> params->minMarkerPerimeterRate;
   fs["maxMarkerPerimeterRate"] >> params->maxMarkerPerimeterRate;
   fs["polygonalApproxAccuracyRate"] >> params->polygonalApproxAccuracyRate;
   fs["minCornerDistanceRate"] >> params->minCornerDistanceRate;
   fs["minDistanceToBorder"] >> params->minDistanceToBorder;
   fs["minMarkerDistanceRate"] >> params->minMarkerDistanceRate;
   fs["cornerRefinementMethod"] >> params->cornerRefinementMethod;
   fs["cornerRefinementWinSize"] >> params->cornerRefinementWinSize;
   fs["cornerRefinementMaxIterations"] >> params->cornerRefinementMaxIterations;
   fs["cornerRefinementMinAccuracy"] >> params->cornerRefinementMinAccuracy;
   fs["markerBorderBits"] >> params->markerBorderBits;
   fs["perspectiveRemovePixelPerCell"] >> params->perspectiveRemovePixelPerCell;
   fs["perspectiveRemoveIgnoredMarginPerCell"] >> params->perspectiveRemoveIgnoredMarginPerCell;
   fs["maxErroneousBitsInBorderRate"] >> params->maxErroneousBitsInBorderRate;
   fs["minOtsuStdDev"] >> params->minOtsuStdDev;
   fs["errorCorrectionRate"] >> params->errorCorrectionRate;
   fs.release();
   return true;
}

bool ADCV::setup()
{
   if(!readDetectorParameters(this->aruco_parameters)) {
      cerr << "[ERROR] Invalid detector parameters file" << endl;
      return false;
   }

   this->aruco_parameters->cornerRefinementMethod = CORNER_REFINE_SUBPIX; // do corner refinement in markers

   if(!readCameraParameters()) {
      cerr << "[ERROR] Invalid camera parameters file" << endl;
      return false;
   }

   return true;
}

void ADCV::determAttitude(int cam)
{ 
  getAruco(cam);
  estimatePose(cam);
  if(cam == CAMX_ID && this->markerIdsX.size() == 1) computeRodriguesVec(this->markerIdsX.size(), this->rvecX, this->camX_id); // axis
  if(cam == CAMY_ID && this->markerIdsY.size() == 1) computeRodriguesVec(this->markerIdsY.size(), this->rvecY, this->camY_id);
  if(cam == CAMZ_ID && this->markerIdsZ.size() == 1) computeRodriguesVec(this->markerIdsZ.size(), this->rvecZ, this->camZ_id);
  if(cam == CAMX_ID && this->markerIdsX.size() == 1) computeEulersVec(this->markerIdsX.size(), this->rvecX, this->camX_id);
  if(cam == CAMY_ID && this->markerIdsY.size() == 1) computeEulersVec(this->markerIdsY.size(), this->rvecY, this->camY_id);
  if(cam == CAMZ_ID && this->markerIdsZ.size() == 1) computeEulersVec(this->markerIdsZ.size(), this->rvecZ, this->camZ_id);
}
void ADCV::getAruco(int cam)
{
  if((cam == CAMX_ID) && this->camX_on && !this->rawX.empty())
    detectMarkers(this->rawX, this->aruco_dictionary, this->markerCornersX, this->markerIdsX, this->aruco_parameters);
  if((cam == CAMY_ID) && this->camY_on && !this->rawY.empty())
    detectMarkers(this->rawY, this->aruco_dictionary, this->markerCornersY, this->markerIdsY, this->aruco_parameters);
  if((cam == CAMZ_ID) && this->camZ_on && !this->rawZ.empty())
    detectMarkers(this->rawZ, this->aruco_dictionary, this->markerCornersZ, this->markerIdsZ, this->aruco_parameters);
}
void ADCV::estimatePose(int cam)
{
    if((cam == CAMX_ID) && this->camX_on && (this->markerIdsX.size() == 1))
    {
        estimatePoseSingleMarkers(this->markerCornersX, this->markerLength, this->intrinsics, this->distortion, this->rvecX, this->tVecX);
        this->attitude[cam] = rvecX[0];
    }
    if((cam == CAMY_ID) && this->camY_on && (this->markerIdsY.size() == 1)){
        estimatePoseSingleMarkers(this->markerCornersY, this->markerLength, this->intrinsics, this->distortion, this->rvecY, this->tVecY);
        this->attitude[cam] = rvecY[0];
    }
    if((cam == CAMZ_ID) && this->camZ_on && (this->markerIdsZ.size() == 1)){
      estimatePoseSingleMarkers(this->markerCornersZ, this->markerLength, this->intrinsics, this->distortion, this->rvecZ, this->tVecZ);
      this->attitude[cam] = rvecZ[0];
    }
}

void ADCV::computeRodriguesVec(int markerLen, vector<Vec3d> rvecs, int axis){
    Vec3d rvecOut(0, 0, 0);  
    rvecOut += rvecs[0];
    
    rvecOut = Vec3d(rvecOut[0] / markerLen, rvecOut[1] / markerLen, rvecOut[2] / markerLen);
    if(rvecOut[0]>360)rvecOut[0] = rvecOut[0]/1000; // Numeric imprecision correction
    if(rvecOut[1]>360)rvecOut[1] = rvecOut[1]/1000;
    if(rvecOut[2]>360)rvecOut[2] = rvecOut[2]/1000;
    this->attitude[ OFFSET_ROTATION_RAW + axis] = rvecOut;
}

bool ADCV::isRotationMatrix(Mat &R)
{
   Mat Rt;
   transpose(R, Rt);
   Mat shouldBeIdentity = Rt * R;
   Mat I = Mat::eye(3, 3, shouldBeIdentity.type());

   return  norm(I, shouldBeIdentity) < 1e-6;
}

Vec3d ADCV::rotationMatrixToEulerAngles(Mat &R)
{
   assert(isRotationMatrix(R));

   double sy = sqrt(R.at<double>(0, 0) * R.at<double>(0, 0) + R.at<double>(1, 0) * R.at<double>(1, 0));

   bool singular = sy < 1e-6; // If

   double x, y, z;
   if(!singular)
   {
      x = atan2(R.at<double>(2, 1), R.at<double>(2, 2));
      y = atan2(-R.at<double>(2, 0), sy);
      z = atan2(R.at<double>(1, 0), R.at<double>(0, 0));

   }
   else
   {
      x = atan2(-R.at<double>(1, 2), R.at<double>(1, 1));
      y = atan2(-R.at<double>(2, 0), sy);
      z = 0;
   }

   return Vec3d(x,y,z);
}

void ADCV::computeEulersVec(int markerLen, vector<Vec3d> rvecs, int axis){
    Mat A = Mat(Size(3, 3), CV_64F);

    A.at<double>(0, 0) = 0.9989584; A.at<double>(0, 1) = 0.0049715; A.at<double>(0, 2) = 0.0453595;
    A.at<double>(1, 0) = -0.0084494; A.at<double>(1, 1) = 0.9970102; A.at<double>(1, 2) = 0.0768069;
    A.at<double>(2, 0) = -0.0448420; A.at<double>(2, 1) = -0.0771102; A.at<double>(2, 2) = 0.9960137;

    Mat R1= Mat(Size(3, 3), CV_64F);
    Vec3d eulerAngs(0, 0, 0), eulerAngsTmp;
    vector<Vec3d> evecOut;
    for(int i = 0; i < markerLen; i++) {
        Rodrigues(rvecs[i], R1);
        Mat R2 = Mat(Size(3, 3), CV_64F);
        Mat R = Mat(Size(3, 3), CV_64F);
        for (int j = 0; j < 3; j++) {
              for (int k = 0; k < 3; k++) {
                  R2.at<double>(0, j) = R1.at<double>(1, j);
                  R2.at<double>(1, j) = R1.at<double>(0, j);
                  R2.at<double>(2, j) = - R1.at<double>(k, j);
              }
        }
        for (int j = 0; j < 3; j++) {
              for (int k = 0; k < 3; k++) {
                     R.at<double>(j, k) = 0.0;
                     for (int l = 0; l < 3; l++) {
                             R.at<double>(j, k) += A.at<double>(j, l) * R2.at<double>(l, k);
                      }
               }
        }
        eulerAngsTmp = rotationMatrixToEulerAngles(R);
        // Converting angles to degrees
        eulerAngsTmp = eulerAngsTmp * 180.0 / M_PI;
        eulerAngs += eulerAngsTmp;
    }
    eulerAngs = Vec3d(eulerAngs[0]/markerLen, eulerAngs[1] / markerLen, eulerAngs[2] / markerLen);

    if(eulerAngs[0]>360)eulerAngs[0] = eulerAngs[0]/1000; // Numeric imprecision correction
    if(eulerAngs[1]>360)eulerAngs[1] = eulerAngs[1]/1000;
    if(eulerAngs[2]>360)eulerAngs[2] = eulerAngs[2]/1000;

    this->attitude[OFFSET_RODRIGUES + axis] = eulerAngs;
}

#endif /* _ADCV_CPP_ */
