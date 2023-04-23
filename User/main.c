#include "ht32.h"
#include "usart.h"
#include "pwm.h"
#include "delay.h"
#include "hx711.h"
#include "encoder.h"
#include "MatrixKey.h"
#include "oled.h"
#include "isd1820.h"
#include "timdelay.h"

void MatrixKey_proc(void);
void Servos_proc(void);
  
unsigned char key_num = 0;		// 矩阵键盘的按键值


int main(void)
{
	MCTM_PWM_init();				// pwm初始化
	HX711_Init();					// 初始化压力传感器
	USART_Configuration();			// 串口初始化
	TM_Capture_Configuration();		// 电机编码器初始化
	TM_Configuration();				// sctm0初始化
	GPIO_MatrixKey_Configuration();	// 矩阵键盘初始化
	GPIO_ServosPwm_Init();			// 舵机初始化
	OLED_Init();					// OLED初始化
	ISD1820_Init();					// 语音模块初始化
	
	OLED_ShowString(0,Y_1," Key:",16);	// x的范围为0-127；y的范围为0-7，字体选择12/16（16占两行）
	OLED_ShowString(0,Y_2," Weight:",16);
	OLED_ShowString(0,Y_3," Distance:",16);
	OLED_ShowString(0,Y_4," PULSE:",16);
//	OLED_ShowNum(0*8,Y_2,11,2,16);		// 如果设置位数比实际位数大，则在最前面补空格 
//	OLED_ShowNum(2*8,Y_3,2345,4,16);
//	OLED_ShowNum(3*8,Y_4,3,1,16);
	SetMotor_R(20);		//设置占空比
	SetMotor_L(20);

	delay_ms(2000);
	Get_Basic();
	
	while (1)
	{
//		ISD1820_Alert();
//		delay_ms(4000); 
		
//		Read_Encoder(1);
//		OLED_ShowNum(8*8,Y_4,Encoder_TIM,7,16);
//		delay_ms(100);
		
//		Get_Weight();
//		OLED_ShowNum(8*8,Y_2,Weight_Medicine_1,4,16);
		
//		OLED_ShowNum(0*8,Y_3,Weight,4,16);
//		delay_ms(50);
	
//		Usart_SendStr(COM0_PORT,"Hello World!");//循环发送字符串，测试用
//		delay_ms(100);
//		Usart_Sendbyte(COM0_PORT,0xff);
//		delay_ms(100);
//		Ultrasonic_Ranging();
//		OLED_ShowNum(10*8,Y_3,distance,4,16);
		
		Servos_proc();
	}
}

void Servos_proc()
{
	servos1_angle(45);
	servos2_angle(45);
}

void MatrixKey_proc()
{
	key_num = MatrixKey();
	if(key_num)	// 如果按下了按键
	{
		OLED_ShowNum(5*8,Y_1,key_num,2,16);
	} 
}
