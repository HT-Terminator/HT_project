#ifndef __MATRIXKEY_H__
#define __MATRIXKEY_H__

//#define P1_0	GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_3)
//#define P1_1	GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_9)
//#define P1_2	GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_8)
//#define P1_3	GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_7)

#define P1_SET()		GPIO_WriteOutBits(HT_GPIOA, GPIO_PIN_15, SET);\
						GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, SET);\
						GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, SET);

#define P1_0_RESET()	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_3, RESET);
#define P1_1_RESET()	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_9, RESET);
#define P1_2_RESET()	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_8, RESET);
#define P1_3_RESET()	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_7, RESET);

#define P1_4			GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_15)
#define P1_5 			GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_3)
#define P1_6 			GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_5)
#define P1_7 			GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_15)

#define P1_11 			GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_11)

extern unsigned char State;
extern unsigned char sale_state;
extern unsigned int  RoomNum ;
extern unsigned int  WeightDelivey;
extern unsigned int  WeightSale;

void GPIO_MatrixKey_Configuration(void);
unsigned char MatrixKey(void);

void GPIO_Key_Configuration(void);
void read_key(void);
void KeyProcess(unsigned char KeyNum);

#endif
