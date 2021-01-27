# -*- coding:utf-8 -*-
"""
   @file getAcceleration.py
   @brief Get the acceleration in x, y, z directions
   @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @licence     The MIT License (MIT)
   @author [fengli](li.feng@dfrobot.com)
   @version  V1.0
   @date  2021-01-16
   @get from https://www.dfrobot.com
   @https://github.com/DFRobot/DFRobot_H3LIS200DL
"""

import sys
sys.path.append("../..") # set system path to top

from DFRobot_H3LIS200DL import *
import time

# peripheral params
RASPBERRY_PIN_CS = 27              #Chip selection pin
I2C_MODE         = 0x01            # default use I2C1
ADDRESS_0        = 0x19

#acce = DFRobot_H3LIS200DL_SPI(RASPBERRY_PIN_CS)
#
acce = DFRobot_H3LIS200DL_I2C(I2C_MODE ,ADDRESS_0)
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
    Set data measurement rate�?
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
time.sleep(0.1)

while True:
    #Get the acceleration in the three directions of xyz
    x,y,z = acce.readAcceFromXYZ()
    time.sleep(1)
    
    
    print("Acceleration [X = %.2d g,Y = %.2d g,Z = %.2d g]"%(x,y,z))
