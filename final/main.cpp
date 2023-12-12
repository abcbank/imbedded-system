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
    Master master = Master("/dev/InputController", "/dev/OutputController","/dev/svnSegment", 0);
    
    master.StartPolling();
    sleep(15);

    master.Dispose();
}
