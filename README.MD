# HX711-Arduino
The H3LIS200DL is a low-power high performance 3-axis linear accelerometer <br>
belonging to the “nano” family, with digital I2C/SPI <br>
serial interface standard output. <br>
The device features ultra-low-power operational <br>
modes that allow advanced power saving and <br>
smart sleep-to-wakeup functions.<br>
The H3LIS200DL has dynamically user selectable full scales of ±100g/±200g and is <br>
capable of measuring accelerations with output <br>
data rates from 0.5 Hz to 1 kHz.<br>
The H3LIS200DL is available in a small thin <br>
plastic land grid array package (LGA) and is <br>
guaranteed to operate over an extended <br>
temperature range from -40 °C to +85 °C.<br>


## DFRobot_H3LIS200DL Library for Arduino
---------------------------------------------------------

Provide an Arduino library to get weight by reading data from HX711.

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

Provide an Arduino library to get Three-axis acceleration by reading data from H3LIS200DL.

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
#include <DFRobot_H3LIS200DL.h>

  DFRobot_H3LIS200DL();
  /**
   * @brief Initialize the function
   * @return Return 0 indicates a successful initialization, while other values indicates failure and return to error code.
   */
  int begin(void);
 
  /**
   * @brief Get chip id
   * @return Returns the eight-digit serial number
   */
  uint8_t getID();
  
  /**
   * @brief Enable interrupt
   * @param source:Interrupt pin selection
   * @param event:Interrupt event selection
   */
  void enableInterruptEvent(eInterruptSource_t source, eInterruptEvent_t event);
  
  /**
   * @brief Set the measurement range
   * @param range:Range(g)
            eOnehundred =  ±100g
            eTwohundred = ±200g
   */
  void setRange(eRange_t range);
  
  /**
   * @brief Set data measurement rate
   * @param range:rate(g)
   */
  void setAcquireRate(ePowerMode_t rate);
  
  /**
   * @brief Set data filtering mode
   * @param mode:Four modes
                 eCutoffMode1 = 0,
                 eCutoffMode2,
                 eCutoffMode3,
                 eCutoffMode4,
                 eShutDown,
   */
  void setHFilterMode(eHighPassFilter_t mode);

  /**
   * @brief Set the threshold of interrupt source 1 interrupt
   * @param threshold:Threshold(g)
   */
  void setIntOneTh(uint8_t threshold);//0 - 100 / 0 - 200 

  /**
   * @brief Set interrupt source 2 interrupt generation threshold
   * @param threshold:Threshold(g)
   */
  void setIntTwoTh(uint8_t threshold);//0 - 100 / 0 - 200 

  /**
   * @brief Enable sleep wake function
   * @param enable:true\false
   * @return 0
   */
  int enableSleep(bool enable);
  
  /**
   * @brief Get the acceleration in the three directions of xyz
   * @return Three-axis acceleration 
             acceleration_x;
             acceleration_y;
             acceleration_z;
   */
  sAccel_t getAcceFromXYZ();

  /**
   * @brief Check whether the interrupt event'source' is generated in interrupt 1
   * @param source:中断事件
                   eXLowThanTh = 0,/<The acceleration in the x direction is less than the threshold>/
                   eXhigherThanTh ,/<The acceleration in the x direction is greater than the threshold>/
                   eYLowThanTh,/<The acceleration in the y direction is less than the threshold>/
                   eYhigherThanTh,/<The acceleration in the y direction is greater than the threshold>/
                   eZLowThanTh,/<The acceleration in the z direction is less than the threshold>/
                   eZhigherThanTh,/<The acceleration in the z direction is greater than the threshold>/
   * @return true ：产生
             false：未产生
   */
  bool getInt1Event(eInterruptEvent_t source);

  /**
   * @brief Check whether the interrupt event'source' is generated in interrupt 2
   * @param source:中断事件
                   eXLowThanTh = 0,/<The acceleration in the x direction is less than the threshold>/
                   eXhigherThanTh ,/<The acceleration in the x direction is greater than the threshold>/
                   eYLowThanTh,/<The acceleration in the y direction is less than the threshold>/
                   eYhigherThanTh,/<The acceleration in the y direction is greater than the threshold>/
                   eZLowThanTh,/<The acceleration in the z direction is less than the threshold>/
                   eZhigherThanTh,/<The acceleration in the z direction is greater than the threshold>/
   * @return true ：产生
             false：未产生
   */
  bool getInt2Event(eInterruptEvent_t source);


```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino uno        |      √       |              |             | 
FireBeetle-ESP8266        |      √       |              |             | 
FireBeetle-ESP32        |      √       |              |             | 
掌控板        |      √       |              |             | 
树莓派        |      √       |              |             | 
microbit        |      √       |              |             | 



## History

- data 2020-12-31
- version V1.0


## Credits

Written by(li.feng@dfrobot.com), 2020. (Welcome to our [website](https://www.dfrobot.com/))
