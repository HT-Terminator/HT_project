#include "pwm.h"
#include "time.h"

unsigned char pwm_tim_mun = 2;
unsigned char pwm_pin_value = 1,tim1_num1; // 

void GPIO_PWM_Init()
{
  { /* Enable peripheral clock                                                                              */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.AFIO = 1;
	CKCUClock.Bit.PC = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure AFIO mode as GPIO                                                                          */
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1|AFIO_PIN_5, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_1|GPIO_PIN_5, GPIO_PR_UP);

    /* Configure GPIO direction as ouput                                                                    */
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1|GPIO_PIN_5, GPIO_DIR_OUT);
	  
    /* Enable input function for read                                                                       */
    GPIO_InputConfig(HT_GPIOC, GPIO_PIN_1|GPIO_PIN_5, ENABLE);
  }
}

//void pwm_proc()
//{
//	if((pwm_pin_value == 1) && tim1_num1 >= 19)
//	{
//		tim1_num1 = 0;
//		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, SET);
//		pwm_pin_value = !pwm_pin_value;
//	}
//	else if((pwm_pin_value == 0) && tim1_num1 >= 1)
//	{
//		tim1_num1 = 1;
//		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, RESET);
//		pwm_pin_value = !pwm_pin_value;
//	}
//}

