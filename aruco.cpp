// Compile by: g++ -o compiledFileName fileName.cpp `pkg-config --cflags --libs opencv4`

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;



int startWebCamMonitoring(const Mat& cameraMatrix, const Mat& distanceCoeffiecints, float arucoSquareDimension){

    Mat frame;

    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    aruco::DetectorParameters parameters;
    
    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::PredefinedDictionaryType::DICT_4X4_50);
    Ptr<aruco::Dictionary> markerDictionary = makePtr<aruco::Dictionary>(dictionary);


    VideoCapture vid(0);

    if(!vid.isOpened()){
        return -1;
    }

    namedWindow("Webcam", WINDOW_AUTOSIZE);

    vector<Vec3d> rotationVectors, translationVectors;

    while(true){

        if(!vid.read(frame)){
            break;
        }

        aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
        aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoeffiecints, rotationVectors, translationVectors);

        for(int i = 0; i < markerIds.size(); i++){

            drawFrameAxes(frame, cameraMatrix, distanceCoeffiecints, rotationVectors[i], translationVectors[i], 0.1f);
            cout << rotationVectors[i] << " " << translationVectors[i] << endl;

        }

        imshow("Webcam", frame);

        if(waitKey(30) >= 0){
            break;
        }
        
    }

    return 1;

}

bool loadCameraCalibration(Mat& cameraMatrix, Mat& distanceCoeffiecints) {
    
    FileStorage fs("calibration_data.yaml", FileStorage::READ);
    
    if (!fs.isOpened()) {
        cerr << "Failed to open calibration file." << endl;
        return false;
    }

    fs["camera_matrix"] >> cameraMatrix;
    fs["dist_coeffs"] >> distanceCoeffiecints;
    fs.release();
    return true;

}

int main(){

    Mat cameraMatrix, distanceCoeffiecints; 

    if (!loadCameraCalibration(cameraMatrix, distanceCoeffiecints)) {
        return -1;
    }

    if (cameraMatrix.empty() || distanceCoeffiecints.empty()) {
        cerr << "Invalid calibration data." << endl;
        return -1;
    }

    startWebCamMonitoring(cameraMatrix, distanceCoeffiecints, 0.142f);

    return 0;

}