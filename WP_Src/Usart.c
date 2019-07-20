
#include "Headfile.h"
#include "uart.h"
#include "Usart.h"
#include "Ringbuf.h"
//串口循环队列缓冲数据定义
RingBuff_t COM0_Rx_Buf,COM1_Rx_Buf,COM2_Rx_Buf,COM3_Rx_Buf,COM4_Rx_Buf,COM5_Rx_Buf,COM6_Rx_Buf,COM7_Rx_Buf;
/***********************************************************
@函数名：UART0_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口0数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART0_IRQHandler(void)//UART0中断函数
{	
  //获取中断标志 原始中断状态 不屏蔽中断标志		
  uint32_t flag = UARTIntStatus(UART0_BASE,1);
  //清除中断标志	
  UARTIntClear(UART0_BASE,flag);		
  //判断FIFO是否还有数据		
  while(UARTCharsAvail(UART0_BASE))		
  {			
    RingBuf_Write(UARTCharGet(UART0_BASE),&COM0_Rx_Buf,32);//往环形队列里面写数据
    //RDroneStudio_Receive(UARTCharGet(UART0_BASE));		
  }
}

/***********************************************************
@函数名：ConfigureUART0
@入口参数：无
@出口参数：无
功能描述：串口0配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART0(void)//串口0初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);//使能UART外设
  GPIOPinConfigure(GPIO_PA0_U0RX);//GPIO模式配置 PA0--RX PA1--TX 
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(0, 115200, 16000000);
  //UART协议配置 波特率115200 8位 1停止位  无校验位	
  //UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断	//禁用后接收1位就产生中断	
  UARTFIFODisable(UART0_BASE);//使能UART0中断	IntEnable(INT_UART0);	
  UARTIntEnable(UART0_BASE,UART_INT_RX);//使能UART0接收中断		
  UARTIntRegister(UART0_BASE,UART0_IRQHandler);//UART中断地址注册	
  IntPrioritySet(INT_UART0, USER_INT2);
}

/***********************************************************
@函数名：USART0_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口0发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART0_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART0_BASE, *pui8Buffer++);
  }
}
/***********************************************************
@函数名：wust_sendware
@入口参数：unsigned char *wareaddr, int16_t waresize
@出口参数：无
功能描述：山外虚拟示波器发送
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void wust_sendware(unsigned char *wareaddr, int16_t waresize)//山外发送波形
{
#define CMD_WARE     3
  uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};//帧头
  uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};//帧尾
  USART0_Send(cmdf, sizeof(cmdf));
  USART0_Send(wareaddr, waresize);
  USART0_Send(cmdr, sizeof(cmdr));
}

/***********************************************************
@函数名：UART1_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口1数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART1_IRQHandler(void)//UART1中断函数
{				
  uint32_t flag = UARTIntStatus(UART1_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志	
  UARTIntClear(UART1_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART1_BASE))//判断FIFO是否还有数据		
  {			
    //RingBuf_Write(UARTCharGet(UART1_BASE),&COM1_Rx_Buf,50);//往环形队列里面写数据	
    ANO_DT_Data_Receive_Prepare(UARTCharGet(UART1_BASE)); 		
  }
}


/***********************************************************
@函数名：USART1_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口1发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART1_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART1_BASE, *pui8Buffer++);
  }
}
/***********************************************************
@函数名：ConfigureUART1
@入口参数：无
@出口参数：无
功能描述：串口1配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART1(void)//串口1初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);//使能UART外设
  GPIOPinConfigure(GPIO_PB0_U1RX);//GPIO模式配置 PB0--RX PB1--TX 
  GPIOPinConfigure(GPIO_PB1_U1TX);
  GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
  UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(1, 115200, 16000000);
  UARTFIFODisable(UART1_BASE);//使能UART1中断	
  UARTIntEnable(UART1_BASE,UART_INT_RX);//使能UART1接收中断		
  UARTIntRegister(UART1_BASE,UART1_IRQHandler);//UART1中断地址注册	
  IntPrioritySet(INT_UART1, USER_INT4);
}


/***********************************************************
@函数名：UART2_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口2数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART2_IRQHandler(void)
{
  uint32_t flag = UARTIntStatus(UART2_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART2_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART2_BASE))//判断FIFO是否还有数据				
  {		
    //输出得到的数据
    RingBuf_Write(UARTCharGet(UART2_BASE),&COM2_Rx_Buf,200);//往环形队列里面写数据
    if(COM2_Rx_Buf.Ring_Buff[0]!=0XB5)
    {
      COM2_Rx_Buf.Head=1;
      COM2_Rx_Buf.Tail=0; 
    }		
  }
}

/***********************************************************
@函数名：USART2_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口2发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART2_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART2_BASE, *pui8Buffer++);
  }
}
/***********************************************************
@函数名：ConfigureUART2
@入口参数：无
@出口参数：无
功能描述：串口2配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART2(unsigned long bound)//串口2初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);//使能UART外设
  
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//解锁PD6
  HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;//确认
  HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;//重新锁定
  
  GPIOPinConfigure(GPIO_PD6_U2RX);//GPIO模式配置 PD6--RX PD7--TX 
  GPIOPinConfigure(GPIO_PD7_U2TX);
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO的UART模式配置
  UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), bound,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART2_BASE);//使能UART2中断	
  UARTIntEnable(UART2_BASE,UART_INT_RX |UART_INT_RT);//使能UART0接收中断		
  UARTIntRegister(UART2_BASE,UART2_IRQHandler);//UART中断地址注册	
  IntPrioritySet(INT_UART2, USER_INT1);
}

/***********************************************************
@函数名：UART3_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口3数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART3_IRQHandler(void)
{		
  uint32_t flag = UARTIntStatus(UART3_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART3_BASE,flag);//清除中断标志			
  while(UARTCharsAvail(UART3_BASE))//判断FIFO是否还有数据		
  {			
    RingBuf_Write(UARTCharGet(UART3_BASE),&COM3_Rx_Buf,24);//往环形队列里面写数据
    //SDK_Data_Receive_Prepare(UARTCharGet(UART3_BASE));		
  }
}

/***********************************************************
@函数名：USART3_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口3发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART3_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART3_BASE, *pui8Buffer++);
  }
}

/***********************************************************
@函数名：ConfigureUART3
@入口参数：无
@出口参数：无
功能描述：串口3配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART3(void)//串口3初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);//使能UART外设
  GPIOPinConfigure(GPIO_PC6_U3RX);//GPIO模式配置 PC6--RX PC7--TX 
  GPIOPinConfigure(GPIO_PC7_U3TX);
  GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);//GPIO的UART模式配置
  UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 115200,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART3_BASE);//使能UART0中断	
  UARTIntEnable(UART3_BASE,UART_INT_RX);//使能UART3接收中断		
  UARTIntRegister(UART3_BASE,UART3_IRQHandler);//UART3中断地址注册	
  IntPrioritySet(INT_UART3, USER_INT3);
}

/***********************************************************
@函数名：UART6_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口6数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART6_IRQHandler(void)
{		
  uint32_t flag = UARTIntStatus(UART6_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志		
  UARTIntClear(UART6_BASE,flag);//清除中断标志	
  while(UARTCharsAvail(UART6_BASE))//判断FIFO是否还有数据		
  {			
    RingBuf_Write(UARTCharGet(UART6_BASE),&COM6_Rx_Buf,FLOW_BUF_CNT);//往环形队列里面写数据		
  }
}

/***********************************************************
@函数名：USART6_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口6发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART6_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART6_BASE, *pui8Buffer++);
  }
}

/***********************************************************
@函数名：ConfigureUART6
@入口参数：无
@出口参数：无
功能描述：串口6配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART6(void)//串口6初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);//使能UART外设
  GPIOPinConfigure(GPIO_PD4_U6RX);//GPIO模式配置 PD4--RX PD5--TX 
  GPIOPinConfigure(GPIO_PD5_U6TX);
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);//GPIO的UART模式配置
  UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), 19200,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
  UARTFIFODisable(UART6_BASE);//使能UART0中断	
  UARTIntEnable(UART6_BASE,UART_INT_RX);//使能UART6接收中断		
  OpticalFlow_Init();//光流滤波参数初始化
  OpticalFlow_Is_Work=Config_Init_Uart();//光流传感器初始化
  UARTIntRegister(UART6_BASE,UART6_IRQHandler);//UART6中断地址注册	
  IntPrioritySet(INT_UART6, USER_INT5);
}

/***********************************************************
@函数名：UART7_IRQHandler
@入口参数：无
@出口参数：无
功能描述：串口7数据接收
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void UART7_IRQHandler(void)//UART2中断函数
{		
  uint32_t flag = UARTIntStatus(UART7_BASE,1);//获取中断标志 原始中断状态 屏蔽中断标志	
  UARTIntClear(UART7_BASE,flag);//清除中断标志		
  while(UARTCharsAvail(UART7_BASE))//判断FIFO是否还有数据			
  {			
    //输出得到的数据			
        //UARTCharPut(UART1_BASE,UARTCharGet(UART1_BASE));36
    //RingBuf_Write(UARTCharGet(UART7_BASE),&COM7_Rx_Buf,4);//往环形队列里面写数据
#if (Ground_Distance_Sensor==US100)	
		RingBuf_Write(UARTCharGet(UART7_BASE),&COM7_Rx_Buf,4);//往环形队列里面写数据
#else
		RingBuf_Write(UARTCharGet(UART7_BASE),&COM7_Rx_Buf,18);//往环形队列里面写数据	
#endif		
  }
}

/***********************************************************
@函数名：USART7_Send
@入口参数：uint8_t *pui8Buffer, uint32_t ui32Count
@出口参数：无
功能描述：串口7发送N个字节数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void USART7_Send(uint8_t *pui8Buffer, uint32_t ui32Count)//发送N个字节长度的数据
{
  while(ui32Count--)
  {
    UARTCharPut(UART7_BASE, *pui8Buffer++);
  }
}

/***********************************************************
@函数名：ConfigureUART7
@入口参数：无
@出口参数：无
功能描述：串口7配置
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ConfigureUART7(void)//串口7初始化
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//使能GPIO外设		
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);//使能UART外设
  GPIOPinConfigure(GPIO_PE0_U7RX);//GPIO模式配置 PE0--RX PE1--TX 
  GPIOPinConfigure(GPIO_PE1_U7TX);
  GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);//GPIO的UART模式配置
#if (Ground_Distance_Sensor==US100)	
  UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 9600,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));
#else
  UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), 115200,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));	
#endif
  UARTFIFODisable(UART7_BASE);//使能UART0中断	
  UARTIntEnable(UART7_BASE,UART_INT_RX);//使能UART0接收中断		
  UARTIntRegister(UART7_BASE,UART7_IRQHandler);//UART中断地址注册	
  IntPrioritySet(INT_UART7, USER_INT6);
}

/***********************************************************/
uint8_t data_to_send[50];//ANO地面站发送数据缓冲
uint8_t ANO_Send_PID_Flag[6]={0};//PID发送标志位
#define vs16 int16_t
#define vs32 int32_t
#define s16  int16_t
/***********************************************************
@函数名：ANO_DT_Data_Receive_Prepare
@入口参数：u8 data
@出口参数：无
功能描述：匿名地面站单个数据解析
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ANO_DT_Data_Receive_Prepare(u8 data)//ANO地面站数据解析
{
  static u8 RxBuffer[50];
  static u8 _data_len = 0,_data_cnt = 0;
  static u8 state = 0;
  if(state==0&&data==0xAA)//帧头1
  {
    state=1;
    RxBuffer[0]=data;
  }
  else if(state==1&&data==0xAF)//帧头2
  {
    state=2;
    RxBuffer[1]=data;
  }
  else if(state==2&&data<0XF1)//功能字节
  {
    state=3;
    RxBuffer[2]=data;
  }
  else if(state==3&&data<50)//有效数据长度
  {
    state = 4;
    RxBuffer[3]=data;
    _data_len = data;
    _data_cnt = 0;
  }
  else if(state==4&&_data_len>0)//数据接收
  {
    _data_len--;
    RxBuffer[4+_data_cnt++]=data;
    if(_data_len==0)
      state = 5;
  }
  else if(state==5)//校验和
  {
    state = 0;
    RxBuffer[4+_data_cnt]=data;
    ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);//数据解析
  }
  else state = 0;
}

/***********************************************************
@函数名：ANO_DT_Send_Check
@入口参数：无
@出口参数：无
功能描述：匿名地面站和校验函数
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
static void ANO_DT_Send_Check(u8 head, u8 check_sum)//ANO地面站和校验
{
  u8 sum = 0,i=0;
  data_to_send[0]=0xAA;
  data_to_send[1]=0xAA;
  data_to_send[2]=0xEF;
  data_to_send[3]=2;
  data_to_send[4]=head;
  data_to_send[5]=check_sum;
  for(i=0;i<6;i++)
    sum += data_to_send[i];
  data_to_send[6]=sum;
  USART1_Send(data_to_send, 7);
}

/***********************************************************
@函数名：ANO_DT_Data_Receive_Anl
@入口参数：无
@出口参数：无
功能描述：匿名地面站帧数据解析函数
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num)//ANO数据解析
{
  u8 sum = 0,i=0;
  for(i=0;i<(num-1);i++)  sum += *(data_buf+i);
  if(!(sum==*(data_buf+num-1)))    return; //判断sum
  if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))     return;//判断帧头
  if(*(data_buf+2)==0X01)
  {
    if(*(data_buf+4)==0X01) {;}
    if(*(data_buf+4)==0X02) {;}
    if(*(data_buf+4)==0X03) {;}
  }
  
  if(*(data_buf+2)==0X02)
  {
    if(*(data_buf+4)==0X01)		//读取当前PID参数
    {
      ANO_Send_PID_Flag[0]=1;
      ANO_Send_PID_Flag[1]=1;
      ANO_Send_PID_Flag[2]=1;
      ANO_Send_PID_Flag[3]=1;
      ANO_Send_PID_Flag[4]=1;
      ANO_Send_PID_Flag[5]=1;
      Bling_Set(&Light_1,1000,50,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_1,0);
      Bling_Set(&Light_2,1000,50,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);
      Bling_Set(&Light_3,1000,50,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,0);
    }
    if(*(data_buf+4)==0X02)
    {
      ;
    }
    if(*(data_buf+4)==0XA0)     //读取版本信息
    {
      ;
    }
    if(*(data_buf+4)==0XA1)     //恢复默认参数
    {
      Sort_PID_Flag=3;
      Bling_Set(&Light_1,1000,50,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_1,0);
      Bling_Set(&Light_2,1000,50,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);
      Bling_Set(&Light_3,1000,50,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,0);
    }
  }
  
  if(*(data_buf+2)==0X10)                             //接收PID1
  {
    Total_Controller.Roll_Gyro_Control.Kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
    Total_Controller.Roll_Gyro_Control.Ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
    Total_Controller.Roll_Gyro_Control.Kd  = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
    Total_Controller.Pitch_Gyro_Control.Kp   = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
    Total_Controller.Pitch_Gyro_Control.Ki   = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
    Total_Controller.Pitch_Gyro_Control.Kd   = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
    Total_Controller.Yaw_Gyro_Control.Kp    = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
    Total_Controller.Yaw_Gyro_Control.Ki    = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
    Total_Controller.Yaw_Gyro_Control.Kd    = 0.01*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
    ANO_DT_Send_Check(*(data_buf+2),sum);
  }
  if(*(data_buf+2)==0X11)                             //接收PID2
  {
    Total_Controller.Roll_Angle_Control.Kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
    Total_Controller.Roll_Angle_Control.Ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
    Total_Controller.Roll_Angle_Control.Kd  = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
    Total_Controller.Pitch_Angle_Control.Kp   = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
    Total_Controller.Pitch_Angle_Control.Ki   = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
    Total_Controller.Pitch_Angle_Control.Kd   = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
    Total_Controller.Yaw_Angle_Control.Kp    = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
    Total_Controller.Yaw_Angle_Control.Ki    = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
    Total_Controller.Yaw_Angle_Control.Kd    = 0.01*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
    ANO_DT_Send_Check(*(data_buf+2),sum); 
  }
  if(*(data_buf+2)==0X12)                             //接收PID3
  {
    Total_Controller.High_Speed_Control.Kp    = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
    Total_Controller.High_Speed_Control.Ki    = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
    Total_Controller.High_Speed_Control.Kd    = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
    Total_Controller.High_Position_Control.Kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
    Total_Controller.High_Position_Control.Ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
    Total_Controller.High_Position_Control.Kd = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
    Total_Controller.Latitude_Speed_Control.Kp= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
    Total_Controller.Latitude_Speed_Control.Ki= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
    Total_Controller.Latitude_Speed_Control.Kd= 0.01*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
    /***********************位置控制：位置、速度参数共用一组PID参数**********************************************************/
    Total_Controller.Longitude_Speed_Control.Kp=Total_Controller.Latitude_Speed_Control.Kp;
    Total_Controller.Longitude_Speed_Control.Ki=Total_Controller.Latitude_Speed_Control.Ki;
    Total_Controller.Longitude_Speed_Control.Kd=Total_Controller.Latitude_Speed_Control.Kd;
    ANO_DT_Send_Check(*(data_buf+2),sum); 
  }
  if(*(data_buf+2)==0X13)                             //接收PID4
  {
    Total_Controller.Latitude_Position_Control.Kp    = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
    Total_Controller.Latitude_Position_Control.Ki    = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
    Total_Controller.Latitude_Position_Control.Kd    = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
    Total_Controller.High_Acce_Control.Kp            = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
    Total_Controller.High_Acce_Control.Ki            = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
    Total_Controller.High_Acce_Control.Kd            = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
    /***********************位置控制：位置、速度参数共用一组PID参数**********************************************************/
    Total_Controller.Longitude_Position_Control.Kp=Total_Controller.Latitude_Position_Control.Kp;
    Total_Controller.Longitude_Position_Control.Ki=Total_Controller.Latitude_Position_Control.Ki;
    Total_Controller.Longitude_Position_Control.Kd=Total_Controller.Latitude_Position_Control.Kd;
    ANO_DT_Send_Check(*(data_buf+2),sum);
  }
  if(*(data_buf+2)==0X14)                             //接收PID5
  {
    Total_Controller.Optical_Position_Control.Kp = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
    Total_Controller.Optical_Position_Control.Ki = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
    Total_Controller.Optical_Position_Control.Kd = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
    Total_Controller.Optical_Speed_Control.Kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
    Total_Controller.Optical_Speed_Control.Ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
    Total_Controller.Optical_Speed_Control.Kd = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
    
    Total_Controller.SDK_Roll_Position_Control.Kp = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
    Total_Controller.SDK_Roll_Position_Control.Ki = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
    Total_Controller.SDK_Roll_Position_Control.Kd = 0.01*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
    Total_Controller.SDK_Pitch_Position_Control.Kp = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
    Total_Controller.SDK_Pitch_Position_Control.Ki = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
    Total_Controller.SDK_Pitch_Position_Control.Kd = 0.01*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );   
    ANO_DT_Send_Check(*(data_buf+2),sum);
  }
  if(*(data_buf+2)==0X15)                             //接收PID6
  {
    ANO_DT_Send_Check(*(data_buf+2),sum);
    Sort_PID_Cnt++;
    Sort_PID_Flag=1;
    Bling_Set(&Light_1,1000,50,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_1,0);
    Bling_Set(&Light_2,1000,50,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_2,0);
    Bling_Set(&Light_3,1000,50,0.5,0,GPIO_PORTF_BASE,GPIO_PIN_3,0);
  }
}

/***********************************************************
@函数名：ANO_Data_Send_Status
@入口参数：无
@出口参数：无
功能描述：匿名地面站发送姿态数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ANO_Data_Send_Status(void)//发送基本信息（姿态、锁定状态）
{
  u8 _cnt=0;
  vs16 _temp;
  vs32 _temp2;
  u8 sum = 0;
  u8 i;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x01;
  data_to_send[_cnt++]=0;
  
  _temp = (int)(Roll*100);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int)(Pitch*100);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int)(-Yaw_Temp*100);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  _temp2 = (vs32)(100*NamelessQuad.Position[_YAW]);//单位cm
  data_to_send[_cnt++]=BYTE3(_temp2);
  data_to_send[_cnt++]=BYTE2(_temp2);
  data_to_send[_cnt++]=BYTE1(_temp2);
  data_to_send[_cnt++]=BYTE0(_temp2);
  
  data_to_send[_cnt++]=0x01;//飞行模式
  data_to_send[_cnt++]=Controler_State;//上锁0、解锁1
  
  data_to_send[3] = _cnt-4;
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;
  USART1_Send(data_to_send, _cnt);
}

/***********************************************************
@函数名：ANO_DT_Send_Senser
@入口参数：s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,
s16 m_x,s16 m_y,s16 m_z6
@出口参数：无
功能描述：匿名地面站发送传感器原始数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)//发送传感器原始数字量
{
  u8 _cnt=0;
  vs16 _temp;
  u8 sum = 0;
  u8 i=0;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x02;
  data_to_send[_cnt++]=0;
  
  _temp = a_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = a_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = a_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  _temp = g_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = g_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = g_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  _temp = m_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = m_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = m_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  data_to_send[3] = _cnt-4;
  
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++] = sum;
  USART1_Send(data_to_send, _cnt);
}

/***********************************************************
@函数名：ANO_DT_Send_RCData
@入口参数：u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,
u16 aux3,u16 aux4,u16 aux5,u16 aux6
@出口参数：无
功能描述：匿名地面站发送遥控器各个通道数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6)//发送遥控器通道数据
{
  u8 _cnt=0;
  u8 i=0;
  u8 sum = 0;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x03;
  data_to_send[_cnt++]=0;
  data_to_send[_cnt++]=BYTE1(thr);
  data_to_send[_cnt++]=BYTE0(thr);
  data_to_send[_cnt++]=BYTE1(yaw);
  data_to_send[_cnt++]=BYTE0(yaw);
  data_to_send[_cnt++]=BYTE1(rol);
  data_to_send[_cnt++]=BYTE0(rol);
  data_to_send[_cnt++]=BYTE1(pit);
  data_to_send[_cnt++]=BYTE0(pit);
  data_to_send[_cnt++]=BYTE1(aux1);
  data_to_send[_cnt++]=BYTE0(aux1);
  data_to_send[_cnt++]=BYTE1(aux2);
  data_to_send[_cnt++]=BYTE0(aux2);
  data_to_send[_cnt++]=BYTE1(aux3);
  data_to_send[_cnt++]=BYTE0(aux3);
  data_to_send[_cnt++]=BYTE1(aux4);
  data_to_send[_cnt++]=BYTE0(aux4);
  data_to_send[_cnt++]=BYTE1(aux5);
  data_to_send[_cnt++]=BYTE0(aux5);
  data_to_send[_cnt++]=BYTE1(aux6);
  data_to_send[_cnt++]=BYTE0(aux6);
  
  data_to_send[3] = _cnt-4;
  
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  
  data_to_send[_cnt++]=sum;
  USART1_Send(data_to_send, _cnt);
}

/***********************************************************
@函数名：ANO_DT_Send_GPSData
@入口参数：无
@出口参数：无
功能描述：匿名地面站发送GPS经纬度数据
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ANO_DT_Send_GPSData(u8 Fixstate,
                         u8 GPS_Num,
                         u32 log,
                         u32 lat,
                         int16 gps_head)//发送GPS基本信息
{
  u8 sum = 0;
  u8 _cnt=0;
  u8 i=0;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x04;
  data_to_send[_cnt++]=0;
  data_to_send[_cnt++]=Fixstate;
  data_to_send[_cnt++]=GPS_Num;
  
  data_to_send[_cnt++]=BYTE3(log);
  data_to_send[_cnt++]=BYTE2(log);
  data_to_send[_cnt++]=BYTE1(log);
  data_to_send[_cnt++]=BYTE0(log);
  
  data_to_send[_cnt++]=BYTE3(lat);
  data_to_send[_cnt++]=BYTE2(lat);
  data_to_send[_cnt++]=BYTE1(lat);
  data_to_send[_cnt++]=BYTE0(lat);
  
  data_to_send[_cnt++]=BYTE1(gps_head);
  data_to_send[_cnt++]=BYTE0(gps_head);
  
  data_to_send[3] = _cnt-4;
  
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  
  data_to_send[_cnt++]=sum;
  USART1_Send(data_to_send, _cnt);
}

/***********************************************************
@函数名：ANO_SEND_StateMachine
@入口参数：无
@出口参数：无
功能描述：匿名地面站发送PID数据状态机
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)//发送PID数据
{
  u8 _cnt=0;
  u8 sum = 0,i=0;
  int16_t _temp;
  
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x10+group-1;
  data_to_send[_cnt++]=0;
  
  
  _temp = (int16_t)(p1_p * 1000);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int16_t)(p1_i  * 1000);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int16_t)(p1_d  * 100);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int16_t)(p2_p  * 1000);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int16_t)(p2_i  * 1000);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int16_t)(p2_d * 100);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int16_t)(p3_p  * 1000);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int16_t)(p3_i  * 1000);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = (int16_t)(p3_d * 100);
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  data_to_send[3] = _cnt-4;
  
  
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  
  data_to_send[_cnt++]=sum;
  USART1_Send(data_to_send, _cnt);
}

/***********************************************************
@函数名：ANO_SEND_StateMachine
@入口参数：无
@出口参数：无
功能描述：匿名地面站，数据发送函数，发送了姿态状态、原始传感
器数字量、遥控器通道、GPS经纬度、PID数据等，需要按地面站帧协
议来发送
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void ANO_SEND_StateMachine(void)//各组数据循环发送
{
	static int16_t ANO_Cnt=0;
  ANO_Cnt++;
  if(ANO_Cnt==1)
  {
    ANO_Data_Send_Status();
  }
  else if(ANO_Cnt==2)
  {
    ANO_DT_Send_Senser((int16_t)WP_Sensor.accel_raw.x,(int16_t)WP_Sensor.accel_raw.y,(int16_t)WP_Sensor.accel_raw.z,
                       (int16_t)WP_Sensor.gyro_raw.x,(int16_t)WP_Sensor.gyro_raw.y,(int16_t)WP_Sensor.gyro_raw.z,
                       (int16_t)WP_Sensor.mag_raw.x,(int16_t)WP_Sensor.mag_raw.y,(int16_t)WP_Sensor.mag_raw.z);
  }
  else if(ANO_Cnt==3)
  {
    ANO_DT_Send_RCData(PPM_Databuf[2],PPM_Databuf[3],
                       PPM_Databuf[0],PPM_Databuf[1],
                       PPM_Databuf[4],PPM_Databuf[5],
                       PPM_Databuf[6],PPM_Databuf[7],0,0);
  }
  else if(ANO_Cnt==4
          &&ANO_Send_PID_Flag[0]==0
            &&ANO_Send_PID_Flag[1]==0
              &&ANO_Send_PID_Flag[2]==0
                &&ANO_Send_PID_Flag[3]==0
                  &&ANO_Send_PID_Flag[4]==0
                    &&ANO_Send_PID_Flag[5]==0)//提前终止发送队列
  {
    ANO_DT_Send_GPSData(1,GPS_Sate_Num,Longitude_Origion,Latitude_Origion,10);
    ANO_Cnt=0;
  }
  else if(ANO_Cnt==4)
  {
    ANO_DT_Send_GPSData(1,GPS_Sate_Num,Longitude_Origion,Latitude_Origion,10);
  }
  else if(ANO_Cnt==5&&ANO_Send_PID_Flag[0]==1)
  {
    ANO_DT_Send_PID(1,Total_Controller.Roll_Gyro_Control.Kp,
                    Total_Controller.Roll_Gyro_Control.Ki,
                    Total_Controller.Roll_Gyro_Control.Kd,
                    Total_Controller.Pitch_Gyro_Control.Kp,
                    Total_Controller.Pitch_Gyro_Control.Ki,
                    Total_Controller.Pitch_Gyro_Control.Kd,
                    Total_Controller.Yaw_Gyro_Control.Kp,
                    Total_Controller.Yaw_Gyro_Control.Ki,
                    Total_Controller.Yaw_Gyro_Control.Kd);
    ANO_Send_PID_Flag[0]=0;
  }
  else if(ANO_Cnt==6&&ANO_Send_PID_Flag[1]==1)
  {
    ANO_DT_Send_PID(2,Total_Controller.Roll_Angle_Control.Kp,
                    Total_Controller.Roll_Angle_Control.Ki,
                    Total_Controller.Roll_Angle_Control.Kd,
                    Total_Controller.Pitch_Angle_Control.Kp,
                    Total_Controller.Pitch_Angle_Control.Ki,
                    Total_Controller.Pitch_Angle_Control.Kd,
                    Total_Controller.Yaw_Angle_Control.Kp,
                    Total_Controller.Yaw_Angle_Control.Ki,
                    Total_Controller.Yaw_Angle_Control.Kd);
    ANO_Send_PID_Flag[1]=0;
  }
  else if(ANO_Cnt==7&&ANO_Send_PID_Flag[2]==1)
  {
    ANO_DT_Send_PID(3,Total_Controller.High_Speed_Control.Kp,
                    Total_Controller.High_Speed_Control.Ki,
                    Total_Controller.High_Speed_Control.Kd,
                    Total_Controller.High_Position_Control.Kp,
                    Total_Controller.High_Position_Control.Ki,
                    Total_Controller.High_Position_Control.Kd,
                    Total_Controller.Latitude_Speed_Control.Kp,
                    Total_Controller.Latitude_Speed_Control.Ki,
                    Total_Controller.Latitude_Speed_Control.Kd);
    ANO_Send_PID_Flag[2]=0;
  }
  else if(ANO_Cnt==8&&ANO_Send_PID_Flag[3]==1)
  {
    ANO_DT_Send_PID(4,Total_Controller.Latitude_Position_Control.Kp,
                    Total_Controller.Latitude_Position_Control.Ki,
                    Total_Controller.Latitude_Position_Control.Kd,
                    Total_Controller.High_Acce_Control.Kp,
                    Total_Controller.High_Acce_Control.Ki,
                    Total_Controller.High_Acce_Control.Kd,
                    0,0,0);
    ANO_Send_PID_Flag[3]=0;
  }
  else if(ANO_Cnt==9&&ANO_Send_PID_Flag[4]==1)
  {
    ANO_DT_Send_PID(5,Total_Controller.Optical_Position_Control.Kp
                    ,Total_Controller.Optical_Position_Control.Ki
                      ,Total_Controller.Optical_Position_Control.Kd
                        ,Total_Controller.Optical_Speed_Control.Kp
                          ,Total_Controller.Optical_Speed_Control.Ki
                            ,Total_Controller.Optical_Speed_Control.Kd
                              ,Total_Controller.SDK_Roll_Position_Control.Kp
                                ,Total_Controller.SDK_Roll_Position_Control.Ki
                                  ,Total_Controller.SDK_Roll_Position_Control.Kd);
    ANO_Send_PID_Flag[4]=0;
  }
  else if(ANO_Cnt==10&&ANO_Send_PID_Flag[5]==1)
  {
    ANO_DT_Send_PID(6,0,0,0,
                    0,0,0,
                    0,0,0);
    ANO_Send_PID_Flag[5]=0;
    ANO_Cnt=0;
  }
}

/***********************************************************
@函数名：Vcan_Send
@入口参数：无
@出口参数：无
功能描述：山外数据发送函数，默认发送8个通道，数据类型为float
，每个通道数据可以自己定义
@作者：无名小哥
@日期：2019年01月27日
*************************************************************/
void Vcan_Send(void)//山外地面站发送
{
  static float DataBuf[8];	

	DataBuf[0]=Pitch;//惯导高度
  DataBuf[1]=Roll;//惯导速度
  DataBuf[2]=Yaw;;//惯导加速度
  DataBuf[3]=Pitch_Observation;//气压计高度
  DataBuf[4]=Roll_Observation;
  DataBuf[5]=Yaw_Observation;
  DataBuf[6]=0;
  DataBuf[7]=0;
/*
  DataBuf[0]=NamelessQuad.Position[_YAW];//惯导高度
  DataBuf[1]=NamelessQuad.Speed[_YAW];//惯导速度
  DataBuf[2]=observation_div;//NamelessQuad.Acceleration[_YAW];;//惯导加速度
  DataBuf[3]=Observation_Altitude;//气压计高度
	DataBuf[4]=observation_acc;
  DataBuf[5]=NamelessQuad.Vel_History[_YAW][30];
  DataBuf[6]=WP_Sensor.baro_altitude_div;
	DataBuf[7]=NamelessQuad.Acceleration[_YAW];
	

	DataBuf[0]=Accel_X_Butter_Filter.output[Accel_X_Butter_Filter.N-1];;
  DataBuf[1]=Accel_Y_Butter_Filter.output[Accel_Y_Butter_Filter.N-1];;
  DataBuf[2]=Accel_Z_Butter_Filter.output[Accel_Z_Butter_Filter.N-1];;
  DataBuf[3]=accel.x;
	DataBuf[4]=accel.y;
  DataBuf[5]=accel.z;
	
	//PID期望反馈
  DataBuf[0]=Total_Controller.Pitch_Angle_Control.Expect;
  DataBuf[1]=Total_Controller.Pitch_Gyro_Control.Expect;
  DataBuf[2]=Total_Controller.Pitch_Angle_Control.FeedBack;
  DataBuf[3]=Total_Controller.Pitch_Gyro_Control.FeedBack;
  DataBuf[4]=Total_Controller.Roll_Angle_Control.Expect;
  DataBuf[5]=Total_Controller.Roll_Gyro_Control.Expect;
  DataBuf[6]=Total_Controller.Roll_Angle_Control.FeedBack;
  DataBuf[7]=Total_Controller.Roll_Gyro_Control.FeedBack;
  
  DataBuf[0]=GPS_Vel_Div.E;//惯导高度
  DataBuf[1]=GPS_Vel_Div.N;//惯导速度
  DataBuf[2]=NamelessQuad.Acceleration[_PITCH];;//惯导加速度
  DataBuf[3]=NamelessQuad.Acceleration[_ROLL];;//惯导加速度
  DataBuf[4]=0;//惯导高度;
  DataBuf[5]=0;
  DataBuf[6]=0;
  DataBuf[7]=0;   
  
  //2      
  
  DataBuf[0]=gyro_filter_data.y;
  DataBuf[1]=opt_gyro_filter_data.y;
  DataBuf[2]=OpticalFlow_Speed.y;
  DataBuf[3]=OpticalFlow_SINS.Speed[_ROLL];
  DataBuf[4]=gyro_filter_data.y;
  DataBuf[5]=opt_gyro_data.y;
  DataBuf[6]=opt_origin_data.pixel_flow_y_integral;
  DataBuf[7]=opt_filter_data.y;
  

  DataBuf[0]=gyro_filter_data.x;//Pitch X
  DataBuf[1]=gyro_filter_data.y;//Roll R
  DataBuf[2]=opt_gyro_data.x;
  DataBuf[3]=opt_gyro_data.y;
  DataBuf[4]=opt_filter_data.x;
  DataBuf[5]=opt_filter_data.y;
  DataBuf[6]=opt_gyro_filter_data.x;
  DataBuf[7]=opt_gyro_filter_data.y;
  
  DataBuf[0]=OpticalFlow_SINS.Position[_PITCH];
  DataBuf[1]=OpticalFlow_SINS.Speed[_PITCH];
  DataBuf[2]=OpticalFlow_SINS.Position[_ROLL];
  DataBuf[3]=OpticalFlow_SINS.Speed[_ROLL];
  DataBuf[4]=OpticalFlow_Position.x;
  DataBuf[5]=OpticalFlow_Position.y;
  DataBuf[6]=NamelessQuad.Position[_YAW];//惯导高度
  DataBuf[7]=0;

  
  DataBuf[0]=PPM_Databuf[0];
  DataBuf[1]=PPM_Databuf[1];
  DataBuf[2]=PPM_Databuf[2];
  DataBuf[3]=PPM_Databuf[3];
  DataBuf[4]=PPM_LPF_Databuf[0];
  DataBuf[5]=PPM_LPF_Databuf[1];
  DataBuf[6]=Total_Controller.Pitch_Gyro_Control.Dis_Err;
  DataBuf[7]=Total_Controller.Pitch_Gyro_Control.Dis_Error_History[0];
  */
  //3
   /*
  DataBuf[0]=NamelessQuad.Position[_PITCH];
  DataBuf[1]=NamelessQuad.Speed[_PITCH];
  DataBuf[2]=GPS_Vel.E;
  DataBuf[3]=Earth_Frame_To_XYZ.E;
  DataBuf[4]=NamelessQuad.Position[_ROLL];
  DataBuf[5]=NamelessQuad.Speed[_ROLL];
  DataBuf[6]=GPS_Vel.N;
  DataBuf[7]=Earth_Frame_To_XYZ.N;
 
  DataBuf[0]=NamelessQuad.Position[_PITCH];
  DataBuf[1]=0;
  DataBuf[2]=NamelessQuad.Position[_ROLL];
  DataBuf[3]=0;
  DataBuf[4]=GPS_Ground_Speed;
  DataBuf[5]=sqrt(NamelessQuad.Speed[_PITCH]*NamelessQuad.Speed[_PITCH]+NamelessQuad.Speed[_ROLL]*NamelessQuad.Speed[_ROLL]);
  DataBuf[6]=0;
  DataBuf[7]=NamelessQuad.Position[_YAW];//惯导高度
  */
  wust_sendware((unsigned char *)DataBuf,sizeof(DataBuf));
}




