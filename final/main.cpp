#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <thread>
#include "Modules/data.hpp"
#include "Modules/IOController.hpp"

using namespace std;

int main(void){
    IOController IO = IOController("InputController", "OutputController");

    printf("Input: %x\n", IO.Input);
    printf("Output: %x\n", IO.Output);
}