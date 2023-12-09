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

using namespace std;

int main(void){
    IOController IO = IOController("/dev/InputController", "/dev/OutputController");
    SegmentController Segment = SegmentController("/dev/SegmentController");
    IO.StartPolling();

    IO.Output.Pin_01 = 1;
    IO.Output.Pin_02 = 0;
    IO.Output.Pin_03 = 0;
    sleep(3);

    IO.Output.Pin_01 = 0;
    IO.Output.Pin_02 = 1;
    IO.Output.Pin_03 = 0;
    sleep(3);

    IO.Output.Pin_01 = 0;
    IO.Output.Pin_02 = 0;
    IO.Output.Pin_03 = 1;
    sleep(3);


    IO.Dispose();
}
