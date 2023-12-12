#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <thread>
#include "Modules/data.hpp"
#include "Modules/IOController.hpp"
#include "Modules/SegmentController.hpp"
#include "Modules/Camera.hpp"
#include "Modules/Master.hpp"

using namespace std;

int main(void){
    Master master = Master("/dev/InputController", "/dev/OutputController","/dev/svnSegment", "/dev/ConvyerController", 0);
    if(!master.Valid){
        printf("IO Controller: %d\n", master.IO.Valid);
        printf("Segment Controller: %d\n", master.Segment.Valid);
        printf("Camera Controller: %d\n", master.Capture.Valid);
        printf("Convyer Controller: %d\n", master.Convyer.Valid);
    }
    master.StartPolling();
    sleep(15);

    master.Dispose();
}
