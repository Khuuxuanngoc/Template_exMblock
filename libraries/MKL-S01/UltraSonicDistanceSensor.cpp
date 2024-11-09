/*
  HCSR04 - Library for arduino, for HC-SR04 ultrasonic distance sensor.
  Created by Martin Sosic, June 11, 2016.
*/

#include "Arduino.h"
#include "UltraSonicDistanceSensor.h"

UltraSonicDistanceSensor::UltraSonicDistanceSensor(
        int triggerPin, int echoPin) {
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

double UltraSonicDistanceSensor::measureDistanceCm() {
    // Make sure that trigger pin is LOW.
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance.
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    // Measure the length of echo signal, which is equal to the time needed for sound to go there and back.
    unsigned long durationMicroSec = pulseIn(echoPin, HIGH, 36000);
    distanceCm = durationMicroSec / 2.0 * 0.0343;
    ML_dis = durationMicroSec;
    if (distanceCm == 0 || distanceCm > 600) {
        return -1.0 ;
    } else {
        return distanceCm;
    }
}

bool UltraSonicDistanceSensor::available(){
    if(millis() - lastTimeRead_ms >= DELAY_TIME_READ_ULTRASONIC){
        lastTimeRead_ms = millis();
        // Make sure that trigger pin is LOW.
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        // Hold trigger for 10 microseconds, which is signal for sensor to measure distance.
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
        // Measure the length of echo signal, which is equal to the time needed for sound to go there and back.
        unsigned long durationMicroSec = pulseIn(echoPin, HIGH, 36000);
        distanceCm = durationMicroSec / 2.0 * 0.0343;
        ML_dis = durationMicroSec;
        if (distanceCm == 0 || distanceCm > 600) {
            distanceCm =  -1.0 ;
        }
        return true;
    }else return false;
}

double UltraSonicDistanceSensor::getcm(){
    return distanceCm;
}