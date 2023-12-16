#include "Camera.hpp"

using namespace std;

Camera::Camera(){
    this->Valid = 0;
    this->_isDisposing = 0;
    this->_isPolling = 0;
    this->pause = 0;
}
Camera::Camera(int CamID){
    this->cam = VideoCapture(CamID);
    this->Valid = 1;
    this->_isDisposing = 0;
    this->_isPolling = 0;
    this->pause = 0;

    if(!cam.isOpened()){
        this->Valid = 0;
        return;
    }    
}

int Camera::Capture(){
    if(!this->Valid) return 0;
    // else return 0;
    cam.read(frame);
    return !this->frame.empty();
}

void Camera::Dispose(){
    this->_isDisposing = 1;
    this->StopPolling();
}

void Camera::StartPolling(){
    if(!this->_poller.joinable() && this->Valid){
        this->_isPolling = 1;
        this->_poller = thread([this] { this->Polling(); });
    }
}

void Camera::StopPolling(){
    if(this->_poller.joinable() && this->Valid){
        this->_isPolling = 0;
        this->_poller.join();
    }
}

void Camera::Polling(){
    int counter = 0;

    while(!this->_isDisposing && this->_isPolling){
        if(!this->pause)
            cam.read(frame);
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}