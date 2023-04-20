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
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_6|GPIO_PIN_8, GPIO_PR_UP);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_0|GPIO_PIN_2, GPIO_PR_UP);
	  
    /* Configure GPIO direction as ouput                                                                    */
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_6|GPIO_PIN_8, GPIO_DIR_OUT);
    GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_0|GPIO_PIN_2, GPIO_DIR_OUT);
	  
    /* Enable input function for read                                                                       */
    GPIO_InputConfig(HT_GPIOB, GPIO_PIN_6|GPIO_PIN_8, ENABLE);
    GPIO_InputConfig(HT_GPIOD, GPIO_PIN_0|GPIO_PIN_2, ENABLE);
	  
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6, RESET); // E ��PB6 L ��PD2 ¼����8
	// P-L �Ǽ�⵽�����ؾͲ��ţ�ֱ��������¼�����⵽�½��ز�ֹͣ
	// P-E �Ǽ�⵽�����ؾͲ��ţ�ֱ��������¼����ֹͣ
  }
}

void ISD1820_Alert()
{
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6, SET); // ���������Ǹߵ�ƽ
	delay_ms(10); 
	GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_6, RESET);
}
