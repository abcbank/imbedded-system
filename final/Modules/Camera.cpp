#include "Camera.hpp"

using namespace std;

Camera::Camera(){
    this->Valid = 0;
    this->_isDisposing = 0;
    this->_isPolling = 0;
}
Camera::Camera(int CamID){
    this->cam = VideoCapture(CamID);
    this->Valid = 1;
    this->_isDisposing = 0;
    this->_isPolling = 0;

    if(!cam.isOpened()){
        this->Valid = 0;
        return;
    }    
}

int Camera::Capture(){
    if(!this->Valid) return 0;
    // else return 0;
    this->cam >> this->frame;
    return this->frame.empty();
}
void Camera::Dispose(){
    this->_isDisposing = 1;
}