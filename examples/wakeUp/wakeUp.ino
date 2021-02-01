/**！
 * @file wakeUp.ino
 * @brief 使用睡眠唤醒功能
   @n 现象：使用此功能需要先让模块处于低功耗模式,此时的测量速率会很慢
   @n 当有设置好的中断事件产生,模块会进入正常模式,从而测量速率加快
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2021-01-16
 * @get from https://www.dfrobot.com
 * @https://github.com/DFRobot/DFRobot_H3LIS200DL
 */

#include <DFRobot_H3LIS200DL.h>

//当你使用I2C通信时,使用下面这段程序,使用DFRobot_H3LIS200DL_I2C构造对象
/*!
 * @brief Constructor 
 * @param pWire I2c controller
 * @param addr  I2C address(0x18/0x19)
 */
//DFRobot_H3LIS200DL_I2C acce/*(&Wire,0x19)*/;

//当你使用SPI通信时,使用下面这段程序,使用DFRobot_H3LIS200DL_SPI构造对象
#if defined(ESP32) || defined(ESP8266)
#define H3LIS200DL_CS  D3

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
void setup(void){

  Serial.begin(9600);
  //Chip initialization
  while(acce.begin()){
     delay(1000);
     Serial.println("init failure");
  }
  //Get chip id
  Serial.print("chip id : ");
  Serial.println(acce.getID(),HEX);
  
  /**
    set range:Range(g)
              e100_g ,/<±100g>/
              e200_g ,/<±200g>/
  */
  acce.setRange(/*Range = */DFRobot_H3LIS200DL::e100_g);

  /**
   “sleep to wake-up”  need to put the chip in low power mode first
   Set data measurement rate：
   
      ePowerDown_0HZ = 0,
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
  acce.setAcquireRate(/*Rate = */DFRobot_H3LIS200DL::eLowPower_halfHZ);
  
  /**
    Set the threshold of interrupt source 1 interrupt
    threshold:Threshold(g)
   */
  acce.setIntOneTh(/*Threshold = */6);
  //Enable sleep wake function
  acce.enableSleep(true);
  
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
    Serial.print("Acceleration x: "); //print acceleration
    Serial.print(acce.readAccX());
    Serial.print(" g \ty: ");
    Serial.print(acce.readAccY());
    Serial.print(" g \tz: ");
    Serial.print(acce.readAccZ());
    Serial.println(" g");
    delay(300);
}