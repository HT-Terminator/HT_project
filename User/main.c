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
	MCTM_PWM_init();		// pwm初始化
	HX711_Init();			// 初始化压力传感器
	USART_Configuration();	// 串口初始化
	TM_Capture_Configuration();
	
//	Get_Basic();
//	Servo_Run(0);
//	Servo_Run2(0);
//	Servo_Run3(0);
//	Servo_Run4(0);
//	delay_ms(500);
	
	while (1)
	{
		Get_Weight();
	
//		Usart_SendStr(COM0_PORT,"Hello World!");//循环发送字符串，测试用
//		delay_ms(500);
//		Usart_Sendbyte(COM0_PORT,0xff);
		delay_ms(500);
		Ultrasonic_Ranging();
	}
}
