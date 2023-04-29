#ifndef __HX711_H
#define __HX711_H

#include "ht32f5xxxx_01.h"

#define HX711_SCK_HIGH_1() 	GPIO_SetOutBits(HT_GPIOC, GPIO_PIN_11)		// GPIO = HIGH	// PC11
#define HX711_SCK_LOW_1() 	GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_11)		// GPIO = LOW	// PC11
#define HX711_DOUT_HIGH_1()	GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_1)		// GPIO = HIGH	// PD1
#define HX711_DOUT_LOW_1() 	GPIO_ClearOutBits(HT_GPIOD, GPIO_PIN_1)		// GPIO = LOW	// PD1
#define HX711_DOUT_1				GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_1)

extern void HX711_Init(void);
extern int HX711_Read_1(void);
extern void Get_Basic(void);
extern void Get_Weight(void);

extern volatile int Weight_Medicine_1;
extern uint8_t pickup;
extern uint8_t pickupover;
extern uint8_t saleprice;

#endif
