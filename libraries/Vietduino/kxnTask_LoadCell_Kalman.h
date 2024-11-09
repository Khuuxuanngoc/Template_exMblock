#ifndef kxnTask_LoadCell_Kalman__h
#define kxnTask_LoadCell_Kalman__h

#define kxnTask_LoadCell_Kalman__h_4_y241031

#include "kxnTask.h"
#include "kxnTask_test.h"

#include "Adafruit_HX711.h"
#include "SimpleKalmanFilter.h"
#include <EEPROM.h>

// #define EN_kxnTask_LoadCell_DB

#ifndef DEBUG_PRINTER
#define DEBUG_PRINTER \
  Serial /**< Define where debug output will be printed.                       \
            */
#endif

#ifdef EN_kxnTask_LoadCell_DB
#define kxnTask_LoadCell_DB(...) \
  { \
    DEBUG_PRINTER.print(__VA_ARGS__); \
    DEBUG_PRINTER.println(__LINE__); \
  }

#else
#define kxnTask_LoadCell_DB(...) \
  {}
#endif

#define KXNTASK_LOADCEL_SCALE_100G_DEFAULT_Loadcell_1KG (874.59 / 1000000)
// #define KXNTASK_LOADCEL_SCALE_100G_DEFAULT_Loadcell_5KG  (2290.06/1000000)
#define KXNTASK_LOADCEL_SCALE_100G_DEFAULT_Loadcell_5KG (2324.01 / 1000000)

typedef enum {
  KXNTASK_LOADCEL_TYPE_1KG = 1,
  KXNTASK_LOADCEL_TYPE_5KG = 5
};

typedef struct {
  float factoryScale;
  uint8_t loadCell_Type;
  float UserScale_1KG;
  float UserScale_5KG;
  uint16_t isNew;
} kxnTask_LoadCell_Kalman__h_4_y241031_eeprom;

DEFINE_TASK_STATE(kxnTask_LoadCell){
  kxnTask_LoadCell_ON,
  kxnTask_LoadCell_Tare,
  kxnTask_LoadCell_Tare_Count
};

CREATE_TASK(kxnTask_LoadCell)
unsigned long timeOn;
float scaleValue;
uint8_t numTimeCalib, numTimeCalibCount;
Adafruit_HX711 *hx711;

SimpleKalmanFilter *simpleKalmanFilter;

int32_t gramDataA;
int32_t drawDataA;
uint16_t eeAddress;


kxnTask_LoadCell_Kalman__h_4_y241031_eeprom dataEeprom_loadCell;

void setup(uint8_t paPinSCK, uint8_t paPinDT, uint16_t addressData = 900) {
  this->eeAddress = addressData;
  // Initialize the HX711
  if (this->hx711) delete this->hx711;

  this->hx711 = new Adafruit_HX711(paPinDT, paPinSCK);

  if (!this->simpleKalmanFilter) this->simpleKalmanFilter = new SimpleKalmanFilter(200, 200, 0.2);

  hx711->begin();

  // read and toss 3 values each
  kxnTask_LoadCell_DB("Taring....\n");
  // hx711->readChannelRaw(CHAN_A_GAIN_128);
  // hx711->readChannelRaw(CHAN_A_GAIN_128);
  // for (uint8_t t = 0; t < 10; t++) {
  //   float tempValue = simpleKalmanFilter->updateEstimate(hx711->readChannelRaw(CHAN_A_GAIN_128));


  //   hx711->tareA(tempValue);

  //   // hx711->tareA(hx711->readChannelRaw(CHAN_A_GAIN_128));
  //   // hx711->tareA(hx711->readChannelRaw(CHAN_A_GAIN_128));
  // }
  this->numTimeCalib = 10;  // Init calib
  // this->scaleValue = 1;
  this->scaleValue = KXNTASK_LOADCEL_SCALE_100G_DEFAULT_Loadcell_1KG;
  // this->scaleValue = KXNTASK_LOADCEL_SCALE_100G_DEFAULT_Loadcell_5KG;

  kxnTask_LoadCell_DB(F("Read EEP....\n"));
  this->readEeprom();
  kxnTask_LoadCell_DB(F("Read EEP Done....\n"));
  kxnTask_LoadCell_DB(String() + F("Read ScaleValue: ") + this->scaleValue * 1000000 + "\n");

  start();
}

void setup() {
  this->setup(2, 3);
}

void loop() {
  switch (getState()) {
    case kxnTask_LoadCell_Tare:
      hx711->readChannelRaw(CHAN_A_GAIN_128);
      hx711->readChannelRaw(CHAN_A_GAIN_128);
      this->numTimeCalibCount = 0;

      setState(kxnTask_LoadCell_Tare_Count);
      kDelay(100);
      break;


    case kxnTask_LoadCell_Tare_Count:
      if ((this->numTimeCalib - this->numTimeCalibCount) > 0) {
        this->numTimeCalibCount++;
        float tempValue = simpleKalmanFilter->updateEstimate(hx711->readChannelRaw(CHAN_A_GAIN_128));
        hx711->tareA(tempValue);
        kDelay(1);
      } else {
        setState(kxnTask_LoadCell_ON);
        kDelay(0);
      }

      // for (uint8_t t = 0; t < 10; t++) {
      //   float tempValue = simpleKalmanFilter->updateEstimate(hx711->readChannelRaw(CHAN_A_GAIN_128));
      //   hx711->tareA(tempValue);
      // }
      // setState(kxnTask_LoadCell_ON);
      break;


    case kxnTask_LoadCell_ON:

      // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
      // since the read is blocking this will not be more than 10 or 80 SPS (L or H switch)
      drawDataA = hx711->readChannelBlocking(CHAN_A_GAIN_128) + 0.5;
      float tempDrawDataA_Filter = simpleKalmanFilter->updateEstimate(drawDataA);
      gramDataA = tempDrawDataA_Filter;  // / 100;
      // kxnTask_LoadCell_DB("Channel A: ");
      // kxnTask_LoadCell_DB(String() + gramDataA + " gr\t");
      // kxnTask_LoadCell_DB(String() + drawDataA + " unit\n");

      // // Read from Channel A with Gain 128, can also try CHAN_A_GAIN_64 or CHAN_B_GAIN_32
      // int32_t weightB32 = hx711->readChannelBlocking(CHAN_B_GAIN_32);
      // Serial.print("Channel B (Gain 32): ");
      // Serial.println(weightB32);
      kDelay(100);
      break;
  }
}

void start() {
  kxnTaskManager.add(this);
  setState(kxnTask_LoadCell_Tare);
}

void stop() {
  setStateIdle();
}

float getGram() {
  return abs(this->gramDataA * this->scaleValue);
}

int32_t getDrawValue() {
  return this->drawDataA;
}

float setRealValue(float pa_realValue) {
  this->scaleValue = pa_realValue / this->gramDataA;
  float tempFVal = pa_realValue * 1000000 / this->gramDataA;
  kxnTask_LoadCell_DB(String("Set Scale: ") + this->scaleValue + "x1000000: " + tempFVal + "\n");
  this->setScaleToEeprom();
  return this->scaleValue;
}

void tare() {
  setState(kxnTask_LoadCell_Tare);
  // delay(2000);
}

void runFast() {  // this function will be called without delay
}

void setEepromDefault(uint8_t pa_typeLoad) {
  dataEeprom_loadCell.isNew = this->eeAddress;
  dataEeprom_loadCell.loadCell_Type = pa_typeLoad;
  dataEeprom_loadCell.UserScale_1KG = KXNTASK_LOADCEL_SCALE_100G_DEFAULT_Loadcell_1KG;
  dataEeprom_loadCell.UserScale_5KG = KXNTASK_LOADCEL_SCALE_100G_DEFAULT_Loadcell_5KG;
  // if(pa_typeLoad == KXNTASK_LOADCEL_TYPE_1KG){
  //   dataEeprom_loadCell.UserScale_1KG = KXNTASK_LOADCEL_SCALE_100G_DEFAULT_Loadcell_1KG;
  // }else if(pa_typeLoad == KXNTASK_LOADCEL_TYPE_5KG){
  //   dataEeprom_loadCell.UserScale_5KG = KXNTASK_LOADCEL_SCALE_100G_DEFAULT_Loadcell_5KG;
  // }
}

void expDataFromEEP(uint8_t paType){
  if (paType == KXNTASK_LOADCEL_TYPE_1KG) {
    this->scaleValue = dataEeprom_loadCell.UserScale_1KG;
    dataEeprom_loadCell.loadCell_Type = KXNTASK_LOADCEL_TYPE_1KG;
    kxnTask_LoadCell_DB(F("setTypeLoadCell 1Kg !\t"));
  } else {
    this->scaleValue = dataEeprom_loadCell.UserScale_5KG;
    dataEeprom_loadCell.loadCell_Type = KXNTASK_LOADCEL_TYPE_5KG;
    kxnTask_LoadCell_DB(F("setTypeLoadCell 5Kg !\t"));
  }
  kxnTask_LoadCell_DB(String() + F("setTypeLoadCell paType\t") + paType + "\n");
}

void setTypeLoadCell(uint8_t paType) {
  if (paType == dataEeprom_loadCell.loadCell_Type) {
    return;
  }
  this->expDataFromEEP(paType);
  // if (paType == KXNTASK_LOADCEL_TYPE_1KG) {
  //   this->scaleValue = dataEeprom_loadCell.UserScale_1KG;
  //   dataEeprom_loadCell.loadCell_Type = KXNTASK_LOADCEL_TYPE_1KG;
  //   kxnTask_LoadCell_DB(F("setTypeLoadCell 1Kg !\t"));
  // } else {
  //   this->scaleValue = dataEeprom_loadCell.UserScale_5KG;
  //   dataEeprom_loadCell.loadCell_Type = KXNTASK_LOADCEL_TYPE_5KG;
  //   kxnTask_LoadCell_DB(F("setTypeLoadCell 5Kg !\t"));
  // }
  // kxnTask_LoadCell_DB(String() + F("setTypeLoadCell paType\t") + paType + "\n");
  saveEeprom();
}

void setScaleToEeprom() {
  if (this->scaleValue > 1) {
    setTypeLoadCell(dataEeprom_loadCell.loadCell_Type);
    return;
  } else {
    if (dataEeprom_loadCell.loadCell_Type == KXNTASK_LOADCEL_TYPE_1KG) {
      dataEeprom_loadCell.UserScale_1KG = this->scaleValue;
    } else if (dataEeprom_loadCell.loadCell_Type == KXNTASK_LOADCEL_TYPE_5KG) {
      dataEeprom_loadCell.UserScale_5KG = this->scaleValue;
    }
    kxnTask_LoadCell_DB(F("setScaleToEeprom!\t"));
  }

  saveEeprom();
}

void saveEeprom() {
  EEPROM.put(this->eeAddress, this->dataEeprom_loadCell);
  kxnTask_LoadCell_DB(F("EEprom saved!"));
}

void readEeprom() {
  EEPROM.get(this->eeAddress, this->dataEeprom_loadCell);
  kxnTask_LoadCell_DB(String() + F("Get EEprom ! \n") + this->dataEeprom_loadCell.isNew);
  if (this->dataEeprom_loadCell.isNew != this->eeAddress) {
    // first save Eeprom
    this->setEepromDefault(KXNTASK_LOADCEL_TYPE_1KG);
    this->saveEeprom();
    kxnTask_LoadCell_DB(F("EEprom Load Default!"));
    // delay(4000);
  } else {
    kxnTask_LoadCell_DB(String() + "type" + this->dataEeprom_loadCell.loadCell_Type + "\n");
    this->expDataFromEEP(this->dataEeprom_loadCell.loadCell_Type);
    kxnTask_LoadCell_DB(F("EEprom Load OK!\n"));
  }
}

void resetAll(){
  this->setEepromDefault(this->dataEeprom_loadCell.loadCell_Type);
  this->saveEeprom();
}

END

#endif