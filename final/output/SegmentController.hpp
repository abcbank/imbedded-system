#ifndef SEGMENTCONTROLLER_H
#define SEGMENTCONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <chrono>
#include <thread>
#include "data.hpp"

using namespace std;

class SegmentController{
    private:
        thread _poller;
        int _isDisposing;
        int _isPolling;
        int _driver;
        void Polling();
        void displaySegment();
    public:
        int Valid;
        unsigned char Values[4];

        SegmentController();
        SegmentController(const char* InputDriver);
        void StartPolling();
        void StopPolling();
        void Dispose();
};

#endif