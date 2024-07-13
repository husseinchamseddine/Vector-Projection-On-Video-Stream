// Compile by: g++ -o compiledFileName fileName.cpp `pkg-config --cflags --libs opencv4`

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

// Function to load our calibration data 
bool loadCalibrationData(Mat& cameraMatrix, Mat& distCoeffs) {
    
    FileStorage fs("calibration_data.yaml", FileStorage::READ);
    
    if (!fs.isOpened()) {
        cerr << "Failed to open calibration file." << endl;
        return false;
    }

    fs["camera_matrix"] >> cameraMatrix;
    fs["dist_coeffs"] >> distCoeffs;
    fs.release();
    return true;
}

int main() {

    Mat cameraMatrix, distCoeffs; // Load camera calibration data
    if (!loadCalibrationData(cameraMatrix, distCoeffs)) {
        return -1;
    }

    // Check validity of calibration data
    if (cameraMatrix.empty() || distCoeffs.empty()) {
        cerr << "Invalid calibration data." << endl;
        return -1;
    }

    // Open the camera (currently a webcam)
    VideoCapture cap(0);  
    if (!cap.isOpened()) {
        cerr << "Failed to open the camera." << endl;
        return -1;
    }   

   
    float x, y, z;
   
    cout << "Enter the 3D coordinates (X, Y, Z) of the rotation vector seperated by spaces: ";
    cin >> x >> y >> z;
    Mat rvec = (Mat_<double>(3, 1) << x, y, z);
    

    cout << "Enter the 3D coordinates (X, Y, Z) of the translation vector seperated by spaces: ";
    cin >> x >> y >> z;
    Mat tvec = (Mat_<double>(3, 1) << x, y, z);


    // Rotation Vector (rvec): Represents the orientation of the 3D system relative to the camera. 
    // It provides the rotation needed to align the coordinate system with the camera's coordinate system. 

    // Translation Vector (tvec): Represents the position of the 3d system relative to the camera in 3D space.
    // It gives the coordinates (x, y, z) indicating how far the marker is from the camera along the three axes.



    // Calculating the camera position with respect to the 3D coordinate system
    Mat rotationMatrix;
    Rodrigues(rvec, rotationMatrix);
    Mat cameraPosition = -rotationMatrix.t() * tvec;
    cout << "Camera Position:\n" << cameraPosition << endl;


    // Define 3D world coordinate points
    vector<Point3f> objectPoints;
    cout << "Enter the 3D coordinates (X, Y, Z) separated by spaces: ";
    cin >> x >> y >> z;
    objectPoints.push_back(Point3f(x, y, z));  

   
    // Mat::zeros(3, 1, CV_64F) initializes a 3x1 matrix (a vector) with zeros.
    // This means there is initially no rotation applied, indicating that the 3D coordinate system is aligned with the camera's coordinate system.

    //Mat::zeros(3, 1, CV_64F) initializes a 3x1 matrix (a vector) with zeros. 
    //This means there is initially no translation, indicating that the origin of the 3D coordinate system is at the same position as the camera's origin.
 
    // Counter for flashing effect
    int counter = 0;

    while (true) {
        
        Mat frame;
        cap >> frame;  // Capture a new frame from the camera
        
        if (frame.empty()) {
            cerr << "Failed to capture an image." << endl;
            break;
        }

        // Project 3D points to 2D image plane
        vector<Point2f> imagePoints;
        try {
        
            projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);
        
        } catch (const Exception& e) {
        
            cerr << "Error during projecting points: " << e.what() << endl;
            break;
        
        }

        // Draw the projected points on the frame
        if (counter % 20 < 10) { // Toggle visibility every 10 frames
            for (size_t i = 0; i < imagePoints.size(); i++) {
                circle(frame, imagePoints[i], 5, Scalar(0, 0, 255), -1);
            }
        }
        counter++;
        

        // Display the frame with projected points
        imshow("Projected Points", frame);

        // Break the loop if 'esc' is pressed
        char key = waitKey(30);
        if (key == 27) { // 'esc' key to break the loop
            break;
        } else if (key == 'R' || key == 'r') { // 'R' key to input another point
            cout << "Enter the 3D coordinates (X, Y, Z) separated by spaces: ";
            cin >> x >> y >> z;
            objectPoints.push_back(Point3f(x, y, z));
        }
    }

    // Release the camera
    cap.release();
    destroyAllWindows();

    return 0;
}
