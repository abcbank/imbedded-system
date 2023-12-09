#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <thread>
#include "data.hpp"

using namespace std;

class IOController{
    private:
        thread _poller = thread();
        int _input;
        int _output;
    public:
        PinArray Input;
        PinArray Output;
        IOController(const char* InputDriver, const char* OutputDriver);
        void StartPolling();
        void StopPolling();
};

#endif