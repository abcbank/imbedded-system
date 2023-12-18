#ifndef TF_H
#define TF_H

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
#include <python3.9/Python.h>

using namespace std;

class TFModel{
    private:
        thread _eventInvoker;
        int _driver;
        int _isDisposing;
    public:
        void *ParentAddr;
        int Valid;
        int IsRunning;

        TFModel();
        TFModel(const char* ModePath);
        void StartPolling();
        void StopPolling();
        void Dispose();
};

#endif