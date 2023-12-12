#include "Master.hpp"

using namespace std;

void Input01_Pressed(IOController* ctrl, int Pin){
    Master *master = (Master*)ctrl->ParentAddr;
    if((master->Status == STOP | master->Status == READY)& master->Valid){
        master->SetStatus(RUN);
        master->Convyer.Status = A;
        // 1. Convyer Start
        // 2. if Sensor Enabled
        // 3. Convyer Stop
        // 4. Get Screenshot
        // 5. type detection by tensorflow model
        // 6. Control Robot
        // 7. go to 1
    }
}

void Input02_Pressed(IOController* ctrl, int Pin){
    Master *master = (Master*)ctrl->ParentAddr;
    if(master->Status == RUN & master->Valid){
        master->SetStatus(STOP);
        master->Convyer.Status = NONE;
        // 1. Convyer Stop
        // 2. Robot Stop
    }
}

void Input03_Pressed(IOController* ctrl, int Pin){
    Master *master = (Master*)ctrl->ParentAddr;
    if(master->Status != RUN & master->Valid){
        master->SetStatus(RESET);

    }
}

void EnableTest(IOController* ctrl, int Pin){
    // ctrl->Output.Pins = ctrl->Output.Pins | (1 << Pin);
    // auto parent = (Master*)ctrl->ParentAddr;
    // parent->Segment.Values[Pin] = 1;
    //*(unsigned char*)(&(ctrl->Output)) = *(unsigned char*)(&(ctrl->Output)) & 1 << Pin;
    printf("Pin %d Enabled\n");
}

void DisableTest(IOController* ctrl, int Pin){
    // ctrl->Output.Pins = ctrl->Output.Pins & ~(1 << Pin);
    // auto parent = (Master*)ctrl->ParentAddr;
    // parent->Segment.Values[Pin] = 0;
    //*(unsigned char*)(&(ctrl->Output)) = *(unsigned char*)(&(ctrl->Output)) & 0 << Pin;
    printf("Pin %d Disabled\n");
}

Master::Master(const char* InputDriver, 
                const char* OutputDriver, 
                const char* SegDriver,
                const char* ConvyerDriver,
                int CamID){

    this->Status = READY;
    this->Valid = 0;
    this->_isDisposing = 0;
    this->_isPolling = 0;

    this->Segment = SegmentController(SegDriver);
    this->IO = IOController(InputDriver, OutputDriver);
    this->Convyer = ConvyerController(ConvyerDriver);
    this->Capture = Camera(CamID);

    this->Segment.ParentAddr = this;
    this->IO.ParentAddr = this;
    this->Capture.ParentAddr = this;
    this->Convyer.ParentAddr = this;

    auto test = EnableTest;
    this->IO.InputEnabled[0].push_back((void (*)(IOController*, int))test);
    this->IO.InputEnabled[1].push_back((void (*)(IOController*, int))test);
    this->IO.InputEnabled[2].push_back((void (*)(IOController*, int))test);
    this->IO.InputEnabled[0].push_back((void (*)(IOController*, int))Input01_Pressed);
    this->IO.InputEnabled[1].push_back((void (*)(IOController*, int))Input02_Pressed);
    this->IO.InputEnabled[2].push_back((void (*)(IOController*, int))Input03_Pressed);

    test = DisableTest;
    this->IO.InputDisabled[0].push_back((void (*)(IOController*, int))test);
    this->IO.InputDisabled[1].push_back((void (*)(IOController*, int))test);
    this->IO.InputDisabled[2].push_back((void (*)(IOController*, int))test);
    
    std::function<void()> temp;
    if(this->Segment.Valid 
    && this->IO.Valid 
    //&& this->Capture.Valid 
    && this->Convyer.Valid){
        this->Valid = 1;
    }
}

void Master::SetStatus(MasterStatus Status){
    this->Status = Status;
    switch(Status){
        case READY:
            this->IO.Output.Pin_01 = 0;
            this->IO.Output.Pin_02 = 0;
            this->IO.Output.Pin_03 = 0;
        break;
        case RUN:
            this->IO.Output.Pin_01 = 1;
            this->IO.Output.Pin_02 = 0;
            this->IO.Output.Pin_03 = 0;
        break;
        case STOP:
            this->IO.Output.Pin_01 = 0;
            this->IO.Output.Pin_02 = 0;
            this->IO.Output.Pin_03 = 1;
        break;
        case RESET:
            this->IO.Output.Pin_01 = 0;
            this->IO.Output.Pin_02 = 1;
            this->IO.Output.Pin_03 = 0;
        break;
    }
}

// void Master::RunProcess(){

// }

// void Master::StopProcess(){

// }

// void Master::ResetProcess(){
//     System
// }

void Master::StartPolling(){
    if(!this->_isPolling){
        this->_isPolling = 1;
        this->Segment.StartPolling();
        this->IO.StartPolling();
        this->Convyer.StartPolling();
    }
}

void Master::StopPolling(){
    if(this->_isPolling){
        this->_isPolling = 0;
        this->Segment.StopPolling();
        this->IO.StopPolling();
        this->Convyer.StopPolling();
    }
}

void Master::Dispose(){
    this->_isDisposing = 1;
    this->Segment.Dispose();
    this->IO.Dispose();
    this->Capture.Dispose();
    this->Convyer.Dispose();
}