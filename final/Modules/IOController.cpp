#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <thread>
#include <chrono>
#include "data.hpp"
#include "IOController.hpp"

using namespace std;

IOController::IOController(const char* InputDriver, const char* OutputDriver){
    this->Valid = 1;
    this->_isDisposing = 0;
    this->_isPolling = 0;
    this->_input = open(InputDriver, O_RDWR);
    this->_output = open(OutputDriver, O_RDWR); 

    if(this->_input == -1 || this->_output == -1){
        this->Valid = 0;
        close(this->_input);
        close(this->_output);
        return;
    }

    read(this->_input, &this->Input, 1);
    read(this->_output, &this->Output, 1);
}

void IOController::StartPolling(){
    if(!this->_poller.joinable()){
        this->_isPolling = 1;
        this->_poller = thread([this] { this->Polling(); });
    }
}

void IOController::StopPolling(){
    if(this->_poller.joinable()){
        this->_isPolling = 0;
        this->_poller.join();
    }
}

void IOController::Dispose(){
    this->_isDisposing = 1;
    this->StopPolling();
    close(this->_input);
    close(this->_output);
}

void IOController::Polling(){
    while(!this->_isDisposing && this->_isPolling){
        read(this->_input, &this->Input, 1);
        write(this->_output, &this->Output, 1);

        this_thread::sleep_for(chrono::milliseconds(50));
    }
}