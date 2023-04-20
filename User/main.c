#include "ht32.h"
#include "usart.h"
#include "pwm.h"
#include "delay.h"
#include "hx711.h"
#include "MatrixKey.h"
#include "oled.h"
#include "isd1820.h"

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
  
  unsigned char key_num = 0;
int main(void)
{
	GPTM_PWM_init();		// pwm初始化
	HX711_Init();			// 初始化压力传感器
	USART_Configuration();	// 串口初始化
	GPIO_MatrixKey_Configuration();
	OLED_Init();
	ISD1820_Init();
	
	OLED_ShowString(0,Y_1," Key:",16);	// x的范围为0-127；y的范围为0-7，字体选择12/16（16占两行）
	OLED_ShowString(0,Y_2," Weight:",16);
//	OLED_ShowNum(0*8,Y_2,11,2,16);		// 如果设置位数比实际位数大，则在最前面补空格 
//	OLED_ShowNum(2*8,Y_3,2345,4,16);
//	OLED_ShowNum(3*8,Y_4,3,1,16);
	
//	Get_Basic();
//	Servo_Run(0);
//	Servo_Run2(0);
//	Servo_Run3(0);
//	Servo_Run4(0);
//	delay_ms(500);
	
	while (1)
	{
//		ISD1820_Alert();
//		delay_ms(4000); 
		
//		Get_Weight();
//		OLED_ShowNum(0*8,Y_3,Weight,4,16);
		delay_ms(50);
	
//		Usart_SendStr(COM0_PORT,"Hello World!");//循环发送字符串，测试用
//		delay_ms(100);
//		Usart_Sendbyte(COM0_PORT,0xff);
//		delay_ms(100);
//		Ultrasonic_Ranging();
		
		key_num = MatrixKey();
		if(key_num)	// 如果按下了按键
		{
			OLED_ShowNum(5*8,Y_1,key_num,2,16);
		} 
		
	}
}
