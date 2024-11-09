#ifndef _Motor_lib__H
#define _Motor_lib__H
#include "Arduino.h"

class Motor_lib{
    public:
    uint8_t in1;
    uint8_t in2;
    uint8_t pwm;
    unsigned long lastTime, timeRun;

    //motor(int);
    void begin(int pwm_, int in1_, int in2_);
    void forward(int iSpeed);
    void backward(int iSpeed);
    void stop();
    void setTimeRun(unsigned long ul_timerun_);
    void checkStop();
    void resetTimeRun();
    
};

class robot_lib{
    public:
    Motor_lib *motorLeft;
    Motor_lib *motorRight;
    void begin(Motor_lib* left_M_, Motor_lib* right_M_);
    void forward(int speedL_, int speedR_);
    void backward(int speedL_, int speedR_);
    void turnLeft(int speedL_, int speedR_);
    void turnRight(int speedL_, int speedR_);
    void checkStop();
    void setTimeRun(unsigned long ul_timerun_);
};

#endif