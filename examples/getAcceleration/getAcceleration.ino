/**！
 * @file getAcceleration.ino
 * @brief Get the acceleration in x, y, z directions
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
DFRobot_H3LIS200DL_SPI acce(/*cs = */H3LIS200DL_CS);

//DFRobot_H3LIS200DL_I2C acce;
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
    delay(300);
}