/************************************************************************************
						
*************************************************************************************/
#include "HX711.h"
#include "delay.h"
#include "oled.h"
#include "MatrixKey.h"
#include "timdelay.h"
#include "pwm.h" 
#include "Cloud.h"

u32 HX711_Buffer_1;
u32 Weight_Basic_1;
volatile int Weight_Medicine_1;
int weight_now;
int weight_last;
uint8_t diffbig=0;

uint8_t pickup = 0;	//��¼��Ʒ�Ƿ�����δ����0��������1
uint8_t pickupover = 0;	//��¼��Ʒ�Ƿ���ȫ����δ����0��������1
uint8_t saleprice = 0;

#define firstsale  	5
#define secondsale  10

//У׼����
//��Ϊ��ͬ�Ĵ������������߲��Ǻ�һ�£���ˣ�ÿһ����������Ҫ�������������������ʹ����ֵ��׼ȷ��
//�����ֲ��Գ���������ƫ��ʱ�����Ӹ���ֵ��
//������Գ���������ƫСʱ����С����ֵ��
//��ֵ����ΪС��
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
	
	//��ʼ������Ϊ0
	GPIO_SetOutBits(HT_GPIOD, GPIO_PIN_1);	
}

//****************************************************
//��ȡHX711
//****************************************************
int HX711_Read_1(void)	//����128
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
    count=count^0x800000;//��25�������½�����ʱ��ת������
	delay_us(1);
	HX711_SCK_LOW_1(); 
	return(count);
}

void Get_Basic(void)
{
	Weight_Basic_1 = HX711_Read_1();	
} 

//****************************************************
//����
//****************************************************
void Get_Weight(void)
{
	int diff = 0;
	weight_last = weight_now;
	HX711_Buffer_1 = HX711_Read_1();
	if(HX711_Buffer_1 > Weight_Basic_1)			
	{
		Weight_Medicine_1 = HX711_Buffer_1;
		Weight_Medicine_1 = Weight_Medicine_1 - Weight_Basic_1;				//��ȡʵ���AD������ֵ��
		Weight_Medicine_1 = (int)((float)Weight_Medicine_1/GapValue); 	//����ʵ���ʵ������
	}
	else
	{
		Weight_Medicine_1 = 0;
	}
	weight_now = Weight_Medicine_1;
	OLED_ShowNum(3*8,Y_4,weight_now,4,16);
	
	diff = (int)(weight_last - weight_now);
	
	if(diff<0)
		diff = -diff;
	
	
	if(State == 2 && sale_state == 0){	//�ͻ�ȡ��
		if(diff > 15)
			diffbig=1;
	
		if( diffbig == 1)
			{
				pickup = 1;
				pickupover = 0;
				if(sctm_pickup_2500ms >= 2500*2)
				{
					ServosR_control(CLOSE);
					State  = 1;
					pickupover = 1;
					pickup = 0;
					sctm_pickup_2500ms=0;
					diffbig = 0;
					Send.Pickup = 1;
					Send.Change_Flag =1;
					Send.State = 1;	//������ȡ��
					Cloud_All_Msg_Push();
					state_show();
				}
			}
	}
	else if(State == 2 && sale_state == 1){	//�ۻ�ȡ
		if(diff > 15)
			diffbig=1;
	
		if(diffbig == 1)
		{
			pickup = 1;
			pickupover = 0;
			if(sctm_pickup_2500ms >= 5500*2)
				{
					ServosL_control(CLOSE);
					State  = 1;
					pickupover = 1;
					pickup = 0;
					sctm_pickup_2500ms=0;
					diffbig = 0;
					Sell.Pickup = 1;				
					Send.Change_Flag =1;
					Send.State = 1;	//������ȡ��
					Cloud_All_Msg_Push();
					state_show();
				}
			else if(sctm_pickup_2500ms >= 2500*2 && sctm_pickup_2500ms < 5000*2)
				{
					if(weight_now < 95+5 && weight_now > 95-5)
						saleprice = firstsale;
//						OLED_ShowNum(9*8,Y_4,firstsale,4,16);	//���ߵ�һ����Ʒ����ʾ5Ԫ
					else if(weight_now < 95+10 && weight_now > 95)
						saleprice = secondsale;
					else
						saleprice = 0;
//						OLED_ShowNum(9*8,Y_4,secondsale,4,16);		//���ߵڶ�����Ʒ����ʾ10Ԫ
//					sctm_pickup_2500ms=0;
					OLED_ShowNum(12*8,Y_4,saleprice,2,16);
				}
		}
	}
//	OLED_ShowNum(8*8,Y_4,diff,4,16);
}
