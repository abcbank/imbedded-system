#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

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

class IOController{
    private:
        thread _poller;
        thread _eventInvoker;
        int _input;
        int _output;
        int _isDisposing;
        int _isPolling;
        void Polling();
        void AwakeEvent(PinArray PrevValue);
    public:
        void *ParentAddr;
        int Valid;

        PinArray Input;
        PinArray Output;

        vector<void (*)(IOController*, int)> InputEnabled[8];
        vector<void (*)(IOController*, int)> InputDisabled[8];

        IOController();
        IOController(const char* InputDriver, const char* OutputDriver);
        void StartPolling();
        void StopPolling();
        void Dispose();
};

#endif