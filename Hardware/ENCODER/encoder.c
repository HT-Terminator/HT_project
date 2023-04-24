#include "time.h"
#include "encoder.h"
#include "ht32.h"
#include "math.h"
int Encoder_TIM; 

int encoder_countl = 0,encoder_countr = 0;

//����������Ҫ�������ݴ�����λ���������壩��
s16 encoder1_speed;
s16 encoder2_speed;

volatile encoder_data Speed_data;
//int sum_encoder;

//��ͨ�˲�������ֻ�õ���һ��ֵ��һ�׵�ͨ�˲����˲�ϵ����
low_pass_filter_parameters l_speed={0.9,0,0,0};
low_pass_filter_parameters r_speed={0.9,0,0,0};

//GPTM0   PA6 PA7 
//GPTM1   PA2 PA3
void TM_Capture_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
		CKCUClock.Bit.PA   = 1;
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.GPTM0 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
		
		CKCUClock.Bit.GPTM1 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Configure AFIO mode as TM function                                                                     */
  AFIO_GPxConfig(HTCFG_GPTM0_CH2_GPIO_ID, HTCFG_GPTM0_CH2_AFIO_PIN, AFIO_FUN_GPTM0);
	AFIO_GPxConfig(HTCFG_GPTM0_CH3_GPIO_ID, HTCFG_GPTM0_CH3_AFIO_PIN, AFIO_FUN_GPTM0);
	AFIO_GPxConfig(HTCFG_GPTM1_CH2_GPIO_ID, HTCFG_GPTM1_CH2_AFIO_PIN, AFIO_FUN_GPTM1);
	AFIO_GPxConfig(HTCFG_GPTM1_CH3_GPIO_ID, HTCFG_GPTM1_CH3_AFIO_PIN, AFIO_FUN_GPTM1);
  GPIO_InputConfig(HT_GPIOA,GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_2|GPIO_PIN_3,ENABLE);

  { /* Time base configuration                                                                              */
    TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;

    TM_TimeBaseInitStructure.Prescaler = TM_CAP_PRESCAELER - 1;  // Timer clock = CK_AHB / TM_CAP_PRESCAELER
		TM_TimeBaseInitStructure.CounterReload = 0xFFFF;
    TM_TimeBaseInitStructure.RepetitionCounter = 0;			//BU�ظ�����
    TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
    TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_GPTM0, &TM_TimeBaseInitStructure);
		TM_TimeBaseInit(HT_GPTM1, &TM_TimeBaseInitStructure);
  }
	
  { /* Channel n PWM input configuration                                                                    */
    TM_CaptureInitTypeDef CapInit;

    TM_CaptureStructInit(&CapInit);
    CapInit.Channel = TM_CH_2;
    CapInit.Polarity = TM_CHP_NONINVERTED;	//������
    CapInit.Selection = TM_CHCCS_DIRECT;		//ֱ������
    CapInit.Prescaler = TM_CHPSC_OFF;
    CapInit.Filter = 0x00;
    TM_PwmInputInit(HT_GPTM0, &CapInit);
		
		CapInit.Channel = TM_CH_3;
		CapInit.Polarity = TM_CHP_NONINVERTED;
		TM_PwmInputInit(HT_GPTM0, &CapInit);
		
		CapInit.Channel = TM_CH_2;
		CapInit.Polarity = TM_CHP_NONINVERTED;
		TM_PwmInputInit(HT_GPTM1, &CapInit);
		
		CapInit.Channel = TM_CH_3;
		CapInit.Polarity = TM_CHP_NONINVERTED;
		TM_PwmInputInit(HT_GPTM1, &CapInit);		
  }
	TM_IntConfig(HT_GPTM0, TM_INT_CH2CC , ENABLE);
//	TM_IntConfig(HT_GPTM0, TM_INT_CH3CC , ENABLE);
	TM_IntConfig(HT_GPTM1, TM_INT_CH2CC , ENABLE);
//	TM_IntConfig(HT_GPTM1, TM_INT_CH3CC , ENABLE);
	
  NVIC_EnableIRQ(HT_GPTM0_IRQn);
	NVIC_EnableIRQ(HT_GPTM1_IRQn);
	

  TM_Cmd(HT_GPTM0, ENABLE);
	TM_Cmd(HT_GPTM1, ENABLE);
}

//�жϷ�����
//����
void HT_GPTM0_IRQHandler(void)
{
		if (TM_GetIntStatus(HT_GPTM0, TM_INT_CH2CC) != RESET)
		{
				TM_ClearIntPendingBit(HT_GPTM0, TM_INT_CH2CC);
				encoder_countl++;
				
		}

}

//����
void HT_GPTM1_IRQHandler(void)
{
		if (TM_GetIntStatus(HT_GPTM1, TM_INT_CH2CC) != RESET)
		{
				TM_ClearIntPendingBit(HT_GPTM1, TM_INT_CH2CC);
				encoder_countr++;	
		}

}



//��������CCR��ֵ��CCR��ÿ���ڴ����¼�������ʱ����CNT��ֵ
//�����ԣ�
//1:���֣�0������
#if 0
int Read_Encoder(u8 GPTMX)
{
//   int Encoder_TIM;    
   switch(GPTMX)
	 {
		 case 0:
			Encoder_TIM = TM_GetCaptureCompare2(HT_GPTM0)-TM_GetCaptureCompare3(HT_GPTM0);
			TM_SetCaptureCompare2(HT_GPTM0, 0);
			TM_SetCaptureCompare3(HT_GPTM0, 0);
		 break;
		 case 1:
			Encoder_TIM = TM_GetCaptureCompare2(HT_GPTM1)-TM_GetCaptureCompare3(HT_GPTM1);
			TM_SetCaptureCompare2(HT_GPTM1, 0);
			TM_SetCaptureCompare3(HT_GPTM1, 0);
		 break;
		 	
		 default:  Encoder_TIM=0;
	 }
	 
	 if(Encoder_TIM<0)
	 {
			Encoder_TIM = -Encoder_TIM;
	 }
	 
		return Encoder_TIM;
}
#endif

#if 1
int Read_Encoder(u8 GPTMX)
{
//   int Encoder_TIM;    
   switch(GPTMX)
	 {
		 case 0:
			Encoder_TIM = encoder_countl;
			encoder_countl = 0;
		 break;
		 case 1:
			Encoder_TIM = encoder_countr;
			encoder_countr = 0;
		 break;
		 	
		 default:  Encoder_TIM=0;
	 }
	 
		return Encoder_TIM;
}
#endif

void get_encoder_data(void)
{
    encoder1_speed = Read_Encoder(0);
		encoder2_speed = Read_Encoder(1);
		
    Speed_data.left_wheel_pulse_num  += (float)encoder1_speed;	
    Speed_data.right_wheel_pulse_num += (float)encoder2_speed;
	
}

void encoder_data_process(void)
{
    Speed_data.last_actual_l=Speed_data.actual_value_l;
    Speed_data.last_actual_r=Speed_data.actual_value_r;

    Speed_data.actual_value_l= Low_pass_filter(&l_speed,Speed_data.left_wheel_pulse_num);
    Speed_data.actual_value_r= Low_pass_filter(&r_speed,Speed_data.right_wheel_pulse_num);

    Speed_data.left_wheel_pulse_num=0;
	  Speed_data.right_wheel_pulse_num=0;	//���ݴ�����֮�������й�0����

    Speed_data.actual_value=(Speed_data.actual_value_l+Speed_data.actual_value_r)/2.0;	//ƽ���ٶ�
	
		//��λת��
		/* ת��(1����ת����Ȧ)=��λʱ����(100ms)�ļ���ֵ/�ֱܷ���*ʱ��ϵ��,�ֱܷ��ʼ�����ת��һȦ����������ʱ��ϵ����1000ms/��ʱʱ�� */
//		Speed_data.actual_value_l_rps = (float)Speed_data.actual_value_l/Total_Resolution*(1000/TIM_TIME);
//		Speed_data.actual_value_r_rps = (float)Speed_data.actual_value_r/Total_Resolution*(1000/TIM_TIME);
	
}

//һ�׵�ͨ�˲�����
//filter:�ṹ��������ϵ��������
//data���˲�������
#if 0
float Low_pass_filter(low_pass_filter_parameters *filter,float data)
{
    filter->sample_data=data;
    filter->output_last=filter->output;
    filter->output=filter->low_pass_parameter * filter->sample_data + (1.0-filter->low_pass_parameter) * filter->output_last;
		
		filter->output = 451679 * pow( filter->output, -0.842); 
	
    return filter->output;
}
#endif

#if 1
float Low_pass_filter(low_pass_filter_parameters *filter,float data)
{
    filter->sample_data=data;
    filter->output_last=filter->output;
    filter->output=filter->low_pass_parameter * filter->sample_data + (1.0-filter->low_pass_parameter) * filter->output_last; 
	
    return filter->output;
}


#endif



////�Զ���log����
//float mylog(float a)
//{
//   int N = 15;//����ȡ��ǰ15+1��������
//   int k,nk;
//   int x,xx,y;
//   x = (a-1)/(a+1);
//   xx = x*x;
//   nk = 2*N+1;
//   y = 1.0/nk;
//   for(k=N;k>0;k--)
//   {
//     nk = nk - 2;
//     y = 1.0/nk+xx*y;
//     
//   }
//   return 2.0*x*y;
//}

////�Զ���exp����
//float myexp(float x) 
//{
//    int result = 1;
//    int term = 1;
//	 //̩��չ��ǰ50��
//    for (int i = 1; i <= 50; i++) {
//        term *= x / i;
//        result += term;
//    }
//    return result;
//}

////�Զ���power����
//int power(int x, int n) 
//{
//    return myexp(n * mylog(x));
//}



