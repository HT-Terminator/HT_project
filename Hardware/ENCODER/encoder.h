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
#define		Total_Resolution				    1040	//1040Ϊת��ת��һ���������������4��Ƶ��

typedef struct
{
    float left_wheel_pulse_num;			//���ֵ�ǰ������������
    float right_wheel_pulse_num;		//���ֵ�ǰ������������
    float actual_value_l;						//����ʵ���ٶ�
    float actual_value_r;						//����ʵ���ٶ�
    float last_actual_l;						//�ϴ�����ʵ���ٶ�
    float last_actual_r;						//�ϴ�����ʵ���ٶ�
    float actual_value;							//ʵ���ٶȣ�ƽ���ٶȣ�������������ӳ���2��
	
		float actual_value_l_rps;				//������ʵ���ٶȣ�ת��Ϊ��λrps��
		float actual_value_r_rps;
}encoder_data;

typedef struct
{
    float low_pass_parameter;       //һ�׵�ͨ�˲����˲�ϵ��
    float sample_data;              //��Ҫ�˲��Ĳ���ֵ
    float output;                   //��ǰ�˲����
    float output_last;              //�ϴ��˲����
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
