#ifndef __ENCODER_H
#define __ENCODER_H

#include "ht32f5xxxx_bftm.h"

#define		HTCFG_GPTM0_CH2_GPIO_ID		(GPIO_PA)
#define		HTCFG_GPTM0_CH3_GPIO_ID		(GPIO_PA)
#define		HTCFG_GPTM1_CH2_GPIO_ID		(GPIO_PA)
#define		HTCFG_GPTM1_CH3_GPIO_ID		(GPIO_PA)

#define	  HTCFG_GPTM0_CH2_AFIO_PIN   (AFIO_PIN_6)
#define   HTCFG_GPTM0_CH3_AFIO_PIN   (AFIO_PIN_7)
#define   HTCFG_GPTM1_CH2_AFIO_PIN   (AFIO_PIN_2)
#define   HTCFG_GPTM1_CH3_AFIO_PIN   (AFIO_PIN_3)

#define   HT_GPTM0_IRQn               STRCAT2(GPTM0,  _IRQn)
#define   HT_GPTM0_IRQHandler         STRCAT2(GPTM0,  _IRQHandler)

#define   HT_GPTM1_IRQn               STRCAT2(GPTM1,  _IRQn)
#define   HT_GPTM1_IRQHandler         STRCAT2(GPTM1,  _IRQHandler)

#define   TM_CAP_PRESCAELER           (20)

#define		TIM_TIME										100
#define		Total_Resolution				    1040	//1040为转轮转过一周输出的脉冲数（4倍频后）

typedef struct
{
    float left_wheel_pulse_num;			//左轮当前编码器脉冲数
    float right_wheel_pulse_num;		//右轮当前编码器脉冲数
    float actual_value_l;						//左轮实际速度
    float actual_value_r;						//右轮实际速度
    float last_actual_l;						//上次左轮实际速度
    float last_actual_r;						//上次右轮实际速度
    float actual_value;							//实际速度（平均速度，左右轮脉冲相加除以2）
	
		float actual_value_l_rps;				//左右轮实际速度（转换为单位rps）
		float actual_value_r_rps;
}encoder_data;

typedef struct
{
    float low_pass_parameter;       //一阶低通滤波器滤波系数
    float sample_data;              //需要滤波的采样值
    float output;                   //当前滤波结果
    float output_last;              //上次滤波结果
}low_pass_filter_parameters;

void  TM_Capture_Configuration(void);
int   Read_Encoder(u8 GPTMX);
void  get_encoder_data(void);
void  encoder_data_process(void);
float Low_pass_filter(low_pass_filter_parameters *filter,float data);

//float mylog(float a);
//float myexp(float x);
//int power(int x, int n) ;


extern int Encoder_TIM; 
extern volatile encoder_data Speed_data;

#endif
