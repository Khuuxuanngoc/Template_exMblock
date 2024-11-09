/**
 * File: Vietduino_Led.cpp
 *
 * Author: Ngoc Khuu
 * Contact: xuanngoc1992@gmail.com
 * Copyright: 2018 Ngoc Khuu
 * Copying permission statement:

    Vietduino_Led is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Vietduino_Led.h"
#include "Vietduino_Manager_Priority.h"

Vietduino_Led::Vietduino_Led() : Vietduino_Task(0, &(Vietduino_Led::stepState)){
    this->stop();
    this->_state = STATE_INIT_NONE;
}

Vietduino_Led::Vietduino_Led(byte outPin, byte _ledActive_) : Vietduino_Task(0, &(Vietduino_Led::stepState)){
    this->begin(outPin, _ledActive_);
}

void Vietduino_Led::begin(byte outPin, byte _ledActive_){
    this->init(outPin, _ledActive_, 0, 0, 0, 0);
    this->stop();
    this->_state = STATE_INIT_OK;
}

void Vietduino_Led::init(byte outPin, byte _onlevel_,unsigned long onUs, unsigned long offUs, byte count, unsigned long delayUs) {
  this->onUs = onUs;
  this->offUs = offUs;
  this->count = count;
  this->delayUs = delayUs;
  this->_bitMask = NK_PIN_TO_BITMASK(outPin);
  this->_portRegister = NK_PIN_TO_BASEREG(outPin);
  NK_DIRECT_MODE_OUTPUT(this->_portRegister,this->_bitMask);

  this->onLevel = _onlevel_;
}

void Vietduino_Led::start() {
  this->_state = STATE_WAIT;
  this->_counter = 0;
  this->setPeriodUs(0);
  this->_isrun = true;
  Vietduino_Manager_3.add(this);
}

void Vietduino_Led::stop() {
  this->turnOffLed();
  this->resetAllParam();
  this->_isrun = false;
  Vietduino_Manager_3.remove(this);
}

void Vietduino_Led::resetAllParam(){
  this->onUs = 0;
  this->offUs = 0;
  this->count = 0;
  this->delayUs = 0;
}

void Vietduino_Led::stepState(Vietduino_Task* task) {
  Vietduino_Led* bt = (Vietduino_Led*)task;
  if(bt->_state == STATE_OFF) {
    // Serial.println("STATE_WAIT");
    if((bt->count > 0) && (bt->_counter >= bt->count)) {
      // -- Count was defined, and we reached it.
      bt->_counter = 0;

      if(bt->delayUs > 0) {
        // -- delay was defined.
        bt->_state = STATE_WAIT;
        bt->setPeriodUs(bt->delayUs);
      } else {
        bt->stop();
      }
    }else{
      bt->setPeriodUs(0);
    }
    bt->_state = STATE_WAIT;

  }else if(bt->_state == STATE_ON) {
    // -- Turn off.
    // Serial.println("STATE_OFF");
    bt->turnOffLed();
    bt->_state = STATE_OFF;
    if(bt->offUs == 0){
        bt->stop();
    }else{
        bt->setPeriodUs(bt->offUs);
        bt->_counter += 1;
    }

  }
  else {
    // -- state == WAIT
    // -- Turn on.
    // Serial.println("STATE_ON");
    bt->turnOnLed();
    bt->_state = STATE_ON;
    bt->setPeriodUs(bt->onUs);
  }
}

bool Vietduino_Led::isRunning(){
    return this->_isrun;
}

void Vietduino_Led::turnOffLed(){
  if(this->onLevel != HIGH) {
      NK_DIRECT_WRITE_HIGH(this->_portRegister, this->_bitMask);
  } else {  
      NK_DIRECT_WRITE_LOW(this->_portRegister, this->_bitMask);
  }
}
    
void Vietduino_Led::turnOnLed(){
  if(this->onLevel != HIGH) {
      NK_DIRECT_WRITE_LOW(this->_portRegister, this->_bitMask);
  } else {  
      NK_DIRECT_WRITE_HIGH(this->_portRegister, this->_bitMask);
  }
}

void Vietduino_Led::write(unsigned long _ton_, unsigned long _toff_, byte _count_, unsigned long _delay_){ // unit ms.
    if( (this->onUs != (_ton_*1000))|| \
        (this->offUs != (1000* _toff_))|| \
        (this->count != (_count_))|| \
        (this->delayUs != (1000*_delay_))){
            this->stop();

            if(_ton_ != 0){
              this->onUs = VLED_CHECK_VALID_TIME(_ton_) * 1000;
              this->offUs = VLED_CHECK_VALID_TIME(_toff_) * 1000;
              this->count = _count_;
              this->delayUs = VLED_CHECK_VALID_TIME(_delay_) * 1000;
              this->start();
            }
    }
}
