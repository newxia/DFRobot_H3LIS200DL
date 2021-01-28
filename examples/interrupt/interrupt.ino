/**！
 * @file interrupt.ino
 * @brief Enable some interrupt events in the sensor, and get
   @n the interrupt signal through the interrupt pin
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2021-01-16
 * @get from https://www.dfrobot.com
 * @https://github.com/DFRobot/DFRobot_H3LIS200DL
 */

#include <DFRobot_H3LIS200DL.h>



#if defined(ESP32) || defined(ESP8266)
#define H3LIS200DL_CS  D5

/* AVR series mainboard */
#else
#define H3LIS200DL_CS 3
#endif
/*!
 * @brief Constructor 
 * @param cs : Chip selection pinChip selection pin
 * @param spi :SPI controller
 */
//DFRobot_H3LIS200DL_SPI acce(/*cs = */H3LIS200DL_CS);
/*!
 * @brief Constructor 
 * @param pWire I2c controller
 * @param addr  I2C address(0x19/0x18)
 */
DFRobot_H3LIS200DL_I2C acce;
volatile int Flag = 0;
void interEvent(){
  Flag = 1;
}

void setup(void){

  Serial.begin(9600);
    //Chip initialization
  while(acce.begin()){
     Serial.println("init failure");
  }
  //Get chip id
  Serial.print("chip id : ");
  Serial.println(acce.getID(),HEX);
  
  /**
    set range:Range(g)
              eOnehundred ,/<±100g>/
              eTwohundred ,/<±200g>/
  */
  acce.setRange(/*Range = */DFRobot_H3LIS200DL::eOnehundred);

  /**
    Set data measurement rate：
      ePowerDown = 0,
      eLowPower_halfHZ,
      eLowPower_1HZ,
      eLowPower_2HZ,
      eLowPower_5HZ,
      eLowPower_10HZ,
      eNormal_50HZ,
      eNormal_100HZ,
      eNormal_400HZ,
      eNormal_1000HZ,
  */
  acce.setAcquireRate(/*Rate = */DFRobot_H3LIS200DL::eNormal_50HZ);
  #ifdef ARDUINO_ARCH_MPYTHON 
  /*                    The Correspondence Table of ESP32 Interrupt Pins And Terminal Numbers
   * -----------------------------------------------------------------------------------------------------
   * |            |  DigitalPin  | P0-P20 can be used as an external interrupt                           |
   * |    esp32   |--------------------------------------------------------------------------------------|
   * |            | Interrupt No |  DigitalPinToInterrupt (Pn) can be used to query the interrupt number |
   * |---------------------------------------------------------------------------------------------------|
   */
  attachInterrupt(digitalPinToInterrupt(P16)/*Query the interrupt number of the P16 pin*/,interEvent,CHANGE);
  //Open esp32's P16 pin for external interrupt, bilateral edge trigger, INT1/2 connected to P16
  #else
  /*    The Correspondence Table of AVR Series Arduino Interrupt Pins And Terminal Numbers
   * ---------------------------------------------------------------------------------------
   * |                                        |  DigitalPin  | 2  | 3  |                   |
   * |    Uno, Nano, Mini, other 328-based    |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  |                   |
   * |-------------------------------------------------------------------------------------|
   * |                                        |    Pin       | 2  | 3  | 21 | 20 | 19 | 18 |
   * |               Mega2560                 |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  | 5  |
   * |-------------------------------------------------------------------------------------|
   * |                                        |    Pin       | 3  | 2  | 0  | 1  | 7  |    |
   * |    Leonardo, other 32u4-based          |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  |    |
   * |--------------------------------------------------------------------------------------
   */
  /*                      The Correspondence Table of micro:bit Interrupt Pins And Terminal Numbers
   * ---------------------------------------------------------------------------------------------------------------------------------------------
   * |             micro:bit                       | DigitalPin |P0-P20 can be used as an external interrupt                                     |
   * |  (When using as an external interrupt,      |---------------------------------------------------------------------------------------------|
   * |no need to set it to input mode with pinMode)|Interrupt No|Interrupt number is a pin digital value, such as P0 interrupt number 0, P1 is 1 |
   * |-------------------------------------------------------------------------------------------------------------------------------------------|
   */
  attachInterrupt(/*Interrupt No*/0,interEvent,CHANGE);//Open the external interrupt 0, connect INT1/2 to the digital pin of the main control: 
     //UNO(2), Mega2560(2), Leonardo(3), microbit(P0).
  #endif

  /**
    Set the threshold of interrupt source 1 interrupt
    threshold:Threshold(g)
   */
  acce.setIntOneTh(/*Threshold = */10);//单位为:g

  /**
   * @brief Enable interrupt
   * @ source:Interrupt pin selection
              eINT1 = 0,/<int1 >/
              eINT2,/<int2>/
   * @param event:Interrupt event selection
                   eXLowThanTh = 0,/<The acceleration in the x direction is less than the threshold>/
                   eXhigherThanTh ,/<The acceleration in the x direction is greater than the threshold>/
                   eYLowThanTh,/<The acceleration in the y direction is less than the threshold>/
                   eYhigherThanTh,/<The acceleration in the y direction is greater than the threshold>/
                   eZLowThanTh,/<The acceleration in the z direction is less than the threshold>/
                   eZhigherThanTh,/<The acceleration in the z direction is greater than the threshold>/
   */
  acce.enableInterruptEvent(/*int pin*/DFRobot_H3LIS200DL::eINT1,/*interrupt = */DFRobot_H3LIS200DL::eZhigherThanTh);
  
  delay(1000);
}

void loop(void){
    //Get the acceleration in the three directions of xyz
    DFRobot_H3LIS200DL::sAccel_t accel = acce.getAcceFromXYZ();
    Serial.print("Acceleration x: "); //print acceleration
    Serial.print(accel.acceleration_x);
    Serial.print(" g \ty: ");
    Serial.print(accel.acceleration_y);
    Serial.print(" g \tz: ");
    Serial.print(accel.acceleration_z);
    Serial.println(" g");
    delay(100);  
   //The interrupt flag is set
   if(Flag == 1){
      Serial.println("Flag = 1");
      //Check whether the interrupt event'source' is generated in interrupt 1
      if(acce.getInt1Event(DFRobot_H3LIS200DL::eYhigherThanTh)){
        Serial.println("The acceleration in the y direction is greater than the threshold");
      }
     if(acce.getInt1Event(DFRobot_H3LIS200DL::eZhigherThanTh)){
       Serial.println("The acceleration in the z direction is greater than the threshold");
      }
      if(acce.getInt1Event(DFRobot_H3LIS200DL::eXhigherThanTh)){
        Serial.println("The acceleration in the x direction is greater than the threshold");
      }
      Flag = 0;
   }
}