#include "ht32.h"
#include "usart.h"
#include "pwm.h"
#include "delay.h"
#include "hx711.h"
#include "encoder.h"
#include "MatrixKey.h"
#include "oled.h"
#include "isd1820.h"


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
  
unsigned char key_num = 0;
  
void MatrixKey_proc(void);
  
int main(void)
{
	MCTM_PWM_init();		// pwm��ʼ��
	HX711_Init();			// ��ʼ��ѹ��������
	USART_Configuration();	// ���ڳ�ʼ��
	TM_Capture_Configuration();
	GPIO_MatrixKey_Configuration();
	OLED_Init();
	ISD1820_Init();
	
	OLED_ShowString(0,Y_1," Key:",16);	// x�ķ�ΧΪ0-127��y�ķ�ΧΪ0-7������ѡ��12/16��16ռ���У�
	OLED_ShowString(0,Y_2," Weight:",16);
	OLED_ShowString(0,Y_3," Distance:",16);
	OLED_ShowString(0,Y_4," PULSE:",16);
//	OLED_ShowNum(0*8,Y_2,11,2,16);		// �������λ����ʵ��λ����������ǰ�油�ո� 
//	OLED_ShowNum(2*8,Y_3,2345,4,16);
//	OLED_ShowNum(3*8,Y_4,3,1,16);
	SetMotor_R(20);		//����ռ�ձ�
	SetMotor_L(20);
	
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
//		ISD1820_Alert();
//		delay_ms(4000); 
		Read_Encoder(1);
		OLED_ShowNum(8*8,Y_4,Encoder_TIM,7,16);
				delay_ms(100);
//		Get_Weight();
//		OLED_ShowNum(8*8,Y_2,Weight_Medicine_1,4,16);
		
//		OLED_ShowNum(0*8,Y_3,Weight,4,16);
//		delay_ms(50);
	
//		Usart_SendStr(COM0_PORT,"Hello World!");//ѭ�������ַ�����������
//		delay_ms(100);
//		Usart_Sendbyte(COM0_PORT,0xff);
//		delay_ms(100);
//		Ultrasonic_Ranging();
//		OLED_ShowNum(10*8,Y_3,distance,4,16);
		
//		key_num = MatrixKey();
//		if(key_num)	// ��������˰���
//		{
//			OLED_ShowNum(5*8,Y_1,key_num,2,16);
//		} 
//		
	}
}


void MatrixKey_proc()
{
	key_num = MatrixKey();
	if(key_num)	// ��������˰���
	{
		OLED_ShowNum(5*8,Y_1,key_num,2,16);
	} 
}
