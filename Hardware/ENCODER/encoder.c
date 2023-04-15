#include "time.h"
#include "encoder.h"
#include "ht32.h"

int		Encoder_Timer_Overflow_L		=	0;		//ȫ�ֱ�������¼��������������
int		Encoder_Timer_Overflow_R		=	0;		//ȫ�ֱ�������¼�ұ������������

//����������Ҫ�������ݴ�����λ���������壩��
s16 encoder1_speed;
s16 encoder2_speed;
encoder_data Speed_data;
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

  { /* Time base configuration                                                                              */
    TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;

    TM_TimeBaseInitStructure.Prescaler = TM_CAP_PRESCAELER - 1;  // Timer clock = CK_AHB / TM_CAP_PRESCAELER
    TM_TimeBaseInitStructure.CounterReload = 0xFFFF;
    TM_TimeBaseInitStructure.RepetitionCounter = 1;			//�ظ�����
    TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
    TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_GPTM0, &TM_TimeBaseInitStructure);
		TM_TimeBaseInit(HT_GPTM1, &TM_TimeBaseInitStructure);
  }
		//���ñ�����ģʽ��ģʽ3��up and dowm
		TM_DecoderConfig(HT_GPTM0,TM_SMSEL_DECODER3,TM_CHP_NONINVERTED,TM_CHP_NONINVERTED);
		TM_DecoderConfig(HT_GPTM1,TM_SMSEL_DECODER3,TM_CHP_NONINVERTED,TM_CHP_NONINVERTED);
		
		TM_ChExternalClockConfig(HT_GPTM0, TM_TRSEL_TI0BED, TM_CHP_INVERTED, 6);//�Ƚ��˲���
		TM_ChExternalClockConfig(HT_GPTM1, TM_TRSEL_TI0BED, TM_CHP_INVERTED, 6);//�Ƚ��˲���
	
  /* Clear Update Event Interrupt flag since the "TM_TimeBaseInit()" writes the UEV1G bit                   */
    TM_ClearFlag(HT_GPTM0, TM_FLAG_UEV);
	  TM_ClearFlag(HT_GPTM1, TM_FLAG_UEV);

//  { /* Channel n PWM input configuration                                                                    */
//    TM_CaptureInitTypeDef CapInit;

//    TM_CaptureStructInit(&CapInit);
//    CapInit.Channel = TM_CH_2;
//    CapInit.Polarity = TM_CHP_NONINVERTED;
//    CapInit.Selection = TM_CHCCS_DIRECT;
//    CapInit.Prescaler = TM_CHPSC_OFF;
////    #if (LIBCFG_TM_652XX_V1)
////    CapInit.Fsampling = TM_CHFDIV_1;
////    CapInit.Event = TM_CHFEV_OFF;
////    #else
//    CapInit.Filter = 0x00;
////		#endif
//    TM_PwmInputInit(HT_GPTM0, &CapInit);
//		
//		CapInit.Channel = TM_CH_3;
//		CapInit.Polarity = TM_CHP_INVERTED;
//		TM_PwmInputInit(HT_GPTM0, &CapInit);
//		
//		CapInit.Channel = TM_CH_2;
//		CapInit.Polarity = TM_CHP_NONINVERTED;
//		TM_PwmInputInit(HT_GPTM1, &CapInit);
//		
//		CapInit.Channel = TM_CH_3;
//		CapInit.Polarity = TM_CHP_INVERTED;
//		TM_PwmInputInit(HT_GPTM1, &CapInit);
//		
//  }

//  /* Set the STI trigger souece from the TI1S1, TM is configured as slave restart mode. The TM will be      */
//  /* restarted on the rising edge of the TM channel 1.                                                      */
//  TM_StiConfig(HT_GPTM0, TM_TRSEL_TI1S1);
//  TM_SlaveModeConfig(HT_GPTM0, TM_SMSEL_RESTART);

//  /* Set UGDIS = 1, disable the update event interrupt of the UEVG and slave mode, only counter             */
//  /* overflow/underflow generations an update event interrupt function.                                     */
//  TM_UEVG_IntConfig(HT_GPTM0, DISABLE);
//	TM_UEVG_IntConfig(HT_GPTM1, DISABLE);

  /* Enable TM Channel n Capture and Update Event interrupts                                                */
//  TM_IntConfig(HT_GPTM0, TM_INT_CH2CC | TM_INT_UEV, ENABLE);
//	TM_IntConfig(HT_GPTM0, TM_INT_CH3CC | TM_INT_UEV, ENABLE);
//	TM_IntConfig(HT_GPTM1, TM_INT_CH2CC | TM_INT_UEV, ENABLE);
//	TM_IntConfig(HT_GPTM1, TM_INT_CH3CC | TM_INT_UEV, ENABLE);
	
	TM_IntConfig(HT_GPTM0, TM_INT_UEV, ENABLE);
	TM_IntConfig(HT_GPTM1, TM_INT_UEV, ENABLE);
	
  NVIC_EnableIRQ(HT_GPTM0_IRQn);
	NVIC_EnableIRQ(HT_GPTM1_IRQn);

  TM_Cmd(HT_GPTM0, ENABLE);
	TM_Cmd(HT_GPTM1, ENABLE);
}

//��λʱ���ȡ������������Ӧ�÷����жϴ������д���
int Read_Encoder(u8 GPTMX)
{
   int Encoder_TIM;    
   switch(GPTMX)
	 {
	   case 0:  Encoder_TIM= (short)HT_GPTM0 -> CNTR;  HT_GPTM0 -> CNTR=0; break;	//����֮��ǵ���0
		 case 1:  Encoder_TIM= (short)HT_GPTM1 -> CNTR;  HT_GPTM1 -> CNTR=0; break;		
		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}

//GPTM0�жϷ�����,�����ж������һ�㲻�����
void HT_GPTM0_IRQHandler(void)
{
		//�����������жϣ�����ֵ������װ��ֵ��
		if(TM_GetIntStatus(HT_GPTM0, TM_INT_UEV) != RESET)
		{
			TM_ClearIntPendingBit(HT_GPTM0, TM_INT_UEV);
			Encoder_Timer_Overflow_L++;	//�������
			HT_GPTM0 -> CNTR = 0;			//���������ʱ����CNT��0
		}
}

//GPTM1�жϷ�����,�����ж������һ�㲻�����
void HT_GPTM1_IRQHandler(void)
{
		//�����������жϣ�����ֵ������װ��ֵ��
		if(TM_GetIntStatus(HT_GPTM1, TM_INT_UEV) != RESET)
		{
			TM_ClearIntPendingBit(HT_GPTM1, TM_INT_UEV);
			Encoder_Timer_Overflow_R++;	//�������
			HT_GPTM1 -> CNTR = 0;			//���������ʱ����CNT��0
		}
}

//2ms��ȡһ��
void get_encoder_data(void)
{
    encoder1_speed = Read_Encoder(0)+(65535*Encoder_Timer_Overflow_L);
		encoder2_speed = Read_Encoder(1)+(65535*Encoder_Timer_Overflow_R);
		
    Speed_data.left_wheel_pulse_num+=(float)encoder1_speed;	
    Speed_data.right_wheel_pulse_num+=(float)encoder2_speed;
	
		Encoder_Timer_Overflow_L=0;
		Encoder_Timer_Overflow_R=0;
}

//100msһ�δ���
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
		Speed_data.actual_value_l_rps = (float)Speed_data.actual_value_l/Total_Resolution*(1000/TIM_TIME);
		Speed_data.actual_value_r_rps = (float)Speed_data.actual_value_r/Total_Resolution*(1000/TIM_TIME);
	
}

//һ�׵�ͨ�˲�����
//filter:�ṹ��������ϵ��������
//data���˲�������
float Low_pass_filter(low_pass_filter_parameters *filter,float data)
{
    filter->sample_data=data;
    filter->output_last=filter->output;
    filter->output=filter->low_pass_parameter * filter->sample_data + (1.0-filter->low_pass_parameter) * filter->output_last;
    return filter->output;
}
