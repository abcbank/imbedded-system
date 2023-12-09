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

using namespace std;

class Master{
    private:
    public:

        IOController IO;
        SegmentController Segment;

        int Valid;

        Master(const char* InputDriver, const char* OutputDriver, const char* SegmentDriver);
        void StartPolling();
        void StopPolling();
        void Dispose();
};

#endif