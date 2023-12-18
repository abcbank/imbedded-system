/*
  HCSR04 - Library for arduino, for HC-SR04 ultrasonic distance sensor.
  Created by Martin Sosic, June 11, 2016.
*/
#include "HCSR04.hpp"
Sonar::Sonar(){
    this->Valid = 0;
}
Sonar::Sonar(int triggerPin, int echoPin)
{
    setup();
    this->trigger = triggerPin;
    this->echo = echoPin;

    this->Distance = 0;
    this->_isPolling = 0;
    this->_isDisposing = 0;

    setup_gpio(triggerPin, OUTPUT, 0);
    setup_gpio(echoPin, INPUT, 0);
    output_gpio(this->trigger, LOW);
    this_thread::sleep_for(chrono::milliseconds(500));
    this->Valid = 1;
}
void Sonar::Polling(){
    while(!this->_isDisposing && this->_isPolling){
        this->Distance = this->distance(400);
        this_thread::sleep_for(chrono::milliseconds(10));
        
    }
}
void Sonar::StartPolling(){

    if(!this->_poller.joinable() && this->Valid){
        this->_isPolling = 1;
        this->_poller = thread([this] { this->Polling(); });
    }
}
void Sonar::StopPolling(){

    if(this->_poller.joinable() && this->Valid){
        this->_isPolling = 0;
        this->_poller.join();
    }
}
void Sonar::Dispose(){
    this->_isDisposing = 1;
    this->StopPolling();
    cleanup();
}

double Sonar::distance(int timeout) {
    unsigned long maxDistanceDurationMicroSec;

    // Make sure that trigger pin is LOW.
    output_gpio(this->trigger, LOW);
    this_thread::sleep_for(chrono::milliseconds(10));
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance.
    output_gpio(this->trigger, HIGH);
    this_thread::sleep_for(chrono::milliseconds(10));
    output_gpio(this->trigger, LOW);

    this->startTimeUsec=micros();
    // while(input_gpio(this->echoPin) == LOW){
    //     now=micros();
    //     end=micros();
    // }
    // while (input_gpio(this->echoPin) == HIGH);

    while (input_gpio(this->echo) == LOW && micros()-now<timeout);
        recordPulseLength();
    this->endTimeUsec=micros();

    auto travelTimeUsec = this->endTimeUsec - this->startTimeUsec;
    printf("%d\n", this->endTimeUsec - this->startTimeUsec);
    return 100*(((float)travelTimeUsec/1000000.0)*340.29)/2;
}
void Sonar::recordPulseLength()
{
    startTimeUsec = micros();
    while ( input_gpio(echo) == HIGH );
    endTimeUsec = micros();
}
