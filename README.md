
# Real-Time 3D Vector Projection Using OpenCV

## Overview
This project demonstrates real-time projection of 3D points onto a 2D video stream using OpenCV. The system allows users to input 3D coordinates, which are then projected onto the video feed. Additionally, users can click on the video feed to retrieve the corresponding 3D coordinates.

## Features
- **Real-time video processing**: Capture video from a webcam and project 3D points onto the video stream.
- **Mouse interaction**: Click on the video to get the 2D and corresponding 3D coordinates.
- **Dynamic input**: Press 'R' to input new 3D points to be projected.
- **Crosshair display**: Display a crosshair in the center of the video for reference.

## Requirements
- OpenCV (version 4.x or later)
- C++11 or later
- Camera

## Usage
1. Run the calibration file inside ‘camera-calibration’ by running the following command: `./calib data/ 0.025 5 7`, which will automatically calibrate the camera connected to your laptop.
2. Running the code will produce a file containing the intrinsic parameters of the camera which is the most crucial part for projecting points into our video streams.
3. Transfer the file “calibration_data.yaml” into our top directory.
4. Compile our “project.cpp” file by running the following command: 
   ```sh
   g++ -o project project.cpp `pkg-config --cflags --libs opencv4`
   ```
5. Now by running the file, you will be asked to enter the rotation and translation vector coordinates which will determine the location of your 3D Coordinate System with respect to your camera.
6. This will determine your camera location which will be outputted in the terminal.
7. You will then be asked to enter your (X, Y, Z) Vector coordinates which will be projected onto the 3D Coordinates System and will be visible in the camera video stream if the point is in the camera’s field of view.
8. Pressing ‘R’ or ‘r’ will allow you to enter another point, and pressing ‘esc’ will terminate the program.
9. The camera position with respect to the coordinate system will also be calculated.
10. The “3Dto2D.cpp” file will convert any 3D point to a 2D point.
11. The “Video_Coordinates.cpp” file will allow you to specify the location of the 3D system with respect to your camera, and clicking on any point in the video stream will give you the coordinates of that point after specifying the z-coordinate.

