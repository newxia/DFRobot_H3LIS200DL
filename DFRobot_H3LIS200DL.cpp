/*!
 * @file DFRobot_H3LIS200DL.cpp
 * @brief Define the basic structure of class DFRobot_H3LIS200DL 
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-12-23
 * @get from https://www.dfrobot.com
 * @https://github.com/DFRobot/DFRobot_H3LIS200DL
 */

#include <DFRobot_H3LIS200DL.h>




DFRobot_H3LIS200DL::DFRobot_H3LIS200DL(){



}

int DFRobot_H3LIS200DL::begin(void){


  uint8_t identifier = 0; 
  uint8_t regester = H3LIS200DL_REG_CARD_ID;
  if(_interface == 1){
    regester  = H3LIS200DL_REG_CARD_ID | 0x80;
  }
  readReg(regester,&identifier,1);
  DBG(identifier);
  if(identifier == 0x32){
    return 0;
  } else if(identifier == 0 || identifier == 0xff){
    DBG("Communication failure");
    return 1;
  } else {  
    DBG("the ic is not LIS331HH");
    return 1;
  }

}

uint8_t DFRobot_H3LIS200DL::getID(){
  uint8_t identifier; 
  uint8_t regester = H3LIS200DL_REG_CARD_ID;
  if(_interface == 1){
    regester  = H3LIS200DL_REG_CARD_ID | 0x80;
  }
  readReg(regester,&identifier,1);
  return identifier;
}
void DFRobot_H3LIS200DL::setRange(eRange_t range){
  uint8_t reg = 0;
  uint8_t regester = H3LIS200DL_REG_CTRL_REG4;
  if(_interface == 1){
    regester  = H3LIS200DL_REG_CTRL_REG4 | 0x80;
  }
  readReg(regester,&reg,1);
  if(range == eOnehundred){
    reg = reg & (~0x10);
    _range = 100;
  } else if(range == eTwohundred){
    reg = reg | 0x10;
    _range = 200;
  } else {
    reg = reg & (~0x10);
  }
  DBG(reg);
  writeReg(H3LIS200DL_REG_CTRL_REG4,&reg,1);
}



void DFRobot_H3LIS200DL::setAcquireRate(ePowerMode_t rate)
{
  uint8_t reg = 0;

  uint8_t regester = H3LIS200DL_REG_CTRL_REG1;
  if(_interface == 1){
    regester  = H3LIS200DL_REG_CTRL_REG1 | 0x80;
  }
  readReg(regester,&reg,1);
  switch(rate){
    case ePowerDown : {
     reg = reg & (~(0x7 << 5));
     break;
    }
    case eLowPower_halfHZ :{
     reg = reg & (~(0x7 << 5));
     reg = reg | (0x02 << 5);
     break;
    }
    case eLowPower_1HZ :{
     reg = reg & (~(0x7 << 5));
     reg = reg | (0x03 << 5);
     break;
    }
    case eLowPower_2HZ :{
     reg = reg & (~(0x7 << 5));
     reg = reg | (0x04 << 5);
     break;
    }
    case eLowPower_5HZ :{
     reg = reg & (~(0x7 << 5));
     reg = reg | (0x05 << 5);
     break;
    }
    case eLowPower_10HZ :{
     reg = reg & (~(0x7 << 5));
     reg = reg | (0x06 << 5);
     break;
    }
    case eNormal_50HZ :{
     reg = reg & (~(0x7 << 5));
     reg = reg | (0x01 << 5);
     reg = reg & (~(0x3 << 3));
     break;
    }
    case eNormal_100HZ :{
     reg = reg & (~(0x7 << 5));
     reg = reg | (0x01 << 5);
     reg = reg & (~(0x3 << 3));
     reg = reg | (0x01 << 3);
     break;
    }
    case eNormal_400HZ :{
     reg = reg & (~(0x7 << 5));
     reg = reg | (0x01 << 5);
     reg = reg & (~(0x3 << 3));
     reg = reg | (0x02 << 3);
     break;
    }
    case eNormal_1000HZ :{
     reg = reg & (~(0x7 << 5));
     reg = reg | (0x01 << 5);
     reg = reg & (~(0x3 << 3));
     reg = reg | (0x03 << 3);
     break;
    }
  }
  DBG(reg);
  writeReg(H3LIS200DL_REG_CTRL_REG1,&reg,1);
}
void DFRobot_H3LIS200DL::setIntOneTh(uint8_t threshold)
{
    uint8_t reg = (threshold * 128)/_range;
    DBG(reg);
    writeReg(H3LIS200DL_REG_INT1_THS,&reg,1);
}
void DFRobot_H3LIS200DL::setIntTwoTh(uint8_t threshold)
{
    uint8_t reg = (threshold * 128)/_range;
    DBG(reg);
    writeReg(H3LIS200DL_REG_INT2_THS,&reg,1);
}
void DFRobot_H3LIS200DL::enableInterruptEvent(eInterruptSource_t source,eInterruptEvent_t event)
{
  uint8_t reg = 0;

  uint8_t regester1 = H3LIS200DL_REG_INT1_CFG;
  uint8_t regester2 = H3LIS200DL_REG_INT2_CFG;
  if(_interface == 1){
    regester1  = H3LIS200DL_REG_INT1_CFG | 0x80;
    regester2  = H3LIS200DL_REG_INT2_CFG | 0x80;
  }

  if(source == eINT1)
    readReg(regester1,&reg,1);
  else
    readReg(regester2,&reg,1);

  if(reset == 1){
     reg = 0;
	 //Serial.println("reset");
     //uint8_t reg1 = 0x84;
     //writeReg(LIS331HH_REG_CTRL_REG3,&reg1,1);
    // writeReg(LIS331HH_REG_INT2_CFG,&reg,1);
     reset = 0;
  }
  switch(event){
    case eXLowThanTh :{
      reg = reg | 0x01;
      break;
    }
    case eXhigherThanTh :{
      reg = reg | 0x02;
      break;
    }
    case eYLowThanTh :{
      reg = reg | 0x04;
      break;
    }
    case eYhigherThanTh :{
      reg = reg | 0x08;
      break;
    }
    case eZLowThanTh :{
      reg = reg | 0x10;
      break;
    }
    case eZhigherThanTh :{
      reg = reg | 0x20;
      break;
    }
  }
  DBG(reg);
  if(source == eINT1)
    writeReg(H3LIS200DL_REG_INT1_CFG,&reg,1);
  else
    writeReg(H3LIS200DL_REG_INT2_CFG,&reg,1);
 
 
}
bool DFRobot_H3LIS200DL::getInt1Event(eInterruptEvent_t source)
{
  uint8_t reg = 0;
  uint8_t regester = H3LIS200DL_REG_INT1_SRC;
  
  if(_interface == 1){
    regester  = H3LIS200DL_REG_INT1_SRC | 0x80;
  }
  readReg(regester,&reg,1);

  DBG(reg);
  if(reg & (1 << source))  return true;
  return false;
}
bool DFRobot_H3LIS200DL::getInt2Event(eInterruptEvent_t source)
{
  uint8_t reg = 0;
  uint8_t regester = H3LIS200DL_REG_INT2_SRC;
  if(_interface == 1){
    regester  = H3LIS200DL_REG_INT2_SRC | 0x80;
  }
  readReg(regester,&reg,1);
  DBG(reg);
  if(reg & (1 << source))  return true;
  return false;
}

/*
void DFRobot_H3LIS200DL::setInt1PadSource(eInterruptSource_t source){
  uint8_t reg = 0;
  readReg(H3LIS200DL_REG_CTRL_REG3,&reg,1);
  if(source == eINT1){
     reg = reg & (~3);
     //reg = reg & (~(3<<3));
     reg = reg | (3 << 3);
     //reg = reg | 3;
  }else if(source == eINT2){
     reg = reg & (~(3<<3));
     reg = reg | 3;
  }
  
  writeReg(H3LIS200DL_REG_CTRL_REG3,&reg,1);
}
void DFRobot_H3LIS200DL::setInt2PadSource(eInterruptSource_t source){
  uint8_t reg = 0;
  readReg(H3LIS200DL_REG_CTRL_REG3,&reg,1);
  if(source == eINT1){
   
  }else if(eINT2 == eINT2){

  }
  writeReg(H3LIS200DL_REG_CTRL_REG3,&reg,1);
  
  

}
*/
int DFRobot_H3LIS200DL::enableSleep(bool enable)
{
  uint8_t reg = 0;

  if(enable == true)
    reg = 3;
  else
    reg = 0;
  writeReg(H3LIS200DL_REG_CTRL_REG5,&reg,1);

  return 0;
}
void DFRobot_H3LIS200DL::setHFilterMode(eHighPassFilter_t mode){

  uint8_t reg = 0;
  uint8_t regester = H3LIS200DL_REG_CTRL_REG2;
  if(_interface == 1){
    regester  = H3LIS200DL_REG_CTRL_REG2 | 0x80;
  }
  readReg(regester,&reg,1);
  if(mode == eShutDown){
    reg = reg & (~0x10);
    return;
  } else{
    reg = reg | 0x10;
  }
  reg = reg & (~3);
  reg = reg | (uint8_t)mode;
  DBG(reg);
  writeReg(H3LIS200DL_REG_CTRL_REG2,&reg,1);
}


void DFRobot_H3LIS200DL::enableXYZ(){
  uint8_t reg = 0;
  readReg(H3LIS200DL_REG_CTRL_REG1,&reg,1);
  reg = reg | 0x07;
  DBG(reg);
  writeReg(H3LIS200DL_REG_CTRL_REG1,&reg,1);
}


int DFRobot_H3LIS200DL::readACCFromX(){
  uint8_t reg = 0;
  uint8_t ACCX = 0;
  readReg(H3LIS200DL_REG_STATUS_REG,&reg,1);
  DBG(reg);
  if((reg & 0x01) == 1){
     readReg(H3LIS200DL_REG_OUT_X,&ACCX,1);
     int a = ((int8_t)ACCX *_range)/128;
     return a;
  } else{
     return 0;
  }

}
int DFRobot_H3LIS200DL::readACCFromY(){
  uint8_t reg = 0;
  uint8_t ACCY = 0;
  readReg(H3LIS200DL_REG_STATUS_REG,&reg,1);
  DBG(reg);
  if((reg & 0x02) == 1){
     readReg(H3LIS200DL_REG_OUT_Y,&ACCY,1);
     int a = ((int8_t)ACCY *_range)/128;
     return a;
  } else{
     return 0;
  }
}
DFRobot_H3LIS200DL::sAccel_t DFRobot_H3LIS200DL::getAcceFromXYZ()
{

  uint8_t reg = 0;
  uint8_t ACCX = 0;
  uint8_t sensorData[3];
  sAccel_t accel;
  uint8_t offset = 0x0;
  uint8_t regester = H3LIS200DL_REG_STATUS_REG;
  if(_interface == 1){
    regester  = H3LIS200DL_REG_STATUS_REG | 0x80;
  }
  readReg(regester,&reg,1);
  if((reg & 0x01) == 1){
     if(_interface == 1){
		 offset = 0x80;
     }
     readReg(H3LIS200DL_REG_OUT_X+offset,&sensorData[0],1);

     readReg(H3LIS200DL_REG_OUT_Y+offset,&sensorData[1],1);

     readReg(H3LIS200DL_REG_OUT_Z+offset,&sensorData[2],1);


     float a = ((int8_t)sensorData[0]*_range)/128;
	 float b = ((int8_t)sensorData[1]*_range)/128;
	 float c = ((int8_t)sensorData[2]*_range)/128;
     accel.acceleration_x = a;
     accel.acceleration_y = b;
     accel.acceleration_z = c;

      
  }
return accel;

}
int DFRobot_H3LIS200DL::readACCFromZ(){
  uint8_t reg = 0;
  uint8_t ACCZ = 0;
  readReg(H3LIS200DL_REG_STATUS_REG,&reg,1);
  if((reg & 0x04) == 1){
     readReg(H3LIS200DL_REG_OUT_Z,&ACCZ,1);
     int a = ((int8_t)ACCZ *_range)/128;
     return a;
  } else{
     return 0;
  }
}




DFRobot_H3LIS200DL_I2C::DFRobot_H3LIS200DL_I2C(TwoWire * pWire,uint8_t addr)
{
  _deviceAddr = addr;
  _pWire = pWire;
}

int DFRobot_H3LIS200DL_I2C::begin(void)
{
  _pWire->begin();
  return DFRobot_H3LIS200DL::begin();
}

uint8_t DFRobot_H3LIS200DL_I2C::writeReg(uint8_t reg, const void * pBuf, size_t size)
{
  if(pBuf == NULL){
	  DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(&reg, 1);

  for(uint16_t i = 0; i < size; i++){
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}

uint8_t DFRobot_H3LIS200DL_I2C::readReg(uint8_t reg, uint8_t* pBuf, size_t size)
{
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(&reg, 1);
  if( _pWire->endTransmission() != 0){
      return 0;
  }
  _pWire->requestFrom(_deviceAddr, (uint8_t) size);
  for(uint16_t i = 0; i < size; i++){
	  
    _pBuf[i] = _pWire->read();
	Serial.println(_pBuf[i],HEX);
	  DBG(_pBuf[i]);
  }
  return size;
}







DFRobot_H3LIS200DL_SPI::DFRobot_H3LIS200DL_SPI(uint8_t cs,SPIClass *pSpi)
{
  _pSpi = pSpi;
  _cs = cs;
}




int DFRobot_H3LIS200DL_SPI::begin(void){

  _pSpi->begin();
  pinMode(_cs,OUTPUT);
  digitalWrite(_cs,1);
  _interface = 1;
  return DFRobot_H3LIS200DL::begin();
}
uint8_t DFRobot_H3LIS200DL_SPI::readReg(uint8_t reg,uint8_t * pBuf ,size_t size){
  if(pBuf == NULL){
	  DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  size_t count = 0;
  digitalWrite(_cs,0);
  _pSpi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  _pSpi->transfer(reg);
  while(size--) {
    *_pBuf = SPI.transfer(0xFF);
    _pBuf++;
    count++;
  }
  _pSpi->endTransaction();
  digitalWrite(_cs,1);
  return count;

}

uint8_t  DFRobot_H3LIS200DL_SPI::writeReg(uint8_t reg,const void *pBuf,size_t size){
  if(pBuf == NULL){
	  DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  digitalWrite(_cs,0);
  _pSpi->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  _pSpi->transfer(reg);
  while(size--) {
    _pSpi->transfer(*_pBuf);
    _pBuf ++;
  }

  SPI.endTransaction();
  digitalWrite(_cs,1);
}
