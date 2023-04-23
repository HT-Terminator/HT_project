/************************************************************************************
						
*************************************************************************************/
#include "HX711.h"
#include "delay.h"

u32 HX711_Buffer_1;
u32 Weight_Basic_1;
volatile int Weight_Medicine_1;

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue 106.5

void GPIO_Init_out_pp(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PC = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as output mode                                                                        */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PC, AFIO_PIN_11, AFIO_FUN_GPIO);
		

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_11, GPIO_PR_DOWN);

    /* Default value RESET/SET                                                                              */
    GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_11, RESET);

    /* Configure GPIO direction as output                                                                   */
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_11, GPIO_DIR_OUT);
  }
}

void GPIO_Init_ipu(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PD = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as input mode                                                                         */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PD, AFIO_PIN_1,  AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_1, GPIO_PR_DOWN);

    /* Configure GPIO direction as input                                                                    */
    GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_1, GPIO_DIR_IN);

    /* Enable input function for read                                                                       */
    GPIO_InputConfig(HT_GPIOD, GPIO_PIN_1, ENABLE);
  }
}

void HX711_Init(void)
{
	//HX711_SCK
	GPIO_Init_out_pp();	// PC11
	
	//HX711_DOUT
	GPIO_Init_ipu();	// PD1
	
	//初始化设置为0
	GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_1);	
}

//****************************************************
//读取HX711
//****************************************************
u32 HX711_Read_1(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_DOUT_HIGH_1();
	delay_us(1);
  	HX711_SCK_LOW_1(); 
  	count=0; 
  	while(HX711_DOUT_1); 
  	for(i=0;i<24;i++)
	{ 
	  	HX711_SCK_HIGH_1();
	  	count=count<<1; 
		delay_us(1);
		HX711_SCK_LOW_1(); 
	  	if(HX711_DOUT_1)
			count++; 
		delay_us(1);
	} 
 	HX711_SCK_HIGH_1();
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	delay_us(1);
	HX711_SCK_LOW_1(); 
	return(count);
}

void Get_Basic(void)
{
	Weight_Basic_1 = HX711_Read_1();	
} 

//****************************************************
//称重
//****************************************************
void Get_Weight(void)
{
	HX711_Buffer_1 = HX711_Read_1();
	if(HX711_Buffer_1 > Weight_Basic_1)			
	{
		Weight_Medicine_1 = HX711_Buffer_1;
		Weight_Medicine_1 = Weight_Medicine_1 - Weight_Basic_1;				//获取实物的AD采样数值。
		Weight_Medicine_1 = (s32)((float)Weight_Medicine_1/GapValue); 	//计算实物的实际重量
	}
	else
	{
		Weight_Medicine_1 = 0;
	}

}
