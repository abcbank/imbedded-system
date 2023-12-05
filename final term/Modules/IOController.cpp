#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <thread>
#include "data.hpp"
#include "IOController.hpp"

using namespace std;

IOController::IOController(string InputDriver, string OutputDriver){
    this->_input = open(InputDriver, O_RDWR); 
    this->_output = open(OutputDriver, O_RDWR); 

    read(this->_input, &this->Input, 1);
    read(this->_output, &this->Output, 1);
}

void IOController::StartPolling(){

}
void IOController::StopPolling(){
    
}