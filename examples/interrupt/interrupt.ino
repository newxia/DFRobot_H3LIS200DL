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
  // set range:Range(g)
  //         eOnehundred =  ±100g
  //         eTwohundred = ±200g
  acce.setRange(DFRobot_H3LIS200DL::eOnehundred);

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
  acce.setAcquireRate(DFRobot_H3LIS200DL::eNormal_50HZ);
  //External interrupt setting
  attachInterrupt(0,interEvent, CHANGE);

  /**
    Set the threshold of interrupt source 1 interrupt
    threshold:Threshold(g)
   */
  acce.setIntOneTh(10);//0 - 100 / 0 - 200 

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
  acce.enableInterruptEvent(DFRobot_H3LIS200DL::eINT1,DFRobot_H3LIS200DL::eZhigherThanTh);
  
  delay(1000);
}

void loop(void){
    //Get the acceleration in the three directions of xyz
    DFRobot_H3LIS200DL::sAccel_t accel = acce.getAcceFromXYZ();
    Serial.print("Acceleration x: "); //print acceleration
    Serial.print(accel.acceleration_x);
    Serial.print(" mg \ty: ");
    Serial.print(accel.acceleration_y);
    Serial.print(" mg \tz: ");
    Serial.print(accel.acceleration_z);
    Serial.println(" mg");
    delay(100);  
   //The interrupt flag is set
   if(Flag == 1){
      Serial.println("Flag = 1");
      //Check whether the interrupt event'source' is generated in interrupt 1
      if(acce.getInt1Event(DFRobot_H3LIS200DL::eYhigherThanTh)){
        Serial.println("YH");
	  }
     if(acce.getInt1Event(DFRobot_H3LIS200DL::eZhigherThanTh)){
       Serial.println("ZH");
	  }
      if(acce.getInt1Event(DFRobot_H3LIS200DL::eXhigherThanTh)){
        Serial.println("XH");
	  }
      Flag = 0;
   }
}