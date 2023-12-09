#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <thread>
#include <chrono>
#include "data.hpp"
#include "SegmentController.hpp"

using namespace std;

SegmentController::SegmentController(const char* SegDriver){
    this->Valid = 1;
    this->_isDisposing = 0;
    this->_isPolling = 0;
    this->_driver = open(SegDriver, O_RDWR);

    this->Values[0] = 0;
    this->Values[1] = 0;
    this->Values[2] = 0;
    this->Values[3] = 0;

    if(this->_driver == -1){
        this->Valid = 0;
        close(this->_driver);
        return;
    }
}

void SegmentController::StartPolling(){
    if(!this->_poller.joinable()){
        this->_isPolling = 1;
        this->_poller = thread([this] { this->Polling(); });
    }
}

void SegmentController::StopPolling(){
    if(this->_poller.joinable()){
        this->_isPolling = 0;
        this->_poller.join();
    }
}

void SegmentController::Dispose(){
    this->_isDisposing = 1;
    this->StopPolling();
    close(this->_driver);
}

void SegmentController::Polling(){
    int counter = 0;

    while(!this->_isDisposing && this->_isPolling){
        int tmp = ((1) << counter + 4) | this->Values[counter];
        printf("%x\n",tmp);
        write(this->_driver, &tmp, 4);
        counter = (counter + 1) % 4;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}