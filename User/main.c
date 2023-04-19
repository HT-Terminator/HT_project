#include "ht32.h"
#include "usart.h"
#include "pwm.h"
#include "delay.h"
#include "hx711.h"
#include "MatrixKey.h"

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
  
  unsigned char key_num = 0;
int main(void)
{
	GPTM_PWM_init();		// pwm��ʼ��
	HX711_Init();			// ��ʼ��ѹ��������
	USART_Configuration();	// ���ڳ�ʼ��
	GPIO_MatrixKey_Configuration();
//	Get_Basic();
//	Servo_Run(0);
//	Servo_Run2(0);
//	Servo_Run3(0);
//	Servo_Run4(0);
//	delay_ms(500);
	
	while (1)
	{
//		Get_Weight();
	
//		Usart_SendStr(COM0_PORT,"Hello World!");//ѭ�������ַ�����������
//		delay_ms(500);
		Usart_Sendbyte(COM0_PORT,0xff);
		delay_ms(500);
		Ultrasonic_Ranging();
		
//		key_num = MatrixKey();
//		if(key_num){} // ��������˰���
		
		
	}
}
