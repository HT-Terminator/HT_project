#ifndef __HX711_H
#define __HX711_H

#include "ht32f5xxxx_01.h"

#define HX711_SCK_HIGH_1() 	GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_11)		// GPIO = HIGH	// PB0
#define HX711_SCK_LOW_1() 	GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_11)		// GPIO = LOW	// PB0
#define HX711_DOUT_HIGH_1()	GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_1)		// GPIO = HIGH	// PB1
#define HX711_DOUT_LOW_1() 	GPIO_ClearOutBits(HT_GPIOD, GPIO_PIN_1)		// GPIO = LOW	// PB1
#define HX711_DOUT_1				GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_1)

//#define HX711_SCK_HIGH_2() 	GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_2)		// GPIO = HIGH	// PB2
//#define HX711_SCK_LOW_2() 	GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_2)		// GPIO = LOW	// PB2
//#define HX711_DOUT_HIGH_2()	GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_3)		// GPIO = HIGH	// PB3
//#define HX711_DOUT_LOW_2() 	GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_3)		// GPIO = LOW	// PB3
//#define HX711_DOUT_2		GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_3)

//#define HX711_SCK_HIGH_3() 	GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_4)		// GPIO = HIGH	// PB4
//#define HX711_SCK_LOW_3() 	GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_4)		// GPIO = LOW	// PB4
//#define HX711_DOUT_HIGH_3()	GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_5)		// GPIO = HIGH	// PB5
//#define HX711_DOUT_LOW_3() 	GPIO_ClearOutBits(HT_GPIOB, GPIO_PIN_5)		// GPIO = LOW	// PB5
//#define HX711_DOUT_3		GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_5)

extern void HX711_Init(void);
extern u32 HX711_Read_1(void);
extern u32 HX711_Read_2(void);
extern u32 HX711_Read_3(void);
extern void Get_Basic(void);
extern void Get_Weight(void);

extern s32 Weight_Medicine_1;
extern s32 Weight_Medicine_2;
extern s32 Weight_Medicine_3;
extern u8 Flag_Error;

#endif
