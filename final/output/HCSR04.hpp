#ifndef HCSR04_H
#define HCSR04_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <chrono>
#include <thread>
#include <pi-gpio.h>
#include "data.hpp"

#define ECHO 17
#define TRIGGER 18


using namespace std;
class Sonar
{
  public:
    Sonar(int trigger, int echo);
    void StartPolling();
    void StopPolling();
    void Dispose();
    int Valid;
    volatile double Distance;
  private:
    thread _poller;
    int _isDisposing;
    int _isPolling;
    void Polling();
    double distance(int timeout);
    void recordPulseLength();
    int trigger;
    int echo;
    volatile long startTimeUsec;
    volatile long endTimeUsec;
    double distanceMeters;
    long travelTimeUsec;
    long now;
};

#endif // HCSR04_H
