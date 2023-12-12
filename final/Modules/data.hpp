#ifndef DATA_H
#define DATA_H

#include <iostream>

enum MasterStatus{
    READY = 0,
    RUN = 1,
    STOP = 2,
    RESET = 3
};

typedef union _PinArray
{
    unsigned char Pins;
    struct {
        unsigned char Pin_01: 1;
        unsigned char Pin_02: 1;
        unsigned char Pin_03: 1;
        unsigned char Pin_04: 1;
        unsigned char Pin_05: 1;
        unsigned char Pin_06: 1;
        unsigned char Pin_07: 1;
        unsigned char Pin_08: 1;
    };
}PinArray;

#endif