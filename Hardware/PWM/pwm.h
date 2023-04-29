#ifndef _PWM_H_
#define _PWM_H_
#include "ht32f5xxxx_bftm.h"

#define HTCFG_MCTM_CH0_GPIO_ID                      (GPIO_PA)
#define HTCFG_MCTM_CH1_GPIO_ID                      (GPIO_PB)
#define HTCFG_MCTM_CH2_GPIO_ID                      (GPIO_PB)
#define HTCFG_MCTM_CH3_GPIO_ID                      (GPIO_PD)

#define HTCFG_MCTM_CH0_AFIO_PIN                     (AFIO_PIN_14)
#define HTCFG_MCTM_CH1_AFIO_PIN                     (AFIO_PIN_0)
#define HTCFG_MCTM_CH2_AFIO_PIN                     (AFIO_PIN_2)
#define HTCFG_MCTM_CH3_AFIO_PIN                     (AFIO_PIN_3)

//#define HTCFG_MCTM_RELOAD                           (48000000/2000)	//锟斤拷锟斤拷2ms
#define HTCFG_MCTM_RELOAD                           4800

#define OPEN	1
#define CLOSE	0

extern unsigned char timL_num_high;
extern unsigned char timR_num_high;


void MCTM_PWM_init(void);
void PWM_SetFreq(u32 uReload);
void SetMotor_L(int PWMN);
void SetMotor_R(int PWML);
void SetMotor_Stop(void);
void SetMotor_Run(void);
void GPIO_ServosPwm_Init(void);
void ServosL_control(unsigned char single);
void ServosR_control(unsigned char single);

//void Servo_Run(uint16_t angle);
//void Servo_Run2(uint16_t angle);
//void Servo_Run3(uint16_t angle);
//void Servo_Run4(uint16_t angle);
#endif
