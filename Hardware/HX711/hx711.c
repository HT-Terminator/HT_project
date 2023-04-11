/************************************************************************************
						
*************************************************************************************/
#include "HX711.h"
#include "delay.h"

u32 HX711_Buffer_1;
u32 HX711_Buffer_2;
u32 HX711_Buffer_3;
u32 Weight_Basic_1;
u32 Weight_Basic_2;
u32 Weight_Basic_3;
s32 Weight_Medicine_1;
s32 Weight_Medicine_2;
s32 Weight_Medicine_3;
u8 Flag_Error = 0;

//校准参数
//因为不同的传感器特性曲线不是很一致，因此，每一个传感器需要矫正这里这个参数才能使测量值很准确。
//当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。
//该值可以为小数
#define GapValue_1 106.5
#define GapValue_2 106.5
#define GapValue_3 106.5

void GPIO_Init_out_pp(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PB = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as output mode                                                                        */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_0 | AFIO_PIN_2 | AFIO_PIN_4, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_4, GPIO_PR_DOWN);

    /* Default value RESET/SET                                                                              */
    GPIO_WriteOutBits(HT_GPIOB, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_4, RESET);

    /* Configure GPIO direction as output                                                                   */
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_4, GPIO_DIR_OUT);
  }
}

void GPIO_Init_ipu(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PB = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as input mode                                                                         */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_1 | AFIO_PIN_3 | AFIO_PIN_5, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_5, GPIO_PR_DOWN);

    /* Configure GPIO direction as input                                                                    */
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_5, GPIO_DIR_IN);

    /* Enable input function for read                                                                       */
    GPIO_InputConfig(HT_GPIOB, GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_5, ENABLE);
  }
}

void HX711_Init(void)
{
	//HX711_SCK
	GPIO_Init_out_pp();	// PB0 PB2 PB4
	
	//HX711_DOUT
	GPIO_Init_ipu();	// PB1 PB3 PB5
	
	//初始化设置为0
	GPIO_SetOutBits(HT_GPIOB, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_4);	
}

//****************************************************
//读取HX711
//****************************************************
u32 HX711_Read_1(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_SCK_HIGH_1();
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

u32 HX711_Read_2(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_SCK_HIGH_2();
	delay_us(1);
  	HX711_SCK_LOW_2(); 
  	count=0; 
  	while(HX711_DOUT_2); 
  	for(i=0;i<24;i++)
	{ 
	  	HX711_SCK_HIGH_2();
	  	count=count<<1; 
		delay_us(1);
		HX711_SCK_LOW_2(); 
	  	if(HX711_DOUT_2)
			count++; 
		delay_us(1);
	} 
 	HX711_SCK_HIGH_2();
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	delay_us(1);
	HX711_SCK_LOW_2(); 
	return(count);
}

u32 HX711_Read_3(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 
  	HX711_SCK_HIGH_3();
	delay_us(1);
  	HX711_SCK_LOW_3(); 
  	count=0; 
  	while(HX711_DOUT_3); 
  	for(i=0;i<24;i++)
	{ 
	  	HX711_SCK_HIGH_3();
	  	count=count<<1; 
		delay_us(1);
		HX711_SCK_LOW_3(); 
	  	if(HX711_DOUT_3)
			count++; 
		delay_us(1);
	} 
 	HX711_SCK_HIGH_3();
    count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	delay_us(1);
	HX711_SCK_LOW_3(); 
	return(count);
}
//****************************************************
//获取无重物时度数
//****************************************************
void Get_Basic(void)
{
	Weight_Basic_1 = HX711_Read_1();	
	Weight_Basic_2 = HX711_Read_2();
	Weight_Basic_3 = HX711_Read_3();
} 

//****************************************************
//称重
//****************************************************
void Get_Weight(void)
{
	HX711_Buffer_1 = HX711_Read_1();
	HX711_Buffer_2 = HX711_Read_2();
	HX711_Buffer_3 = HX711_Read_3();
	if(HX711_Buffer_1 > Weight_Basic_1)			
	{
		Weight_Medicine_1 = HX711_Buffer_1;
		Weight_Medicine_1 = Weight_Medicine_1 - Weight_Basic_1;				//获取实物的AD采样数值。
		Weight_Medicine_1 = (s32)((float)Weight_Medicine_1/GapValue_1); 	//计算实物的实际重量
	}
	else
	{
		Weight_Medicine_1 = 0;
	}
	
	if(HX711_Buffer_2 > Weight_Basic_2)			
	{
		Weight_Medicine_2 = HX711_Buffer_2;
		Weight_Medicine_2 = Weight_Medicine_2 - Weight_Basic_2;				//获取实物的AD采样数值。
		Weight_Medicine_2 = (s32)((float)Weight_Medicine_2/GapValue_2); 	//计算实物的实际重量
	}
	else
	{
		Weight_Medicine_2 = 0;
	}
	
	if(HX711_Buffer_3 > Weight_Basic_3)			
	{
		Weight_Medicine_3 = HX711_Buffer_3;
		Weight_Medicine_3 = Weight_Medicine_3 - Weight_Basic_3;				//获取实物的AD采样数值。
		Weight_Medicine_3 = (s32)((float)Weight_Medicine_3/GapValue_3); 	//计算实物的实际重量
	}
	else
	{
		Weight_Medicine_3 = 0;
	}
}
