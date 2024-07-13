// Compile by: g++ -o compiledFileName fileName.cpp `pkg-config --cflags --libs opencv4`


#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    // Define the camera matrix
    Mat cameraMatrix = (Mat_<float>(3, 3) << 623.81783319883937, 0., 317.38282943395228, 
                                              0., 622.68755432735418, 242.89225567197289, 
                                              0., 0., 1.);

    // Define the distortion coefficients
    Mat distCoeffs = (Mat_<float>(5, 1) << 0.15893814231547621, -0.0046864004444970367,
                                           -0.0060795549988802051, 0.0046454683004434338,
                                           -0.91766726985059655);

    // Define the 3D points in the world coordinate system

    float x, y, z;
    cout << "Enter the 3D coordinates (X, Y, Z) seperated by spaces: ";
    cin >> x >> y >> z;
    
    vector<Point3f> points3d;
    points3d.push_back(Point3f(x, y, z));

    cout << "Enter the 3D coordinates (X, Y, Z) of the rotation vector seperated by spaces: ";
    cin >> x >> y >> z;
    Mat rvec = (Mat_<double>(3, 1) << x, y, z);

    cout << "Enter the 3D coordinates (X, Y, Z) of the translation vector seperated by spaces: ";
    cin >> x >> y >> z;
    Mat tvec = (Mat_<double>(3, 1) << x, y, z);

    // Project the 3D points to the 2D image plane
    vector<Point2f> points2d;
    projectPoints(points3d, rvec, tvec, cameraMatrix, distCoeffs, points2d);

    // Display the 2D points
    cout << "2D Point: " << points2d[0] << endl;

    return 0;
}
