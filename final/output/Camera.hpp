#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include "data.hpp"

using namespace std;
using namespace cv;

class Camera{
    private:
        VideoCapture cam;
        thread _poller;
        int _isDisposing;
        int _isPolling;
        void Polling();

    public:
        Mat frame;
        int Valid;
        Camera();
        Camera(int CamID);
        int Capture();
        void Dispose();
};

#endif