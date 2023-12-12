#include "ConvyerController.hpp"

using namespace std;

ConvyerController::ConvyerController(){
    this->Valid = 0;
    this->_isDisposing = 0;
    this->_isPolling = 0;
}

ConvyerController::ConvyerController(const char* ConvyerDriver){
    this->Valid = 1;
    this->_isDisposing = 0;
    this->_isPolling = 0;
    this->_driver = open(ConvyerDriver, O_RDWR);

    this->Status = NONE;

    if(this->_driver == -1){
        this->Valid = 0;
        close(this->_driver);
        return;
    }

    write(this->_driver, &(this->Status), 4);
}

void ConvyerController::StartPolling(){
    if(!this->_poller.joinable() && this->Valid){
        this->_isPolling = 1;
        this->_poller = thread([this] { this->Polling(); });
    }
}

void ConvyerController::StopPolling(){
    if(this->_poller.joinable() && this->Valid){
        this->_isPolling = 0;
        this->_poller.join();
    }
}

void ConvyerController::Dispose(){
    int tmp = 0;
    this->_isDisposing = 1;
    this->StopPolling();
    write(this->_driver, &tmp, 4);
    close(this->_driver);
}

void ConvyerController::Polling(){
    int temp = 0;

    while(!this->_isDisposing && this->_isPolling){
        temp = (int)this->Status;
        write(this->_driver, &temp, 4);
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}