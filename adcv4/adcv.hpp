
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat)
#include <opencv2/videoio.hpp>  // Video write
#include "opencv2/opencv.hpp"
#include <opencv2/video/tracking.hpp>  // KF
#include <iostream>
#include <string>
// #include <chrono>
#include <stdexcept>

#ifndef _ADCV_HPP_
#define _ADCV_HPP_

// #define CAMX_ID 2
// #define CAMY_ID 1
// #define CAMZ_ID 0

#include <cameraIds.hpp>

// #define ATTITUDE_VEC_SIZE           9
// #define TRANSLATION_VEC_SIZE        3
// #define MARKER_LENGTH             0.03f

const int ATTITUDE_VEC_SIZE = 9;
const int TRANSLATION_VEC_SIZE = 3;
const float MARKER_LENGTH = 0.03f;

/**
  ADCV class
*/
class ADCV
{
public:
    ADCV(bool All, bool rec);
    ADCV(std::string X_file_name, std::string Y_file_name, std::string Z_file_name, bool rec);
    ~ADCV();
    bool initCamera();
    cv::VideoCapture openCamera(int camID);
    cv::VideoCapture openCamera(std::string camID);
    bool getFrame(int cam);
    void showImage();
    void drawAruco(int axis);
    time_t showDate();
    void initRecVideo(time_t now);
    void recVideo();
    bool readCameraParameters();
    bool readDetectorParameters(cv::Ptr<cv::aruco::DetectorParameters> &params);
    bool setup();
  //attitude determination
    void determAttitude(int cam);
    void getAruco(int cam);
    void estimatePose(int cam);
    void computeRodriguesVec(int markerLen, std::vector<cv::Vec3d> rvecs, int axis);
    bool isRotationMatrix(cv::Mat &R);
    cv::Vec3d rotationMatrixToEulerAngles(cv::Mat &R);
    void computeEulersVec(int markerLen, std::vector<cv::Vec3d> rvecs, int axis);
  //void computeQuaternion();

    char c = '\0'; // Key to quit

  /*** Camera Attributes
  */
    int camX_id = CAMX_ID;
    int camY_id = CAMY_ID;
    int camZ_id = CAMZ_ID;
    bool camX_on = false, camY_on = false, camZ_on = false, video = false;
    bool rec;
    std::string camX_name, camY_name, camZ_name;
    cv::VideoWriter outX,outY,outZ,outX_raw,outY_raw,outZ_raw;
    bool frameToSend = false;
  /*** Aruco Attributes
  */
    std::vector<int> markerIdsZ, markerIdsX, markerIdsY;
    float markerLength = MARKER_LENGTH;
    std::vector<std::vector<cv::Point2f>> markerCornersZ, markerCornersX, markerCornersY;
  /*** Pose Attributes
  */
    std::vector<cv::Vec3d> rvecX,rvecY,rvecZ,tVecX,tVecY,tVecZ;
    std::vector<cv::Vec3d> attitude = std::vector<cv::Vec3d>(ATTITUDE_VEC_SIZE); // Rotation vector, rodrigues, euler
    std::vector<cv::Vec3d> tVec = std::vector<cv::Vec3d>(TRANSLATION_VEC_SIZE);   // Translation vectors
private:
  /*** Time measurement
  */
    // std::chrono::steady_clock::time_point startTime;
    // std::chrono::steady_clock::time_point currentTime;
    // Por que strings? Inmenso desperdício de memória e tempo de processamento
    // std::chrono::duration<double, std::milli> t;
    // std::vector<std::string> timeElapsed;
    // std::vector<std::vector<std::string>> time_ms;
  // CAMERA
    std::string recFile_name = "./adcv4/data/outData/";
    cv::Mat rawX, rawY, rawZ; // Raw frame
    cv::Mat imgX, imgY, imgZ; // Modified Raw frame
    cv::VideoCapture captureX,captureY,captureZ;
    cv::Mat intrinsics, distortion;
  // ARUCO
    cv::Ptr<cv::aruco::DetectorParameters> aruco_parameters;
    cv::Ptr<cv::aruco::Dictionary> aruco_dictionary;
};

#endif /* _ADCV_HPP_ */
