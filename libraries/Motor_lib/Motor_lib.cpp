#include "Motor_lib.h"

void Motor_lib::begin(int pwm_, int in1_, int in2_){
    this->in1 = in1_;
    this->in2 = in2_;
    this->pwm = pwm_;

    pinMode(this->in1, OUTPUT);
    pinMode(this->in2, OUTPUT);
    pinMode(this->pwm, OUTPUT);

}

void Motor_lib::forward(int iSpeed){
    digitalWrite(this->in1, 1);
    digitalWrite(this->in2, 0);
    analogWrite(this->pwm, iSpeed);
    this->resetTimeRun();
}

void Motor_lib::backward(int iSpeed){
    digitalWrite(this->in1, 0);
    digitalWrite(this->in2, 1);
    analogWrite(this->pwm, iSpeed);
    this->resetTimeRun();
}

void Motor_lib::stop(){
    digitalWrite(this->in1, 0);
    digitalWrite(this->in2, 0);
    analogWrite(this->pwm, 0);
}

void Motor_lib::setTimeRun(unsigned long ul_timerun_){
    this->timeRun = ul_timerun_;
}

void Motor_lib::checkStop(){
    if(this->timeRun == 0) return;
    if(millis() - this->lastTime >= this->timeRun){
        this->stop();
    }
}

void Motor_lib::resetTimeRun(){
    this->lastTime = millis();
}



void robot_lib::begin(Motor_lib* left_M_, Motor_lib* right_M_){
    this->motorLeft = left_M_;
    this->motorRight = right_M_;
}

void robot_lib::forward(int speedL_, int speedR_){
    this->motorLeft->forward(speedL_);
    this->motorRight->forward(speedR_);
}
void robot_lib::backward(int speedL_, int speedR_){
    this->motorLeft->backward(speedL_);
    this->motorRight->backward(speedR_);
}
void robot_lib::turnLeft(int speedL_, int speedR_){
    this->motorLeft->backward(speedL_);
    this->motorRight->forward(speedR_);
}
void robot_lib::turnRight(int speedL_, int speedR_){
    this->motorLeft->forward(speedL_);
    this->motorRight->backward(speedR_);
}

void robot_lib::checkStop(){
    this->motorLeft->checkStop();
    this->motorRight->checkStop();
}

void robot_lib::setTimeRun(unsigned long ul_timerun_){
    this->motorLeft->setTimeRun(ul_timerun_);
    this->motorRight->setTimeRun(ul_timerun_);
}