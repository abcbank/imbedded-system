#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <thread>
#include <chrono>
#include "data.hpp"
#include "Master.hpp"
#include "SegmentController.hpp"
#include "IOController.hpp"

using namespace std;

Master::Master(const char* InputDriver, const char* OutputDriver, const char* SegDriver){
    this->Valid = 0;
    this->_isDisposing = 0;
    this->_isPolling = 0;

    this->Segment = SegmentController(SegDriver);
    this->IO = IOController(InputDriver, OutputDriver);

    if(this->Segment.Valid && this->IO.Valid){
        this->Valid = 1;
    }
}

void SegmentController::StartPolling(){
    if(!this->_poller.joinable()){
        this->_isPolling = 1;
        this->Segment.StartPolling();
        this->IO.StartPolling();
    }
}

void SegmentController::StopPolling(){
    if(this->_poller.joinable()){
        this->_isPolling = 0;
        this->Segment.StopPolling();
        this->IO.StopPolling();
    }
}

void SegmentController::Dispose(){
    this->_isDisposing = 1;
    this->Segment.Dispose();
    this->IO.Dispose();
}