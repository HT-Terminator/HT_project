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

//#define HTCFG_MCTM_RELOAD                           (48000000/2000)	//����2ms
#define HTCFG_MCTM_RELOAD                           4800

void GPIO_ServosPwm_Init(void);
void servos1_angle(unsigned char angle);
void servos2_angle(unsigned char angle);
void MCTM_PWM_init(void);
void PWM_SetFreq(u32 uReload);
void SetMotor_L(int PWMN);
void SetMotor_R(int PWML);
void SetMotor_Stop(void);

//void Servo_Run(uint16_t angle);
//void Servo_Run2(uint16_t angle);
//void Servo_Run3(uint16_t angle);
//void Servo_Run4(uint16_t angle);
#endif
