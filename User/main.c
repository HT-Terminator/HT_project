#include "ht32.h"
#include "usart.h"
#include "pwm.h"
#include "delay.h"
#include "hx711.h"
#include "encoder.h"

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
	MCTM_PWM_init();		// pwm��ʼ��
	HX711_Init();			// ��ʼ��ѹ��������
	USART_Configuration();	// ���ڳ�ʼ��
	TM_Capture_Configuration();
	
	Get_Basic();
//	Servo_Run(0);
//	Servo_Run2(0);
//	Servo_Run3(0);
//	Servo_Run4(0);
			for(int i = 0;i<20;i++){
			delay_ms(100);
			}
	Get_Basic();
	
	
	while (1)
	{
		Get_Weight();
	
//		Usart_SendStr(COM0_PORT,"Hello World!");//ѭ�������ַ�����������
//		delay_ms(500);
//		Usart_Sendbyte(COM0_PORT,0xff);
		delay_ms(500);
		Ultrasonic_Ranging();
	}
}
