#include "usart.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "delay.h"
#include "timdelay.h"

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
