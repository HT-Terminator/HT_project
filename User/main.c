#include "ht32.h"
#include "usart.h"
#include "pwm.h"
#include "delay.h"
#include "hx711.h"
#include "MatrixKey.h"
#include "oled.h"

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
	OLED_Init();
	
	OLED_ShowString(0,Y_1,"Hello",16);// x�ķ�ΧΪ0-127��y�ķ�ΧΪ0-7������ѡ��12/16��16ռ���У�
	OLED_ShowNum(0*8,Y_2,11,2,16);	// �������λ����ʵ��λ����������ǰ�油�ո� 
	OLED_ShowNum(2*8,Y_3,2345,4,16);
	OLED_ShowNum(3*8,Y_4,3,1,16);
	
	
//	Get_Basic();
//	Servo_Run(0);
//	Servo_Run2(0);
//	Servo_Run3(0);
//	Servo_Run4(0);
//	delay_ms(500);
	
	while (1)
	{
//		Get_Weight();
//		delay_ms(50);
	
//		Usart_SendStr(COM0_PORT,"Hello World!");//ѭ�������ַ�����������
		delay_ms(100);
		Usart_Sendbyte(COM0_PORT,0xff);
		delay_ms(100);
		Ultrasonic_Ranging();
		
//		key_num = MatrixKey();
//		if(key_num){} // ��������˰���
		
	}
}
