
#ifndef __IST8310_H
#define __IST8310_H


/* ist8310 Slave Address Select : default address 0x0C
*        CAD1  |  CAD0   |  Address
*    ------------------------------
*         VSS   |   VSS  |  0CH
*         VSS   |   VDD  |  0DH
*         VDD   |   VSS  |  0EH
*         VDD   |   VDD  |  0FH
* if CAD1 and CAD0 are floating, I2C address will be 0EH
*
*
* CTRL_REGA: Control Register 1
* Read Write
* Default value: 0x0A
* 7:4  0   Reserved.
* 3:0  DO2-DO0: Operating mode setting
*        DO3  |  DO2 |  DO1 |  DO0 |   mode
*    ------------------------------------------------------
*         0   |   0  |  0   |  0   |   Stand-By mode
*         0   |   0  |  0   |  1   |   Single measurement mode
*                                       Others: Reserved
*
* CTRL_REGB: Control Register 2
* Read Write
* Default value: 0x0B
* 7:4  0   Reserved.
* 3    DREN : Data ready enable control: 
*      0: disable 
*      1: enable
* 2    DRP: DRDY pin polarity control
*      0: active low
*      1: active high
* 1    0   Reserved.
* 0    SRST: Soft reset, perform Power On Reset (POR) routine
*      0: no action
*      1: start immediately POR routine
*      This bit will be set to zero after POR routine
*/
#define IST8310_SLAVE_ADDRESS     0x0E
#define IST8310_REG_STB           0x0C	//Self-Test response
#define IST8310_REG_INFO          0x01	//More Info
#define IST8310_REG_WIA           0x00	//Who I am
#define IST8310_REG_DATAX         0x03	//Output Value x
#define IST8310_REG_DATAY         0x05	//Output Value y
#define IST8310_REG_DATAZ         0x07	//Output Value z
#define IST8310_REG_STAT1         0x02	//Status register
#define IST8310_REG_STAT2         0x09	//Status register
#define IST8310_REG_CNTRL1        0x0A	//Control setting register 1
#define IST8310_REG_CNTRL2        0x0B	//Control setting register 2
#define IST8310_REG_CNTRL3        0x0D	//Control setting register 3
#define IST8310_REG_OFFSET_START  0xDC	//Offset
#define IST8310_REG_SELECTION_REG 0x42   //Sensor Selection register
#define IST8310_REG_TEST_REG      0x40   //Chip Test register
#define IST8310_REG_TUNING_REG    0x47    //Bandgap Tuning register
/*---IST8310 cross-axis matrix Address-----------------danny-----*/
#define IST8310_REG_XX_CROSS_L    0x9C  //cross axis xx low byte
#define IST8310_REG_XX_CROSS_H    0x9D  //cross axis xx high byte
#define IST8310_REG_XY_CROSS_L    0x9E  //cross axis xy low byte
#define IST8310_REG_XY_CROSS_H    0x9F  //cross axis xy high byte
#define IST8310_REG_XZ_CROSS_L    0xA0  //cross axis xz low byte
#define IST8310_REG_XZ_CROSS_H    0xA1  //cross axis xz high byte                          =       ;
#define IST8310_REG_YX_CROSS_L    0xA2  //cross axis yx low byte
#define IST8310_REG_YX_CROSS_H    0xA3  //cross axis yx high byte
#define IST8310_REG_YY_CROSS_L    0xA4  //cross axis yy low byte
#define IST8310_REG_YY_CROSS_H    0xA5  //cross axis yy high byte
#define IST8310_REG_YZ_CROSS_L    0xA6  //cross axis yz low byte
#define IST8310_REG_YZ_CROSS_H    0xA7  //cross axis yz high byte                    =       ;
#define IST8310_REG_ZX_CROSS_L    0xA8  //cross axis zx low byte
#define IST8310_REG_ZX_CROSS_H    0xA9  //cross axis zx high byte
#define IST8310_REG_ZY_CROSS_L    0xAA  //cross axis zy low byte
#define IST8310_REG_ZY_CROSS_H    0xAB  //cross axis zy high byte
#define IST8310_REG_ZZ_CROSS_L    0xAC  //cross axis zz low byte
#define IST8310_REG_ZZ_CROSS_H    0xAD  //cross axis zz high byte
#define IST8310_AXES_NUM          3






void IST8310_Init(void);
uint8_t IST8310_Read_Data(vector3f *mag);
extern Vector2f magn;
extern Vector3f mag_offset;
extern Butter_Parameter Mag_Parameter;
#endif

