/*
  HCSR04 - Library for arduino, for HC-SR04 ultrasonic distance sensor.
  Created by Martin Sosic, June 11, 2016.
*/

#ifndef _MKL_US01_H_
#define _MKL_US01_H_

#include "Arduino.h"

const unsigned long DELAY_TIME_READ_ULTRASONIC = 50; // ms

class UltraSonicDistanceSensor {
 public:
    /**
     * @param triggerPin  Digital pin that is used for controlling sensor (output).
     * @param echoPin  Digital pin that is used to get information from sensor (input).
     */
    UltraSonicDistanceSensor(int triggerPin, int echoPin);

    /**
     * Measures distance by sending ultrasonic waves and measuring time it takes them
     * to return.
     * @returns Distance in centimeters, or negative value if distance is greater than 400cm.
     */
    double measureDistanceCm();
    bool available();
    double ML_dis;
    double getcm();
 private:
    int triggerPin, echoPin;
    double distanceCm;
    unsigned long lastTimeRead_ms;

};

#endif // _MKL_US01_H_
