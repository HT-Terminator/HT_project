#include "MatrixKey.h"
#include "delay.h"

void GPIO_MatrixKey_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.AFIO = 1;
	CKCUClock.Bit.PA = 1;
	CKCUClock.Bit.PB = 1;
	CKCUClock.Bit.PC = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as input mode                                                                         */

    /* Configure AFIO mode as GPIO                                                                          */
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_FUN_GPIO);
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_3|AFIO_PIN_5|AFIO_PIN_7, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_3|AFIO_PIN_8|AFIO_PIN_9|AFIO_PIN_15, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);	// 上拉
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, GPIO_PR_UP);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, GPIO_PR_UP);

    /* Configure GPIO direction as input                                                                    */
    GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_IN);
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, GPIO_DIR_IN);
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, GPIO_DIR_IN);
	  
    /* Configure GPIO direction as ouput                                                                    */
    GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_OUT);
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, GPIO_DIR_OUT);
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, GPIO_DIR_OUT);
	  
    /* Enable input function for read                                                                       */
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);
    GPIO_InputConfig(HT_GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, ENABLE);
    GPIO_InputConfig(HT_GPIOC, GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, ENABLE);
  }
}

/**
  * @brief  矩阵键盘读取按键键码
  * @param  无
  * @retval KeyNumber 按下按键的键码值
			如果按键按下不放，程序会停留在此函数，松手的一瞬间，返回按键键码，没有按键按下时，返回0
  */
unsigned char MatrixKey()
{
	unsigned char KeyNumber=0;
	
	P1_SET(); // P1=0xFF;
	P1_3_RESET(); // P1_3=0; 
	if(P1_4==0){delay_ms(10);while(P1_7==0){};delay_ms(10);KeyNumber=1;}
	if(P1_5==0){delay_ms(10);while(P1_6==0){};delay_ms(10);KeyNumber=5;}
	if(P1_6==0){delay_ms(10);while(P1_5==0){};delay_ms(10);KeyNumber=9;}
	if(P1_7==0){delay_ms(10);while(P1_4==0){};delay_ms(10);KeyNumber=13;}
	
	P1_SET(); // P1=0xFF;
	P1_2_RESET(); // P1_2=0;
	if(P1_4==0){delay_ms(10);while(P1_7==0){};delay_ms(10);KeyNumber=2;}
	if(P1_5==0){delay_ms(10);while(P1_6==0){};delay_ms(10);KeyNumber=6;}
	if(P1_6==0){delay_ms(10);while(P1_5==0){};delay_ms(10);KeyNumber=10;}
	if(P1_7==0){delay_ms(10);while(P1_4==0){};delay_ms(10);KeyNumber=14;}
	
	P1_SET(); // P1=0xFF;
	P1_1_RESET(); // P1_1=0;
	if(P1_4==0){delay_ms(10);while(P1_7==0){};delay_ms(10);KeyNumber=3;}
	if(P1_5==0){delay_ms(10);while(P1_6==0){};delay_ms(10);KeyNumber=7;}
	if(P1_6==0){delay_ms(10);while(P1_5==0){};delay_ms(10);KeyNumber=11;}
	if(P1_7==0){delay_ms(10);while(P1_4==0){};delay_ms(10);KeyNumber=15;}
	
	P1_SET(); // P1=0xFF;
	P1_0_RESET(); // P1_0=0;
	if(P1_4==0){delay_ms(10);while(P1_7==0){};delay_ms(10);KeyNumber=4;}
	if(P1_5==0){delay_ms(10);while(P1_6==0){};delay_ms(10);KeyNumber=8;}
	if(P1_6==0){delay_ms(10);while(P1_5==0){};delay_ms(10);KeyNumber=12;}
	if(P1_7==0){delay_ms(10);while(P1_4==0){};delay_ms(10);KeyNumber=16;}
	
	return KeyNumber;
}
