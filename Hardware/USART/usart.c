#include "usart.h"
#include "ht32f5xxxx_gpio.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "delay.h"

unsigned char usart0_flag = 0;
uint8_t	distance_temperature[3]	= {0};	//[0]Ϊ���߰�λ��[1]Ϊ���ڰ�λ��[2]Ϊ��������
uint8_t	RXCounter =	0;	//ȫ�ֱ�������¼�������ݵ�˳��
uint8_t tem = 0;				//ȫ�ֱ�������¼�¶ȣ���λ����
uint16_t distance = 0;		//ȫ�ֱ�������¼���룬��λ��mm

void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	
	CKCU_PeripClockConfig_TypeDef CKCUClock= {{0}};
	CKCUClock.Bit.AFIO = 1;
	COM0_CLK(CKCUClock)= 1;  //����ʱ��
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
		
	AFIO_GPxConfig(GPIO_PA, USART_TX_PIN, AFIO_FUN_USART_UART);
	AFIO_GPxConfig(GPIO_PB, USART_RX_PIN, AFIO_FUN_USART_UART);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
	USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
	USART_InitStructure.USART_Parity = USART_PARITY_NO;
	USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	USART_Init(COM0_PORT, &USART_InitStructure);

	// ʹ�� COM1_PORT  ���ͺͽ��� 
	USART_TxCmd(COM0_PORT, ENABLE);
	USART_RxCmd(COM0_PORT, ENABLE);
	
	//�ж�����    
	NVIC_EnableIRQ(COM0_IRQn);

	USART_IntConfig(COM0_PORT, USART_INT_RXDR  , ENABLE);
}


/**************************ʵ�ֺ���********************************************
����˵���������жϷ�����

*******************************************************************************/ 
void USART0_IRQHandler(void)
{
	u32 data;
	
	if( USART_GetFlagStatus(COM0_PORT, USART_FLAG_RXDR ) )         //�����ж�
	{
		data = USART_ReceiveData(COM0_PORT);
		distance_temperature[RXCounter++] = data;
	}
}

/**************************ʵ�ֺ���********************************************
����˵��������һ���ֽ�

*******************************************************************************/ 
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data)
{
	USART_SendData(USARTx, data);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) == RESET);		
}
 
/**************************ʵ�ֺ���********************************************
����˵������������

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
 /**************************ʵ�ֺ���********************************************
����˵���������ַ���

*******************************************************************************/ 

void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str)
{
	uint8_t i;
	for(i = 0;str[i] != '\0';i++)
	{
		Usart_Sendbyte(USARTx,str[i]);
	}
}

void clear_buff2(void)//����1����������
{
	uint8_t i = 0;
	for(i=0;i<3;i++)
	{
		distance_temperature[i] = 0;
		RXCounter = 0;		//��¼�������ݵ�˳����0
	}
}


void Ultrasonic_Ranging(void)
{
	uint8_t	ranging_signal = 0x55;	
	uint8_t	temperature_signal = 0x50;
	//��USART1������ģ�鷢�Ͳ���źţ�0x55��
	Usart_Sendbyte(COM0_PORT,ranging_signal);
//	SendByte_USART1(ranging_signal);
	delay_ms(100);
	//��USART1������ģ�鷢�Ͳ����źţ�0x50��
	Usart_Sendbyte(COM0_PORT,temperature_signal);
//	SendByte_USART1(temperature_signal);
	delay_ms(100);
	distance = distance_temperature[0]*256+distance_temperature[1];	//ʵ�ʾ���
	tem = distance_temperature[2]-45;	//ʵ���¶�

	clear_buff2();
}
