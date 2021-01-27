# -*- coding: utf-8 -*
""" 
  @file DFRobot_H3LIS200DL.py
  @brief Define the basic structure of class DFRobot_H3LIS200DL 
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @licence     The MIT License (MIT)
  @author [fengli](li.feng@dfrobot.com)
  @version  V1.0
  @date  2020-12-23
  @get from https://www.dfrobot.com
  @url https://github.com/DFRobot/DFRobot_H3LIS200DL
"""
import struct
import serial
import time
import smbus
import spidev
from gpio import GPIO
import numpy as np

I2C_MODE                  = 0x01
SPI_MODE                 = 0x02
class SPI:

  MODE_1 = 1
  MODE_2 = 2
  MODE_3 = 3
  MODE_4 = 4
  def __init__(self, bus, dev, speed = 100000, mode = MODE_4):
    self._bus = spidev.SpiDev()
    self._bus.open(0, 0)
    self._bus.no_cs = True
    self._bus.max_speed_hz = speed
    #self._bus.threewire  = True
  def transfer(self, buf):
    if len(buf):
      return self._bus.writebytes(buf)
    return []
  def readData(self, cmd):
    return self._bus.readbytes(cmd)
  
class DFRobot_H3LIS200DL(object):


  H3LIS200DL_I2C_ADDR = 0x19       #
  H3LIS200DL_REG_CARD_ID = 0x0F     #/*Chip id*/
  H3LIS200DL_REG_CTRL_REG1 = 0x20     #/*Control register 1*/
  H3LIS200DL_REG_CTRL_REG4 = 0x23     #/*Control register 4*/
  H3LIS200DL_REG_CTRL_REG2 = 0x21     #/*Control register 2*/
  H3LIS200DL_REG_CTRL_REG3 = 0x22     #/*Control register 3*/
  H3LIS200DL_REG_CTRL_REG5 = 0x24     #/*Control register 5*/
  H3LIS200DL_REG_CTRL_REG6 = 0x25     #/*Control register 6*/
  H3LIS200DL_REG_STATUS_REG = 0x27     #/*Status register*/
  H3LIS200DL_REG_OUT_X = 0x29     #/*Acceleration register*/
  H3LIS200DL_REG_OUT_Y = 0x2B     #/*Acceleration register*/
  H3LIS200DL_REG_OUT_Z = 0x2D     # /*Acceleration register*/
  H3LIS200DL_REG_INT1_THS = 0x32     #/*Interrupt source 1 threshold*/
  H3LIS200DL_REG_INT2_THS = 0x36     #/*Interrupt source 2 threshold*/
  H3LIS200DL_REG_INT1_CFG = 0x30     #/*Interrupt source 1 configuration register*/
  H3LIS200DL_REG_INT2_CFG = 0x34     #/*Interrupt source 2 configuration register*/
  H3LIS200DL_REG_INT1_SRC = 0x31     #/*Interrupt source 1 status register*/
  H3LIS200DL_REG_INT2_SRC = 0x35     #/*Interrupt source 2 status register*/
  __m_flag   = 0                # mode flag
  __count    = 0                # acquisition count    
  __txbuf        = [0]          # i2c send buffer
  __uart_i2c     =  0
  __range = 100
  __reset = 0
  '''
    Power mode selection, determine the frequency of data collection
    Represents the number of data collected per second
  '''
  E_POWER_DOWN = 0
  E_LOWPOWER_HALFHZ = 1 
  E_LOWPOWER_1HZ = 2
  E_LOWPOWER_2HZ = 3
  E_LOWPOWER_5HZ = 4
  E_LOWPOWER_10HZ = 5 
  E_NORMAL_50HZ = 6
  E_NORMAL_100HZ = 7
  E_NORMAL_400HZ = 8
  E_NORMAL_1000HZ = 9

#Sensor range selection
  E_ONE_HUNDRED =0#/**< ±100g>*/
  E_TWO_HUNDRED = 1#/**< ±200g>*/

  '''
       High-pass filter cut-off frequency configuration
  ---------------------------------------------------------------------------------------
  |-------------------------------------------------------------|
  |                |    ft [Hz]      |        ft [Hz]       |       ft [Hz]        |        ft [Hz]        |
  |   mode         |Data rate = 50 Hz|   Data rate = 100 Hz |  Data rate = 400 Hz  |   Data rate = 1000 Hz |
  |--------------------------------------------------------------------------------------------------------|
  |  eCutoffMode1  |     1           |         2            |            8         |             20        |
  |--------------------------------------------------------------------------------------------------------|
  |  eCutoffMode1  |    0.5          |         1            |            4         |             10        |
  |--------------------------------------------------------------------------------------------------------|
  |  eCutoffMode1  |    0.25         |         0.5          |            2         |             5         |
  |--------------------------------------------------------------------------------------------------------|
  |  eCutoffMode1  |    0.125        |         0.25         |            1         |             2.5       |
  |--------------------------------------------------------------------------------------------------------|
  |--------------------------------------------------------------------------------------------------------|
  '''
  E_CUTOFF_MODE1 = 0
  E_CUTOFF_MODE2 = 1
  E_CUTOFF_MODE3 = 2
  E_CUTOFF_MODE4 = 3
  E_SHUTDOWN = 4
  
#Interrupt event
  E_X_LOWTHAN_TH = 0#/**<The acceleration in the x direction is less than the threshold>*/
  E_X_HIGHERTHAN_TH  = 1#/**<The acceleration in the x direction is greater than the threshold>*/
  E_Y_LOWTHAN_TH = 2#/**<The acceleration in the y direction is less than the threshold>*/
  E_Y_HIGHERTHAN_TH = 3#/**<The acceleration in the y direction is greater than the threshold>*/
  E_Z_LOWTHAN_TH = 4#/**<The acceleration in the z direction is less than the threshold>*/
  E_Z_HIGHERTHAN_TH = 5#/**<The acceleration in the z direction is greater than the threshold>*/
  E_EVENT_ERROR = 6#/**< No event>*/

#Interrupt pin selection
  eINT1 = 0,#/**<int1 >*/
  eINT2 = 1,#/**<int2 >*/
  
  ERROR                     = -1
  def __init__(self ,bus ,Baud):
    __reset = 1
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self.__uart_i2c = I2C_MODE;
    else:
      self.__uart_i2c = SPI_MODE



  '''
    @brief Initialize the function
    @return Return 0 indicates a successful initialization, while other values indicates failure and return to error code.
  '''
  def begin(self):
    identifier = 0 
    if(self.__uart_i2c == SPI_MODE):
      identifier = self.read_reg(self.H3LIS200DL_REG_CARD_ID + 0x80)  
    else:
      identifier = self.read_reg(self.H3LIS200DL_REG_CARD_ID)
    #print(identifier)
    if identifier == 0x32:
      #print("identifier = :")
      #print(identifier)
      return 0
    else:
      return 1
      
  '''
    @brief Get chip id
    @return Returns the eight-digit serial number
  '''
  def getID(self):
    identifier = 0 
    if(self.__uart_i2c == SPI_MODE):
      identifier = self.read_reg(self.H3LIS200DL_REG_CARD_ID + 0x80)  
    else:
      identifier = self.read_reg(self.H3LIS200DL_REG_CARD_ID)
    return identifier

  '''
    @brief Set the measurement range
    @param range:Range(g)
           eOnehundred =  ±100g
           eTwohundred = ±200g
  '''
  def setRange(self,range_r):
    regester = self.H3LIS200DL_REG_CTRL_REG4;
    if(self.__uart_i2c == SPI_MODE):
      regester  = self.H3LIS200DL_REG_CTRL_REG4 | 0x80;
    reg = self.read_reg(regester)
    if range_r == self.E_ONE_HUNDRED:
     reg = reg & (~0x10);
     self.__range = 100;
    else:
     reg = reg | 0x10;
     self.__range = 200;
    #print(reg)
    self.write_reg(self.H3LIS200DL_REG_CTRL_REG4,reg)

  
  '''
    @brief Set data measurement rate
    @param range:rate(g)
  '''
  def setAcquireRate(self, rate):
    regester = self.H3LIS200DL_REG_CTRL_REG1;
    if(self.__uart_i2c == SPI_MODE):
      regester  = self.H3LIS200DL_REG_CTRL_REG1 | 0x80;
    
    reg = self.read_reg(regester)
    #print(reg);
    if rate == self.E_POWER_DOWN:
      reg = reg & (~(0x7 << 5))
    elif rate == self.E_LOWPOWER_HALFHZ:
      reg = reg & (~(0x7 << 5))
      reg = reg | (0x02 << 5)
    elif rate == self.E_LOWPOWER_1HZ:
      reg = reg & (~(0x7 << 5))
      reg = reg | (0x03 << 5)
    elif rate == self.E_LOWPOWER_2HZ:
      reg = reg & (~(0x7 << 5))
      reg = reg | (0x04 << 5)
    elif rate == self.E_LOWPOWER_5HZ:
      reg = reg & (~(0x7 << 5))
      reg = reg | (0x05 << 5)
    elif rate == self.E_LOWPOWER_10HZ:
      reg = reg & (~(0x7 << 5))
      reg = reg | (0x06 << 5)
    elif rate == self.E_NORMAL_50HZ:
      reg = reg & (~(0x7 << 5))
      reg = reg | (0x01 << 5)
      reg = reg & (~(0x3 << 3))
    elif rate == self.E_NORMAL_100HZ:
      reg = reg & (~(0x7 << 5))
      reg = reg | (0x01 << 5)
      reg = reg & (~(0x3 << 3))
      reg = reg | (0x01 << 3)
    elif rate == self.E_NORMAL_400HZ:
      reg = reg & (~(0x7 << 5))
      reg = reg | (0x01 << 5)
      reg = reg & (~(0x3 << 3))
      reg = reg | (0x02 << 3)
    elif rate == self.E_NORMAL_1000HZ:
      reg = reg & (~(0x7 << 5))
      reg = reg | (0x01 << 5)
      reg = reg & (~(0x3 << 3))
      reg = reg | (0x03 << 3)
    #print(reg);
    self.write_reg(self.H3LIS200DL_REG_CTRL_REG1,reg);

  '''
    @brief Set the threshold of interrupt source 1 interrupt
    @param threshold:Threshold(g)
  '''
  def setIntOneTh(self,threshold):
    reg = (threshold * 128)/self.__range
    #print(reg)
    self.write_reg(self.H3LIS200DL_REG_INT1_THS,reg)


  '''
    @brief Set interrupt source 2 interrupt generation threshold
    @param threshold:Threshold(g)
  '''
  def setIntTwoTh(self,threshold):
    reg = (threshold * 128)/self.__range
    #print(reg)
    self.write_reg(self.H3LIS200DL_REG_INT2_THS,reg)
  
  '''
    @brief Enable interrupt
    @param source:Interrupt pin selection
    @param event:Interrupt event selection
  '''
  def enableInterruptEvent(self,source,event):
    reg = 0
    regester1 = self.H3LIS200DL_REG_INT1_CFG;
    regester2 = self.H3LIS200DL_REG_INT2_CFG;
    if(self.__uart_i2c == SPI_MODE):
      regester1 = self.H3LIS200DL_REG_INT1_CFG | 0x80;
      regester2 = self.H3LIS200DL_REG_INT2_CFG | 0x80;
    
    if source == self.eINT1:
      reg = self.read_reg(regester1)
    else:
      reg = self.read_reg(regester2)
    if self.__reset == 1:
       reg = 0
       self.__reset = 0
    if event == self.E_X_LOWTHAN_TH:
      reg = reg | 0x01
    elif event == self.E_X_HIGHERTHAN_TH:
      reg = reg | 0x02
    elif event == self.E_Y_LOWTHAN_TH:
      reg = reg | 0x04
    elif event == self.E_Y_HIGHERTHAN_TH:
      reg = reg | 0x08
    elif event == self.E_Z_LOWTHAN_TH:
      reg = reg | 0x10      
    elif event == self.E_Z_HIGHERTHAN_TH:
      reg = reg | 0x20
      
    #print(reg)
    if source == self.eINT1:
      self.write_reg(self.H3LIS200DL_REG_INT1_CFG,reg)
    else:
      self.write_reg(self.H3LIS200DL_REG_INT2_CFG,reg)

  '''
    @brief Check whether the interrupt event'source' is generated in interrupt 1
    @param source:Interrupt event
                  eXLowThanTh = 0,/<The acceleration in the x direction is less than the threshold>/
                  eXhigherThanTh ,/<The acceleration in the x direction is greater than the threshold>/
                  eYLowThanTh,/<The acceleration in the y direction is less than the threshold>/
                  eYhigherThanTh,/<The acceleration in the y direction is greater than the threshold>/
                  eZLowThanTh,/<The acceleration in the z direction is less than the threshold>/
                  eZhigherThanTh,/<The acceleration in the z direction is greater than the threshold>/
    @return true ：produce
            false：Interrupt event
  '''
  def getInt1Event(self,source):
    regester = self.H3LIS200DL_REG_INT1_SRC;
    if(self.__uart_i2c == SPI_MODE):
      regester  = self.H3LIS200DL_REG_INT1_SRC | 0x80;
    reg = self.read_reg(regester)
      #print(reg & (1 << source))
      #print(1 << source)
    if (reg & (1 << source)) >= 1:
    #     print("true")
         return True
    else:
     #    print("false")
         return False
         
  '''
    @brief Check whether the interrupt event'source' is generated in interrupt 2
    @param source:Interrupt event
                  eXLowThanTh = 0,/<The acceleration in the x direction is less than the threshold>/
                  eXhigherThanTh ,/<The acceleration in the x direction is greater than the threshold>/
                  eYLowThanTh,/<The acceleration in the y direction is less than the threshold>/
                  eYhigherThanTh,/<The acceleration in the y direction is greater than the threshold>/
                  eZLowThanTh,/<The acceleration in the z direction is less than the threshold>/
                  eZhigherThanTh,/<The acceleration in the z direction is greater than the threshold>/
    @return true ：produce
            false：Interrupt event
  '''
  def getInt2Event(self,source):
    regester = self.H3LIS200DL_REG_INT2_SRC;
    if(self.__uart_i2c == SPI_MODE):
      regester  = self.H3LIS200DL_REG_INT2_SRC | 0x80;
    reg = self.read_reg(regester)
      #print(reg & (1 << source))
      #print(1 << source)
    if (reg & (1 << source)) >= 1:
    #     print("true")
         return True
    else:
     #    print("false")
         return False
  '''
    @brief Enable sleep wake function
    @param enable:true\false
    @return 0
  '''
  def enableSleep(self, enable):
    reg = 0
    if enable == True:
      reg = 3
    else:
      reg = 0
    self.write_reg(self.H3LIS200DL_REG_CTRL_REG5,reg)
    return 0

  
  
  '''
    @brief Set data filtering mode
    @param mode:Four modes
              eCutoffMode1 = 0,
              eCutoffMode2,
              eCutoffMode3,
              eCutoffMode4,
              eShutDown,
  '''
  def setHFilterMode(self,mode):
    regester = self.H3LIS200DL_REG_CTRL_REG2;
    if(self.__uart_i2c == SPI_MODE):
      regester  = self.H3LIS200DL_REG_CTRL_REG2 | 0x80; 
    reg = self.read_reg(regester)
    if mode == self.E_SHUTDOWN:
      reg = reg & (~0x10)
      return 0
    else:
      reg = reg | 0x10
    reg = reg & (~3)
    reg = reg | mode
    self.write_reg(self.H3LIS200DL_REG_CTRL_REG2,reg)
  
  '''
    @brief Get the acceleration in the three directions of xyz
    @return Three-axis acceleration 
            acceleration_x;
            acceleration_y;
            acceleration_z;
  '''
  def readAcceFromXYZ(self):
    regester = self.H3LIS200DL_REG_STATUS_REG
    if(self.__uart_i2c == SPI_MODE):
      regester  = self.H3LIS200DL_REG_STATUS_REG | 0x80; 
    reg = self.read_reg(regester)
     # reg = 1
    data1 = [0]
    data2 = [0]
    data3 = [0]
    offset = 0
    if(reg & 0x01) == 1:
        if(self.__uart_i2c == SPI_MODE):
		       offset = 0x80
     
        data1[0] = self.read_reg(self.H3LIS200DL_REG_OUT_X+offset)
        data2[0] = self.read_reg(self.H3LIS200DL_REG_OUT_Y+offset)
        data3[0] = self.read_reg(self.H3LIS200DL_REG_OUT_Z+offset)
        data1[0] = np.int8(data1[0])
        data2[0] = np.int8(data2[0])
        data3[0] = np.int8(data3[0])
                   #struct.unpack("b", b"\x81")
       # if()
        #print(data1)
        #print(data2)
        #print(data3)
    #if(x > )
    x = (data1[0]*self.__range)/128
    y = (data2[0]*self.__range)/128
    z = (data3[0]*self.__range)/128
      
    return x,y,z
'''
  @brief An example of an i2c interface module
'''
class DFRobot_H3LIS200DL_I2C(DFRobot_H3LIS200DL): 
  def __init__(self ,bus ,addr):
    self.__addr = addr;
    super(DFRobot_H3LIS200DL_I2C, self).__init__(bus,0)

  '''
    @brief writes data to a register
    @param reg register address
    @param value written data
  '''
  def write_reg(self, reg, data):
        data1 = [0]
        data1[0] = data
        self.i2cbus.write_i2c_block_data(self.__addr ,reg,data1)
        #self.i2cbus.write_byte(self.__addr ,reg)
        #self.i2cbus.write_byte(self.__addr ,data)




  '''
    @brief read the data from the register
    @param reg register address
    @param value read data
  '''
  def read_reg(self, reg):
    self.i2cbus.write_byte(self.__addr,reg)
    time.sleep(0.01)
    rslt = self.i2cbus.read_byte(self.__addr)
    #print(rslt)
    return rslt

class DFRobot_H3LIS200DL_SPI(DFRobot_H3LIS200DL): 


  def __init__(self ,cs):
    super(DFRobot_H3LIS200DL_SPI, self).__init__(0,cs)
    #DFRobot_H3LIS200DL.__init__(0,0)
    #self._busy = GPIO(busy, GPIO.IN)
    self.__cs = GPIO(cs, GPIO.OUT)
    self.__cs.setOut(GPIO.LOW)
    self._spi = SPI(0, 0)

    
  '''
    @brief writes data to a register
    @param reg register address
    @param value written data
  '''
  def write_reg(self, reg, data):
     data1 =[reg,data]
     self.__cs.setOut(GPIO.LOW)
     self._spi.transfer(data1)
     self.__cs.setOut(GPIO.HIGH)
     #self._spi.transfer(data)
  '''
    @brief read the data from the register
    @param reg register address
    @param value read data
  '''
  def read_reg(self, reg):
     data1 =[reg]
     self.__cs.setOut(GPIO.LOW)
     self._spi.transfer(data1)
     time.sleep(0.01);
     data = self._spi.readData(1);
     self.__cs.setOut(GPIO.HIGH)
     #print(data)
     return  data[0]