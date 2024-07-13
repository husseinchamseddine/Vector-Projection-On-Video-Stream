// Compile by: g++ -o compiledFileName fileName.cpp `pkg-config --cflags --libs opencv4`

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat frame;
Mat cameraMatrix, distCoeffs, rvec, tvec;

double z; // Global variable to store the Z coordinate

// Function to load calibration data from a YAML file
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

void mouseClick(int event, int x, int y, int flags, void* param) {
    if (event == EVENT_LBUTTONDOWN) {
        // Display coordinates on the console
        cout << "2D Point: " << x << ", " << y << " -> ";

        // Calculate the 3D coordinates
        Mat uvPoint = (Mat_<double>(3, 1) << x, y, 1);
        Mat normalizedUVPoint = cameraMatrix.inv() * uvPoint;
        Mat rotationMatrix;
        Rodrigues(rvec, rotationMatrix);

        Mat point3D = rotationMatrix.inv() * (normalizedUVPoint * z - tvec);

        // Display coordinates on the console
        cout << "3D Point: (" << point3D.at<double>(0) << ", " << point3D.at<double>(1) << ", " << point3D.at<double>(2) << ")" << endl;

        // Display coordinates on the video
        putText(frame, to_string(x) + "," + to_string(y), Point(x, y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
        imshow("Video", frame);
    }
}

int main() {
    // Load camera calibration data
    if (!loadCalibrationData(cameraMatrix, distCoeffs)) {
        return -1;
    }

    cout << "Enter the 3D coordinates (X, Y, Z) of the rotation vector separated by spaces: ";
    float rx, ry, rz;
    cin >> rx >> ry >> rz;
    rvec = (Mat_<double>(3, 1) << rx, ry, rz);

    cout << "Enter the 3D coordinates (X, Y, Z) of the translation vector separated by spaces: ";
    float tx, ty, tz;
    cin >> tx >> ty >> tz;
    tvec = (Mat_<double>(3, 1) << tx, ty, tz);

    cout << "Enter the Z coordinate: ";
    cin >> z;

    // Open the camera
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Failed to open the camera." << endl;
        return -1;
    }

    namedWindow("Video");

    // Set the mouse callback function
    setMouseCallback("Video", mouseClick);

    while (true) {
        cap >> frame;  // Capture a new frame from the camera
        if (frame.empty()) {
            cerr << "Failed to capture an image." << endl;
            break;
        }

        Size frameSize = frame.size();
        int width = frameSize.width;
        int height = frameSize.height;
        line(frame, Point(0, height / 2), Point(width, height / 2), Scalar(255), 2, 8, 0);
        line(frame, Point(width / 2, 0), Point(width / 2, height), Scalar(255), 2, 8, 0);
        imshow("Video", frame);

        // Wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        if (waitKey(30) == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
