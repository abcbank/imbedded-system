#include "IOController.hpp"

using namespace std;


IOController::IOController(){
    this->Valid = 0;
    this->_isDisposing = 0;
    this->_isPolling = 0;
}

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

    this->Output.Pin_01 = 0;
    this->Output.Pin_02 = 0;
    this->Output.Pin_03 = 0;
    this->Output.Pin_04 = 0;
    this->Output.Pin_05 = 0;
    this->Output.Pin_06 = 0;
    this->Output.Pin_07 = 0;
    this->Output.Pin_08 = 0;

    read(this->_input, &this->Input, 1);
    write(this->_output, &this->Output, 1);
}

void IOController::StartPolling(){
    if(!this->_poller.joinable() && this->Valid){
        this->_isPolling = 1;
        this->_poller = thread([this] { this->Polling(); });
    }
}

void IOController::StopPolling(){
    if(this->_poller.joinable() && this->Valid){
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

void IOController::AwakeEvent(PinArray PrevValue){

        if(PrevValue.Pin_01 != this->Input.Pin_01){
            if(this->Input.Pin_01 == 0){
                for(auto f : this->InputDisabled[0])
                    f(this, 0);
            }
            else{
                for(auto f : this->InputEnabled[0])
                    f(this, 0);
            }
        }

        if(PrevValue.Pin_02 != this->Input.Pin_02){
            if(this->Input.Pin_02 == 0){
                for(auto f : this->InputDisabled[1])
                    f(this, 1);
            }
            else{
                for(auto f : this->InputEnabled[1])
                    f(this, 1);
            }
        }

        if(PrevValue.Pin_03 != this->Input.Pin_03){
            if(this->Input.Pin_03 == 0){
                for(auto f : this->InputDisabled[2])
                    f(this, 2);
            }
            else{
                for(auto f : this->InputEnabled[2])
                    f(this, 2);
            }
        }

        if(PrevValue.Pin_04 != this->Input.Pin_04){
            if(this->Input.Pin_04 == 0){
                for(auto f : this->InputDisabled[3])
                    f(this, 3);
            }
            else{
                for(auto f : this->InputEnabled[3])
                    f(this, 3);
            }
        }

        if(PrevValue.Pin_05 != this->Input.Pin_05){
            if(this->Input.Pin_05 == 0){
                for(auto f : this->InputDisabled[4])
                    f(this, 4);
            }
            else{
                for(auto f : this->InputEnabled[4])
                    f(this, 4);
            }
        }

        if(PrevValue.Pin_06 != this->Input.Pin_06){
            if(this->Input.Pin_06 == 0){
                for(auto f : this->InputDisabled[5])
                    f(this, 5);
            }
            else{
                for(auto f : this->InputEnabled[5])
                    f(this, 5);
            }
        }

        if(PrevValue.Pin_07 != this->Input.Pin_07){
            if(this->Input.Pin_07 == 0){
                for(auto f : this->InputDisabled[6])
                    f(this, 6);
            }
            else{
                for(auto f : this->InputEnabled[6])
                    f(this, 6);
            }
        }

        if(PrevValue.Pin_08 != this->Input.Pin_08){
            if(this->Input.Pin_08 == 0){
                for(auto f : this->InputDisabled[7])
                    f(this, 7);
            }
            else{
                for(auto f : this->InputEnabled[7])
                    f(this, 7);
            }
        }
}

void IOController::Polling(){
    while(!this->_isDisposing && this->_isPolling){
        PinArray Prev = this->Input;

        read(this->_input, &this->Input, 1);
        write(this->_output, &this->Output, 1);
        AwakeEvent(Prev);
        //if (!this->_eventInvoker.joinable()) this->_eventInvoker = thread([this, Prev] {this->AwakeEvent(Prev);});

        this_thread::sleep_for(chrono::milliseconds(15));
    }
}