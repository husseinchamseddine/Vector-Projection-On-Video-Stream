#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using namespace cv;



void createArucoMarkers(){

    Mat outputMarker;

    aruco::Dictionary markerDictionary = aruco::getPredefinedDictionary(aruco::PredefinedDictionaryType::DICT_4X4_50);
    
    
    for(int i = 0; i<50; i++){
        
        aruco::generateImageMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert;
        string imageName = "4x4Marker_";
        convert << imageName << i << ".png";
        imwrite(convert.str(), outputMarker);

    }

}

int main(){

    createArucoMarkers();

}

