#include "Master.hpp"

using namespace std;

void Run(IOController* ctrl, int Pin){
    Master *master = (Master*)ctrl->ParentAddr;
    if((master->Status == STOP | master->Status == READY)& master->Valid){
        master->SetStatus(RUN);
        master->SetSequenceStatus(WaitForDetect);
        // 1. Convyer Start
        // 2. if Sensor Enabled
        // 3. Convyer Stop
        // 4. Get Screenshot
        // 5. type detection by tensorflow model
        // 6. Control Robot
        // 7. go to 1
    }
}

void SensorEnabled(IOController* ctrl, int Pin){
    Master *master = (Master*)ctrl->ParentAddr;
    if(master->Status == RUN & master->SeqStatus == WaitForDetect & master->Valid){
        master->SetSequenceStatus(RUN_TF);
        // printf("%d", master->Capture.Capture());
        // prinf()
    }
}


void Stop(IOController* ctrl, int Pin){
    Master *master = (Master*)ctrl->ParentAddr;
    if(master->Status == RUN & master->Valid){
        master->SetStatus(STOP);
        master->SetSequenceStatus(SeqenceNONE);
        // 1. Convyer Stop
        // 2. Robot Stop
    }
}

void Reset(IOController* ctrl, int Pin){
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
    this->IsBusy = 0;

    this->Segment = SegmentController(SegDriver);
    this->IO = IOController(InputDriver, OutputDriver);
    this->Convyer = ConvyerController(ConvyerDriver);
    this->Capture = Camera(CamID);
    this->TF_lite = TFModel("ABC");
    // this->Sensor = Sonar(18,17);

    this->Segment.ParentAddr = this;
    this->IO.ParentAddr = this;
    this->Capture.ParentAddr = this;
    this->Convyer.ParentAddr = this;

    auto test = EnableTest;
    this->IO.InputEnabled[0].push_back((void (*)(IOController*, int))test);
    this->IO.InputEnabled[1].push_back((void (*)(IOController*, int))test);
    this->IO.InputEnabled[2].push_back((void (*)(IOController*, int))test);

    test = DisableTest;
    this->IO.InputDisabled[0].push_back((void (*)(IOController*, int))test);
    this->IO.InputDisabled[1].push_back((void (*)(IOController*, int))test);
    this->IO.InputDisabled[2].push_back((void (*)(IOController*, int))test);
    this->IO.InputDisabled[0].push_back((void (*)(IOController*, int))Run);
    this->IO.InputDisabled[1].push_back((void (*)(IOController*, int))Stop);
    //this->IO.InputDisabled[2].push_back((void (*)(IOController*, int))Reset);
    this->IO.InputDisabled[2].push_back((void (*)(IOController*, int))SensorEnabled);
    
    std::function<void()> temp;
    if(this->Segment.Valid 
    && this->IO.Valid 
    && this->Capture.Valid 
    && this->Convyer.Valid
    // && this->Sensor.Valid
    ){
        this->Valid = 1;
    }
    this->SetStatus(STOP);
}

void Master::SetSequenceStatus(SequenceStatus Status){
    if(!this->IsBusy && this->Valid)
    {
        this->SeqStatus = Status;
        if(this->proc.joinable())
            this->proc.join();
        switch(Status){
            case SeqenceNONE:
                this->proc = thread([this] {
                    this->IsBusy = true;
                    this->Convyer.Status = NONE;
                    this->IsBusy = false;
                });
            break;
            case WaitForDetect:
                this->proc = thread([this] {
                    this->IsBusy = true;
                    this->Convyer.Status = A;
                    this->IsBusy = false;
                });
            break;
            case RUN_TF:
                this->proc = thread([this] {
                    this->IsBusy = true;
                    this->Convyer.Status = NONE;
                    this->Capture.pause = 1;
                    this->TarFrame = this->Capture.frame.clone();
                    this->Capture.pause = 0;
                    this->IsBusy = false;
                    sleep(5);
                    this->SetSequenceStatus(ROBOT);
                });
            break;
            case ROBOT:
                this->proc = thread([this] {
                    this->IsBusy = true;
                    this->Convyer.Status = NONE;
                    this->IsBusy = false;
                });
            break;
        }
    }
}

void Master::SetStatus(MasterStatus Status){
    this->Status = Status;
    printf("%d", Status);
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
        this->Capture.StartPolling();
        this->Segment.StartPolling();
        this->IO.StartPolling();
        this->Convyer.StartPolling();
        // this->Sensor.StartPolling();
    }
}

void Master::StopPolling(){
    if(this->_isPolling){
        this->_isPolling = 0;
        this->Capture.StopPolling();
        this->Segment.StopPolling();
        this->IO.StopPolling();
        this->Convyer.StopPolling();
        // this->Sensor.StopPolling();
    }
}

void Master::Dispose(){
    this->_isDisposing = 1;
    this->Segment.Dispose();
    this->IO.Dispose();
    this->Capture.Dispose();
    this->Convyer.Dispose();
    this->Capture.Dispose();
    // this->Sensor.Dispose();
}