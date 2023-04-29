#include "usart.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "delay.h"
#include "timdelay.h"

//cloud
unsigned char flag_mqqt = 0;
#if  USART1_RX_ENABLE                   //如果使能接收功能
char Usart1_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart1_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   	
#endif

#if EN_USART0_RX   //如果使能了接收
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记
#endif
//cloud

unsigned char usart0_flag = 0;
uint8_t	distance_temperature[3]	= {0};	//[0]为测距高八位，[1]为测距第八位，[2]为测温数据
uint8_t	RXCounter =	0;	//全局变量，记录接收数据的顺序
uint8_t tem = 0;				//全局变量，记录温度，单位：度
uint16_t distance = 0;		//全局变量，记录距离，单位：mm

uint8_t	ranging_signal = 0x55;	
uint8_t	temperature_signal = 0x50;

void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	CKCU_PeripClockConfig_TypeDef CKCUClock= {{0}};
	CKCUClock.Bit.AFIO = 1;
	COM0_CLK(CKCUClock)= 1;  //开启时钟
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
		
	AFIO_GPxConfig(GPIO_PA, USART_TX_PIN, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PB, USART_RX_PIN, AFIO_FUN_USART_UART);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(COM0_PORT, &USART_InitStructure);

	// 使能 COM1_PORT  发送和接收 
	USART_TxCmd(COM0_PORT, ENABLE);
	USART_RxCmd(COM0_PORT, ENABLE);
	
	//中断设置    
	NVIC_EnableIRQ(COM0_IRQn);

	USART_IntConfig(COM0_PORT, USART_INT_RXDR  , ENABLE);
}

void UART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	CKCU_PeripClockConfig_TypeDef CKCUClock= {{0}};
	CKCUClock.Bit.AFIO = 1;
//	COM1_CLK(CKCUClock)= 1;  //开启时钟
	CKCUClock.Bit.UART1 = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
		
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_12, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PC, GPIO_PIN_13, AFIO_FUN_USART_UART);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(HT_UART1, &USART_InitStructure);

	// 使能 COM1_PORT  发送和接收 
	USART_TxCmd(HT_UART1, ENABLE);
	USART_RxCmd(HT_UART1, ENABLE);
	
	//中断设置    
	NVIC_EnableIRQ(UART1_IRQn);

	USART_IntConfig(HT_UART1, USART_INT_RXDR  , ENABLE);
}

/**************************实现函数********************************************
函数说明：接收中断服务函数

*******************************************************************************/ 
void USART0_IRQHandler(void)
{
	u32 data;
	
	if( USART_GetFlagStatus(COM0_PORT, USART_FLAG_RXDR ) )         //接收中断
	{
		data = USART_ReceiveData(COM0_PORT);
		distance_temperature[RXCounter++] = data;
	}
}

void UART1_IRQHandler(void)
{
	if( USART_GetFlagStatus(HT_UART1, USART_FLAG_RXDR ) )         //接收中断
	{  //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
		if(Connect_flag==0)
		{                                //如果Connect_flag等于0，当前还没有连接服务器，处于指令配置状态
			if(HT_UART1->DR)
			{                                 //处于指令配置状态时，非零值才保存到缓冲区	
				Usart1_RxBuff[Usart1_RxCounter]=HT_UART1->DR; //保存到缓冲区	
				Usart1_RxCounter ++;                        //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
			}
		}
		else
		{		                                        //反之Connect_flag等于1，连接上服务器了	
			Usart1_RxBuff[Usart1_RxCounter] = HT_UART1->DR;   //把接收到的数据保存到Usart2_RxBuff中				
			if(Usart1_RxCounter == 0)
			{    					//如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支				
//				TM_Cmd(HTCFG_TM_PORT1,ENABLE);
				flag_mqqt = 1;
			}
			else
			{                        					//else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
//				TM_SetCounter(HTCFG_TM_PORT1,0);  
				sctm_mqtt_num = 0;
			}
			Usart1_RxCounter ++;         				    //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
		}
	}
}

/**************************实现函数********************************************
函数说明：发送一个字节

*******************************************************************************/ 
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data)
{
	USART_SendData(USARTx, data);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);		
}
 
/**************************实现函数********************************************
函数说明：发送数组

*******************************************************************************/ 
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num)
{
	u8 i;
	for( i = 0;i < num;i++)
	{
		Usart_Sendbyte(USARTx,*array);
		array++;
	}
}
 /**************************实现函数********************************************
函数说明：发送字符串

*******************************************************************************/ 

void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Usart_Sendbyte(USARTx,str[i]);
	}
}

void clear_buff2(void)//串口1缓存清理函数
{
	uint8_t i = 0;
	for(i=0;i<3;i++)
	{
		distance_temperature[i] = 0;
		RXCounter = 0;		//记录接收数据的顺序清0
	}
}


void Ultrasonic_Ranging(void)
{
	//让USART1向超声波模块发送测距信号（0x55）
	if(sctm_usart0_100ms > 100*2 && usart0_flag == 0)
	{
		Usart_Sendbyte(COM0_PORT,ranging_signal);
		usart0_flag = 1;
		sctm_usart0_100ms = 0;
	}
	//delay_ms(100);
	//让USART1向超声波模块发送测温信号（0x50）
	if(sctm_usart0_100ms > 100*2 && usart0_flag == 1)
	{
//		Usart_Sendbyte(COM0_PORT,temperature_signal);
		usart0_flag = 0;
		sctm_usart0_100ms = 0;
//		delay_ms(100);
		distance = distance_temperature[0]*256+distance_temperature[1];	//实际距离
	  tem = distance_temperature[2]-45;	//实际温度
		clear_buff2();
	}
	
	
}

//cloud
__align(8) char USART0_TxBuff[USART0_TXBUFF_SIZE];  
//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口1打印函数
//  @param      char* fmt,...  格式化输出字符串和参数
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void DEBUG_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART0_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)USART0_TxBuff);		
	while((HT_USART0->SR&0X80)==0);
	for(i = 0;i < length;i ++)
	{			
		HT_USART0->DR = USART0_TxBuff[i];
		while((HT_USART0->SR&0X80)==0);	
	}	
}

__align(8) char USART1_TxBuff[USART1_TXBUFF_SIZE];  
//-------------------------------------------------------------------------------------------------------------------
//  @brief      云端发送函数
//  @param      char* fmt,...  格式化输出字符串和参数
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void Cloud_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART1_TxBuff,fmt,ap);
	va_end(ap);
	
	length=strlen((const char*)USART1_TxBuff);		
	while((HT_UART1->SR&0X80)==0);
	for(i = 0;i < length;i ++)
	{			
		HT_UART1->DR = USART1_TxBuff[i];
		while((HT_UART1->SR&0X80)==0);
	}	
}
//cloud

