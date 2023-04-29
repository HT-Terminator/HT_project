#ifndef __USART_H
#define __USART_H 			   
#include "ht32f5xxxx_usart.h"

//cloud
#include "stdio.h"	
#include "stdarg.h"		//������Ҫ��ͷ�ļ� 
#include "string.h"     //������Ҫ��ͷ�ļ�
#include "MQTT.h"
#include "Cloud.h"
//cloud

//#define USART_GPIO_GROUP             (GPIO_PA)
//#define USART_TX_PIN                 (GPIO_PIN_14)
//#define USART_RX_PIN                 (GPIO_PIN_15)
//#define USART_AFIO_MODE              (AFIO_FUN_USART_UART) //ģʽ���ã�Ĭ��ģʽ��AFIO_MODE_DEFAULT ��AFIO_MODE_1~15��Ӧģʽ1~15
//#define COM1_PORT                    (HT_USART1)
//#define COM1_IRQn                    (USART1_IRQn)

#define USART_GPIO_GROUP             (GPIO_PA)
#define USART_TX_PIN                 (GPIO_PIN_8)
#define USART_RX_PIN                 (GPIO_PIN_1)
#define USART_AFIO_MODE              (AFIO_FUN_USART_UART) //ģʽ���ã�Ĭ��ģʽ��AFIO_MODE_DEFAULT ��AFIO_MODE_1~15��Ӧģʽ1~15
#define COM0_PORT                    (HT_USART0)
#define COM0_IRQn                    (USART0_IRQn)

//cloud
extern unsigned char flag_mqqt;
#define USART1_RX_ENABLE     1      //�Ƿ������չ���  1������  0���ر�
#define USART1_TXBUFF_SIZE   1024   //���崮��2 ���ͻ�������С 1024�ֽ�

#if  USART1_RX_ENABLE                          //���ʹ�ܽ��չ���
#define USART1_RXBUFF_SIZE   1024              //���崮��2 ���ջ�������С 1024�ֽ�
extern char Usart1_RxCompleted ;               //�ƶ˽�����ɱ�־
extern unsigned int Usart1_RxCounter;          //�ƶ˽�������
extern char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //�ƶ˽�������
#endif

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART0_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define USART0_TXBUFF_SIZE   256    //���崮��1 ���ͻ�������С 256�ֽ�ine USART1_TXBUFF_SIZE   1024   //���崮��2 ���ͻ�������С 1024�ֽ�

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

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

