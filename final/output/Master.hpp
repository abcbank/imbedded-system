#ifndef MASTER_H
#define MASTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <chrono>
#include <thread>
#include "data.hpp"
#include "SegmentController.hpp"
#include "IOController.hpp"
#include "Camera.hpp"

using namespace std;

class Master{
    private:
        int _isDisposing;
        int _isPolling;
        // void EnableTest(IOController* ctrl, int Pin);
        // void DisableTest(IOController* ctrl, int Pin);
    public:

        IOController IO;
        SegmentController Segment;
        Camera Capture;
        int Valid;
        Master(const char* InputDriver, const char* OutputDriver, const char* SegmentDriver, int CamID);
        void StartPolling();
        void StopPolling();
        void Dispose();
};
void EnableTest(IOController *ctrl, int Pin);
void DisableTest(IOController *ctrl, int Pin);

#endif