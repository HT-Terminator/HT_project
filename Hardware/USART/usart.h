#ifndef __USART_H
#define __USART_H 			   
#include "ht32f5xxxx_usart.h"

//#define USART_GPIO_GROUP             (GPIO_PA)
//#define USART_TX_PIN                 (GPIO_PIN_14)
//#define USART_RX_PIN                 (GPIO_PIN_15)
//#define USART_AFIO_MODE              (AFIO_FUN_USART_UART) //模式设置，默认模式：AFIO_MODE_DEFAULT ，AFIO_MODE_1~15对应模式1~15
//#define COM1_PORT                    (HT_USART1)
//#define COM1_IRQn                    (USART1_IRQn)

#define USART_GPIO_GROUP             (GPIO_PA)
#define USART_TX_PIN                 (GPIO_PIN_2)
#define USART_RX_PIN                 (GPIO_PIN_3)
#define USART_AFIO_MODE              (AFIO_FUN_USART_UART) //模式设置，默认模式：AFIO_MODE_DEFAULT ，AFIO_MODE_1~15对应模式1~15
#define COM0_PORT                    (HT_USART0)
#define COM0_IRQn                    (USART0_IRQn)

void USART_Configuration(void);
void COM0_IRQHandler(void);
void Usart_Sendbyte(HT_USART_TypeDef* USARTx, u8 data);
void Usart_SendArray(HT_USART_TypeDef* USARTx, u8 *array,u8 num);
void Usart_SendStr(HT_USART_TypeDef* USARTx, uint8_t *str);
void Ultrasonic_Ranging(void);

extern unsigned char usart0_flag;

#endif

