#ifndef __USART_H
#define __USART_H 			   
#include "ht32f5xxxx_usart.h"

//cloud
#include "stdio.h"	
#include "stdarg.h"		//包含需要的头文件 
#include "string.h"     //包含需要的头文件
#include "MQTT.h"
#include "Cloud.h"
//cloud

//#define USART_GPIO_GROUP             (GPIO_PA)
//#define USART_TX_PIN                 (GPIO_PIN_14)
//#define USART_RX_PIN                 (GPIO_PIN_15)
//#define USART_AFIO_MODE              (AFIO_FUN_USART_UART) //模式设置，默认模式：AFIO_MODE_DEFAULT ，AFIO_MODE_1~15对应模式1~15
//#define COM1_PORT                    (HT_USART1)
//#define COM1_IRQn                    (USART1_IRQn)

#define USART_GPIO_GROUP             (GPIO_PA)
#define USART_TX_PIN                 (GPIO_PIN_8)
#define USART_RX_PIN                 (GPIO_PIN_1)
#define USART_AFIO_MODE              (AFIO_FUN_USART_UART) //模式设置，默认模式：AFIO_MODE_DEFAULT ，AFIO_MODE_1~15对应模式1~15
#define COM0_PORT                    (HT_USART0)
#define COM0_IRQn                    (USART0_IRQn)

//cloud
extern unsigned char flag_mqqt;
#define USART1_RX_ENABLE     1      //是否开启接收功能  1：开启  0：关闭
#define USART1_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节

#if  USART1_RX_ENABLE                          //如果使能接收功能
#define USART1_RXBUFF_SIZE   1024              //定义串口2 接收缓冲区大小 1024字节
extern char Usart1_RxCompleted ;               //云端接收完成标志
extern unsigned int Usart1_RxCounter;          //云端接收索引
extern char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //云端接收数组
#endif

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART0_RX 			1		//使能（1）/禁止（0）串口1接收
#define USART0_TXBUFF_SIZE   256    //定义串口1 发送缓冲区大小 256字节ine USART1_TXBUFF_SIZE   1024   //定义串口2 发送缓冲区大小 1024字节

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

//cloud

void USART_Configuration(void);
void UART1_Configuration(void);
void COM0_IRQHandler(void);
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data);
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num);
void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str);
void Ultrasonic_Ranging(void);

//cloud
void DEBUG_printf(char* fmt,...);
void Cloud_printf(char* fmt,...); 
//cloud

extern unsigned char usart0_flag;
extern uint16_t distance;

#endif

