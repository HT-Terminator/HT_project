#ifndef __HX711_H
#define __HX711_H

#include "ht32f5xxxx_01.h"

#define HX711_SCK_HIGH() 	GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_11)		// GPIO = HIGH	// PC11
#define HX711_SCK_LOW() 	GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_11)	// GPIO = LOW	// PC11
#define HX711_DOUT_HIGH()	GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_1)		// GPIO = HIGH	// PD1
#define HX711_DOUT_LOW() 	GPIO_ClearOutBits(HT_GPIOD, GPIO_PIN_1)		// GPIO = LOW	// PD1
#define HX711_DOUT			GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_1)

extern void HX711_Init(void);
extern u32 HX711_Read(void);
extern void Get_Basic(void);
extern void Get_Weight(void);

extern s32 Weight_Medicine;

#endif
