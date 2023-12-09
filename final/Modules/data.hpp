#ifndef DATA_H
#define DATA_H

typedef struct _Segment{
    unsigned char Seg_01;
    unsigned char Seg_02;
    unsigned char Seg_03;
    unsigned char Seg_04;
}Segment;

typedef struct _PinArray{
    unsigned char Pin_01: 1;
    unsigned char Pin_02: 1;
    unsigned char Pin_03: 1;
    unsigned char Pin_04: 1;
    unsigned char Pin_05: 1;
    unsigned char Pin_06: 1;
    unsigned char Pin_07: 1;
    unsigned char Pin_08: 1;
}PinArray;

#endif