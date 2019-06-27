/* Copyright (c)  2018-2025 Wuhan Nameless Innovation Technology Co.,Ltd. All rights reserved.*/
/*----------------------------------------------------------------------------------------------------------------------/
*               本程序只供购买者学习使用，版权著作权属于无名科创团队，无名科创团队将飞控程序源码提供给购买者，
*               购买者要为无名科创团队提供保护，未经作者许可，不得将源代码提供给他人，不得将源代码放到网上供他人免费下载， 
*               更不能以此销售牟利，如发现上述行为，无名科创团队将诉之以法律解决！！！
-----------------------------------------------------------------------------------------------------------------------/
*               生命不息、奋斗不止；前人栽树，后人乘凉！！！
*               开源不易，且学且珍惜，祝早日逆袭、进阶成功！！！
*               学习优秀者，简历可推荐到DJI、ZEROTECH、XAG、AEE、GDU、AUTEL、EWATT、HIGH GREAT等公司就业
*               求职简历请发送：15671678205@163.com，需备注求职意向单位、岗位、待遇等
*               无名科创开源飞控QQ群：540707961
*               CSDN博客：http://blog.csdn.net/u011992534
*               优酷ID：NamelessCotrun无名小哥
*               B站教学视频：https://space.bilibili.com/67803559/#/video
*               客户使用心得、改进意见征集贴：http://www.openedv.com/forum.php?mod=viewthread&tid=234214&extra=page=1
*               淘宝店铺：https://shop348646912.taobao.com/?spm=2013.1.1000126.2.5ce78a88ht1sO2
*               百度贴吧:无名科创开源飞控
*               公司官网:www.nameless.tech
*               修改日期:2019/4/12
*               版本：躺赢者——CarryPilot_V1.0
*               版权所有，盗版必究。
*               Copyright(C) 2017-2025 武汉无名创新科技有限公司 
*               All rights reserved
*               重要提示：
*               正常淘宝咸鱼转手的飞控、赠送朋友、传给学弟的都可以进售后群学习交流，
*               不得直接在网上销售无名创新资料，无名创新代码有声明版权，他人不得将
*               资料代码传网上供他人下载，不得以谋利为目的销售资料代码，发现有此操
*               作者，公司会提前告知，请1天内及时处理，否则你的学校、单位、姓名、电
*               话、地址信息会被贴出在公司官网、官方微信公众平台、官方技术博客、知乎
*               专栏以及淘宝店铺首页予以公示公告，此种所作所为，会成为个人污点，影响
*               升学、找工作、社会声誉、很快就很在无人机界出名，后果很严重。
*               因此行为给公司造成重大损失者，会以法律途径解决，感谢您的合作，谢谢！！！
----------------------------------------------------------------------------------------------------------------------*/
/* Copyright (C) 2015 Kristian Sloth Lauszus. All rights reserved.

This software may be distributed and modified under the terms of the GNU
General Public License version 2 (GPL2) as published by the Free Software
Foundation and appearing in the file GPL2.TXT included in the packaging of
this file. Please note that GPL2 Section 2[b] requires that all works based
on this software must also be made publicly available under the terms of
the GPL2 ("Copyleft").

Contact information
-------------------

Kristian Sloth Lauszus
Web      :  http://www.lauszus.com
e-mail   :  lauszus@gmail.com
*/

#include <stdint.h>
#include <stdbool.h>

#include "I2C.h"

#include "hw_memmap.h"
#include "gpio.h"
#include "pin_map.h"
#include "myiic.h"
#include "sysctl.h"
/***********************************************************
@函数名：Init_I2C
@入口参数：无
@出口参数：无
功能描述：TM4C硬件I2C初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Init_I2C(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1); // Enable I2C1 peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable GPIOA peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  
  // Use alternate function
  GPIOPinConfigure(GPIO_PA6_I2C1SCL);
  GPIOPinConfigure(GPIO_PA7_I2C1SDA);
  
  GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6); // Use pin with I2C SCL peripheral
  GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7); // Use pin with I2C peripheral
	
  I2CMasterInitExpClk(I2C1_BASE, 400*100000,true); // Enable and set frequency to 400 kHz  100
  //I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(),true);          
  SysCtlDelay(2); // Insert a few cycles after enabling the I2C to allow the clock to be fully activated
}

/***********************************************************
@函数名：i2cWriteData
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length
@出口参数：无
功能描述：I2C写数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2cWriteData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length) {
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode
  
  I2CMasterDataPut(I2C1_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START); // Send start condition
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  
  for (uint8_t i = 0; i < length - 1; i++) {
    I2CMasterDataPut(I2C1_BASE, data[i]); // Place data into data register
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); // Send continues condition
    while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  }
  
  I2CMasterDataPut(I2C1_BASE, data[length - 1]); // Place data into data register
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // Send finish condition
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
}

/***********************************************************
@函数名：i2cRead
@入口参数：uint8_t addr, uint8_t regAddr
@出口参数：无
功能描述：I2C读数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
uint8_t i2cRead(uint8_t addr, uint8_t regAddr) {
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode
  
  I2CMasterDataPut(I2C1_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, true); // Set to read mode
  
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); // Tell master to read data
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  return I2CMasterDataGet(I2C1_BASE); // Read data
}

/***********************************************************
@函数名：i2cWrite
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t data
@出口参数：无
功能描述：I2C写数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2cWrite(uint8_t addr, uint8_t regAddr, uint8_t data) {
  i2cWriteData(addr, regAddr, &data, 1);
}


/***********************************************************
@函数名：i2cReadData
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t *data,
uint8_t length
@出口参数：无
功能描述：I2C读数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2cReadData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length) {
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, false); // Set to write mode
  I2CMasterDataPut(I2C1_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done 
  I2CMasterSlaveAddrSet(I2C1_BASE, addr, true); // Set to read mode
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START); // Send start condition
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  data[0] = I2CMasterDataGet(I2C1_BASE); // Place data into data register 
  for (uint8_t i = 1; i < length - 1; i++) 
	{
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT); // Send continues condition
    while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
    data[i] = I2CMasterDataGet(I2C1_BASE); // Place data into data register
  }
  I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); // Send finish condition
  while (I2CMasterBusy(I2C1_BASE)); // Wait until transfer is done
  data[length - 1] = I2CMasterDataGet(I2C1_BASE); // Place data into data register
}


void Single_WriteI2C(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)
{
  i2cWrite(SlaveAddress,REG_Address,REG_data);
}	

unsigned char Single_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address)
{
  return i2cRead(SlaveAddress,REG_Address);
}

short int Double_ReadI2C(unsigned char SlaveAddress,unsigned char REG_Address)
{
  unsigned char msb , lsb ;
  msb = i2cRead(SlaveAddress,REG_Address);
  lsb = i2cRead(SlaveAddress,REG_Address+1);
  return ( ((short int)msb) << 8 | lsb) ;
}

////////////////////////
/***********************************************************
@函数名：Init_I2C2
@入口参数：无
@出口参数：无
功能描述：TM4C硬件I2C初始化
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Init_I2C0(void) {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0); // Enable I2C0 peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Enable GPIOB peripheral
  SysCtlDelay(2); // Insert a few cycles after enabling the peripheral to allow the clock to be fully activated
  
  // Use alternate function
  GPIOPinConfigure(GPIO_PB2_I2C0SCL);
  GPIOPinConfigure(GPIO_PB3_I2C0SDA);
  
  GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2); // Use pin with I2C SCL peripheral
  GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3); // Use pin with I2C peripheral
  
  I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(),true); // Enable and set frequency to 400 kHz
  
  SysCtlDelay(2); // Insert a few cycles after enabling the I2C to allow the clock to be fully activated
}

/***********************************************************
@函数名：i2cWriteData
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length
@出口参数：无
功能描述：I2C写数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2c0WriteData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length) {
  I2CMasterSlaveAddrSet(I2C0_BASE, addr, false); // Set to write mode
  
  I2CMasterDataPut(I2C0_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); // Send start condition
  while (I2CMasterBusy(I2C0_BASE)); // Wait until transfer is done
  
  for (uint8_t i = 0; i < length - 1; i++) {
    I2CMasterDataPut(I2C0_BASE, data[i]); // Place data into data register
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); // Send continues condition
    while (I2CMasterBusy(I2C0_BASE)); // Wait until transfer is done
  }
  
  I2CMasterDataPut(I2C0_BASE, data[length - 1]); // Place data into data register
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // Send finish condition
  while (I2CMasterBusy(I2C0_BASE)); // Wait until transfer is done
}

/***********************************************************
@函数名：i2cRead
@入口参数：uint8_t addr, uint8_t regAddr
@出口参数：无
功能描述：I2C读数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
uint8_t i2c0Read(uint8_t addr, uint8_t regAddr) {
  I2CMasterSlaveAddrSet(I2C0_BASE, addr, false); // Set to write mode
  
  I2CMasterDataPut(I2C0_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
  while (I2CMasterBusy(I2C0_BASE)); // Wait until transfer is done
  
  I2CMasterSlaveAddrSet(I2C0_BASE, addr, true); // Set to read mode
  
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); // Tell master to read data
  while (I2CMasterBusy(I2C0_BASE)); // Wait until transfer is done
  return I2CMasterDataGet(I2C0_BASE); // Read data
}

/***********************************************************
@函数名：i2cWrite
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t data
@出口参数：无
功能描述：I2C写数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2c0Write(uint8_t addr, uint8_t regAddr, uint8_t data) {
  i2c0WriteData(addr, regAddr, &data, 1);
}


/***********************************************************
@函数名：i2cReadData
@入口参数：uint8_t addr, uint8_t regAddr, uint8_t *data,
uint8_t length
@出口参数：无
功能描述：I2C读数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void i2c0ReadData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length) {
  I2CMasterSlaveAddrSet(I2C0_BASE, addr, false); // Set to write mode
  I2CMasterDataPut(I2C0_BASE, regAddr); // Place address into data register
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Send data
  while (I2CMasterBusy(I2C0_BASE)); // Wait until transfer is done 
  I2CMasterSlaveAddrSet(I2C0_BASE, addr, true); // Set to read mode
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START); // Send start condition
  while (I2CMasterBusy(I2C0_BASE)); // Wait until transfer is done
  data[0] = I2CMasterDataGet(I2C0_BASE); // Place data into data register 
  for (uint8_t i = 1; i < length - 1; i++) 
	{
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT); // Send continues condition
    while (I2CMasterBusy(I2C0_BASE)); // Wait until transfer is done
    data[i] = I2CMasterDataGet(I2C0_BASE); // Place data into data register
  }
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); // Send finish condition
  while (I2CMasterBusy(I2C0_BASE)); // Wait until transfer is done
  data[length - 1] = I2CMasterDataGet(I2C0_BASE); // Place data into data register
}
