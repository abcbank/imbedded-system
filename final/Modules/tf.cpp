#include "tf.hpp"
TFModel::TFModel(){
    this->Valid = 0;
}
TFModel::TFModel(const char* Model){

    Py_Initialize();

    this->IsRunning = 0;
    this->_isDisposing = 0;
    this->Valid = 1;
}