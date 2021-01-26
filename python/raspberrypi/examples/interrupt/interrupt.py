# -*- coding:utf-8 -*-

#include <DFRobot_H3LIS200DL.h>
import threading

import sys
sys.path.append("../..") # set system path to top

from DFRobot_H3LIS200DL import *
import time
from gpio import GPIO
# peripheral params
RASPBERRY_SPI_BUS = 0
RASPBERRY_PIN_CS = 27
I2C_MODE         = 0x01            # default use I2C1
ADDRESS_0        = 0x19
INT1 = 27

keyALock = threading.Lock() # key A threading lock
keyAFlag = False # key A flag


def keyCallBack():
  global keyALock, keyAFlag
  keyALock.acquire() # wait key A lock release
  keyAFlag = True
  print("-------------------------interrupt------------------------");
  keyALock.release() 
keyA = GPIO(INT1, GPIO.IN) # set key to input
keyA.setInterrupt(GPIO.FALLING, keyCallBack) # set key interrupt callback

#acce = DFRobot_H3LIS200DL_SPI(RASPBERRY_SPI_BUS,RASPBERRY_PIN_CS)
acce = DFRobot_H3LIS200DL_I2C(I2C_MODE ,ADDRESS_0)
# clear screen
acce.begin()
print("chip id :")
print(acce.getID())
#time.sleep(1)
acce.setRange(acce.E_ONE_HUNDRED)
acce.setAcquireRate(acce.E_NORMAL_50HZ)
#设置唤醒阈�?
acce.setIntOneTh(5);#0 - 100 / 0 - 200 
#进入睡眠状�?

#acce.enableSleep(True);
acce.enableInterruptEvent(acce.eINT1,acce.E_Y_HIGHERTHAN_TH)
time.sleep(1)

while True:
    #获取三个方向上的加速度数据
    if(keyAFlag == True):
      #Check whether the interrupt event'source' is generated in interrupt 1
      if acce.getInt1Event(acce.E_Y_HIGHERTHAN_TH) == True:
         print("The acceleration in the y direction is greater than the threshold")
      
      if acce.getInt1Event(acce.E_Z_HIGHERTHAN_TH) == True:
        print("The acceleration in the z direction is greater than the threshold")
       
      if acce.getInt1Event(acce.E_X_HIGHERTHAN_TH) == True:
        print("The acceleration in the x direction is greater than the threshold")
      
      keyAFlag = False
    
    x,y,z = acce.readAcceFromXYZ()
    time.sleep(0.1)
    print("Acceleration [X = %.2f mg,Y = %.2f mg,Z = %.2f mg]"%(x,y,z))