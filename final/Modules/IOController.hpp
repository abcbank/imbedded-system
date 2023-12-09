#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

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

class IOController{
    private:
        thread _poller;
        int _input;
        int _output;
        int _isDisposing;
        int _isPolling;
        void Polling();
    public:
        int Valid;
        PinArray Input;
        PinArray Output;
        IOController(const char* InputDriver, const char* OutputDriver);
        void StartPolling();
        void StopPolling();
        void Dispose();
};

#endif