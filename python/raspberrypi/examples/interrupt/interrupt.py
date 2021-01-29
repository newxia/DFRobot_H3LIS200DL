# -*- coding:utf-8 -*-
"""
   @file interrupt.py
   @brief Enable some interrupt events in the sensor, and get
   @n the interrupt signal through the interrupt pin
   @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @licence     The MIT License (MIT)
   @author [fengli](li.feng@dfrobot.com)
   @version  V1.0
   @date  2021-01-16
   @get from https://www.dfrobot.com
   @https://github.com/DFRobot/DFRobot_H3LIS200DL
"""

import threading

import sys
sys.path.append("../..") # set system path to top

from DFRobot_H3LIS200DL import *
import time
from gpio import GPIO
# peripheral params
RASPBERRY_PIN_CS = 27              #Chip selection pin
I2C_MODE         = 0x01            # default use I2C1
ADDRESS_0        = 0x19
INT1 = 26                           #Interrupt pin

intPadLock = threading.Lock() # intPad  threading lock
intPadAFlag = False # intPad  flag


def keyCallBack():
  global intPadLock, intPadAFlag
  intPadLock.acquire() # wait intPad  lock release
  intPadAFlag = True
  #print("-------------------------interrupt------------------------");
  intPadLock.release() 
intPad = GPIO(INT1, GPIO.IN) # set intPad to input
intPad.setInterrupt(GPIO.FALLING, keyCallBack) #set intPad interrupt callback

acce = DFRobot_H3LIS200DL_SPI(RASPBERRY_PIN_CS)
#acce = DFRobot_H3LIS200DL_I2C(I2C_MODE ,ADDRESS_0)
# clear screen
acce.begin()
print("chip id :")
print(acce.getID())
'''
    set range:Range(g)
              E_ONE_HUNDRED =0#/**< ±100g>*/
              E_TWO_HUNDRED = 1#/**< ±200g>*/
'''
acce.setRange(acce.E_ONE_HUNDRED)
'''
    Set data measurement rate
           E_POWER_DOWN 
           E_LOWPOWER_HALFHZ 
           E_LOWPOWER_1HZ 
           E_LOWPOWER_2HZ 
           E_LOWPOWER_5HZ 
           E_LOWPOWER_10HZ 
           E_NORMAL_50HZ 
           E_NORMAL_100HZ 
           E_NORMAL_400HZ 
           E_NORMAL_1000HZ 
'''
acce.setAcquireRate(acce.E_NORMAL_50HZ)

'''
    Set the threshold of interrupt source 1 interrupt
    threshold:Threshold(g)
'''
acce.setIntOneTh(5);#0 - 100 / 0 - 200 

'''
@brief Enable interrupt
@ source:Interrupt pin selection
         eINT1 = 0,/<int1 >/
         eINT2,/<int2>/
@param event:Interrupt event selection
              eXLowThanTh = 0,/<The acceleration in the x direction is less than the threshold>/
              eXhigherThanTh ,/<The acceleration in the x direction is greater than the threshold>/
              eYLowThanTh,/<The acceleration in the y direction is less than the threshold>/
              eYhigherThanTh,/<The acceleration in the y direction is greater than the threshold>/
              eZLowThanTh,/<The acceleration in the z direction is less than the threshold>/
              eZhigherThanTh,/<The acceleration in the z direction is greater than the threshold>/
'''
acce.enableInterruptEvent(acce.eINT1,acce.E_Y_HIGHERTHAN_TH)
time.sleep(1)

while True:
    
    if(intPadAFlag == True):
      #Check whether the interrupt event'source' is generated in interrupt 1
      if acce.getInt1Event(acce.E_Y_HIGHERTHAN_TH) == True:
         print("The acceleration in the y direction is greater than the threshold")
      
      if acce.getInt1Event(acce.E_Z_HIGHERTHAN_TH) == True:
        print("The acceleration in the z direction is greater than the threshold")
       
      if acce.getInt1Event(acce.E_X_HIGHERTHAN_TH) == True:
        print("The acceleration in the x direction is greater than the threshold")
      
      intPadAFlag = False
    #Get the acceleration in the three directions of xyz
    x,y,z = acce.readAcceFromXYZ()
    time.sleep(0.1)
    print("Acceleration [X = %.2f g,Y = %.2f g,Z = %.2f g]"%(x,y,z))