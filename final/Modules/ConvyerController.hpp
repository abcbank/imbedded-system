#ifndef CONVYERCONTROLLER_H
#define CONVYERCONTROLLER_H

// #include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include "data.hpp"

using namespace std;

class ConvyerController{
    private:
        thread _poller;
        thread _eventInvoker;
        int _driver;
        int _isDisposing;
        int _isPolling;
        void Polling();
    public:
        void *ParentAddr;
        ConvyerStatus Status;
        int Valid;

        ConvyerController();
        ConvyerController(const char* ConvyerDriver);
        void StartPolling();
        void StopPolling();
        void Dispose();
};

#endif