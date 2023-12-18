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
#include "ConvyerController.hpp"
#include "tf.hpp"
// #include "HCSR04.hpp"

using namespace std;

class Master{
    private:
        thread proc;
        int _isDisposing;
        int _isPolling;
        // void EnableTest(IOController* ctrl, int Pin);
        // void DisableTest(IOController* ctrl, int Pin);
    public:
        volatile int IsBusy;
        MasterStatus Status;
        SequenceStatus SeqStatus;
        Mat TarFrame;
        ConvyerController Convyer;
        IOController IO;
        SegmentController Segment;
        TFModel TF_lite;
        Camera Capture;
        // Sonar Sensor;
        
        int Valid;
        Master(const char* InputDriver, const char* OutputDriver, const char* SegmentDriver, const char * ConvyerDriver, int CamID);
        void SetStatus(MasterStatus Status);
        void SetSequenceStatus(SequenceStatus Status);
        void StartPolling();
        void StopPolling();
        void Dispose();
};
void EnableTest(IOController *ctrl, int Pin);
void DisableTest(IOController *ctrl, int Pin);

#endif