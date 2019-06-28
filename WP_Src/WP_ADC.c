
#include "Headfile.h"
#include "WP_ADC.h"
#include "hw_adc.h"
#include "adc.h"

uint32_t value[8];
double value_filter;
/***********************************************************
@函数名：ADC_Init
@入口参数：无
@出口参数：无
功能描述：电压测量端口PE3，作为ADC0的通道0初始化，非中断式采集
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ADC_Init(void)//ADC初始化配置   
{    
  SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);// Enable the ADC1 module.
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0));// Wait for the ADC1 module to be ready.	
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);    
  GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
  //ADCReferenceSet(ADC0_BASE, ADC_REF_EXT_3V);
  //Enable the first sample sequencer to capture the value of channel 0 when
  //the processor trigger occurs.  
  ADCSequenceConfigure(ADC0_BASE,0,ADC_TRIGGER_PROCESSOR, 0); 
  //ADCHardwareOversampleConfigure(ADC0_BASE, 8);	
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0 |ADC_CTL_END | ADC_CTL_IE);    
	//ADCIntClear(ADC0_BASE, 0);
	ADCSequenceEnable(ADC0_BASE, 0);    
  //ADCIntEnable(ADC0_BASE, 0);   
} 


float Battery_Voltage;
/***********************************************************
@函数名：Get_Battery_Voltage
@入口参数：无
@出口参数：无
功能描述：测量电池电压，飞控默认分压比位11，故测量电压不要超过
3.3V*11=36.6V，若想测更大的电压，调整板子上分压电阻阻值即可
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Get_Battery_Voltage(void)//ADC获取   
{
	ADCProcessorTrigger(ADC0_BASE, 0);   
	while(!ADCIntStatus(ADC0_BASE, 0, false)) {;}
	ADCIntClear(ADC0_BASE, 0);	
	ADCSequenceDataGet(ADC0_BASE, 0, value);   
	//value[0] =  HWREG(ADC0_BASE+ ADC_SEQ + (ADC_SEQ_STEP*0) + ADC_SSFIFO);
	value_filter=(double)(0.7*value_filter+10.89*value[0]/4095.0f);
	Battery_Voltage=value_filter;		
}

