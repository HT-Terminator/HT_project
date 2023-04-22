#include "time.h"
#include "encoder.h"
#include "ht32.h"
int Encoder_TIM; 

int		Encoder_Timer_Overflow_L		=	0;		//全局变量，记录左编码器溢出次数
int		Encoder_Timer_Overflow_R		=	0;		//全局变量，记录右编码器溢出次数

//以下数据主要用于数据处理（单位：个（脉冲））
s16 encoder1_speed;
s16 encoder2_speed;
encoder_data Speed_data;
//int sum_encoder;

//低通滤波参数，只用到第一个值（一阶低通滤波器滤波系数）
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
    TM_TimeBaseInitStructure.RepetitionCounter = 0;			//BU重复计数
    TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
    TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_GPTM0, &TM_TimeBaseInitStructure);
		TM_TimeBaseInit(HT_GPTM1, &TM_TimeBaseInitStructure);
  }
	
  { /* Channel n PWM input configuration                                                                    */
    TM_CaptureInitTypeDef CapInit;

    TM_CaptureStructInit(&CapInit);
    CapInit.Channel = TM_CH_2;
    CapInit.Polarity = TM_CHP_NONINVERTED;	//上升沿
    CapInit.Selection = TM_CHCCS_DIRECT;		//直接输入
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

  TM_Cmd(HT_GPTM0, ENABLE);
	TM_Cmd(HT_GPTM1, ENABLE);
}

//读到的是CCR的值，CCR在每次在触发事件到来了时保存CNT的值
//经测试：
//
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

void get_encoder_data(void)
{
    encoder1_speed = Read_Encoder(0);
		encoder2_speed = Read_Encoder(1);
		
    Speed_data.left_wheel_pulse_num+=(float)encoder1_speed;	
    Speed_data.right_wheel_pulse_num+=(float)encoder2_speed;
	
		Encoder_Timer_Overflow_L=0;
		Encoder_Timer_Overflow_R=0;
}

void encoder_data_process(void)
{
    Speed_data.last_actual_l=Speed_data.actual_value_l;
    Speed_data.last_actual_r=Speed_data.actual_value_r;

    Speed_data.actual_value_l= Low_pass_filter(&l_speed,Speed_data.left_wheel_pulse_num);
    Speed_data.actual_value_r= Low_pass_filter(&r_speed,Speed_data.right_wheel_pulse_num);

    Speed_data.left_wheel_pulse_num=0;
	  Speed_data.right_wheel_pulse_num=0;	//数据处理完之后必须进行归0操作

    Speed_data.actual_value=(Speed_data.actual_value_l+Speed_data.actual_value_r)/2.0;	//平均速度
	
		//单位转换
		/* 转速(1秒钟转多少圈)=单位时间内(100ms)的计数值/总分辨率*时间系数,总分辨率即车轮转过一圈的脉冲数，时间系数是1000ms/定时时长 */
//		Speed_data.actual_value_l_rps = (float)Speed_data.actual_value_l/Total_Resolution*(1000/TIM_TIME);
//		Speed_data.actual_value_r_rps = (float)Speed_data.actual_value_r/Total_Resolution*(1000/TIM_TIME);
	
}

//一阶低通滤波函数
//filter:结构体里的相关系数及数据
//data：滤波的数据
float Low_pass_filter(low_pass_filter_parameters *filter,float data)
{
    filter->sample_data=data;
    filter->output_last=filter->output;
    filter->output=filter->low_pass_parameter * filter->sample_data + (1.0-filter->low_pass_parameter) * filter->output_last;
    return filter->output;
}
