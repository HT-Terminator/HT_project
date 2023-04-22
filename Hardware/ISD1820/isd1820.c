#include "isd1820.h"
#include "delay.h"

void ISD1820_Init()
{
 { /* Enable peripheral clock                                                                              */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.AFIO = 1;
	CKCUClock.Bit.PB = 1;
	CKCUClock.Bit.PD = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as input mode                                                                         */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_6|AFIO_PIN_8, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PD, AFIO_PIN_0|AFIO_PIN_2, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_6|GPIO_PIN_8, GPIO_PR_DOWN);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_0|GPIO_PIN_2, GPIO_PR_DOWN);
	  
    /* Configure GPIO direction as ouput                                                                    */
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_6|GPIO_PIN_8, GPIO_DIR_OUT);
    GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_0|GPIO_PIN_2, GPIO_DIR_OUT);
	  
    /* Enable input function for read                                                                       */
    GPIO_InputConfig(HT_GPIOB, GPIO_PIN_6|GPIO_PIN_8, ENABLE);
    GPIO_InputConfig(HT_GPIOD, GPIO_PIN_0|GPIO_PIN_2, ENABLE);
	  
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6, RESET); // E 是PB6 L 是PD2 录音是8
	// P-L 是检测到上升沿就播放，直到播放完录音或检测到下降沿才停止
	// P-E 是检测到上升沿就播放，直到播放完录音才停止
  }
}

void ISD1820_Alert()
{
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6, SET); // 按键按下是高电平
	delay_ms(10); 
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6, RESET);
}
