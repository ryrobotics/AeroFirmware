
#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

void I2C_GPIO_Config(void);
void Single_WriteI2C0(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_ReadI2C0(unsigned char SlaveAddress,unsigned char REG_Address);

void hmc_write_reg(u8 adr,u8 reg,u8 data);
u8 hmc_read_reg(u8 adr,u8 reg);



#endif

