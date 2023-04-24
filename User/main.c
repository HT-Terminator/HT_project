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
#include "track.h"
#include "pid.h"


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
  
unsigned char key_num = 0;
float  PWML=0,PWMR=0;  

void MatrixKey_proc(void);
void encoder_process_100ms(void);
void control_100ms(void);
  
int main(void)
{
	MCTM_PWM_init();		// pwm初始化
	HX711_Init();			// 初始化压力传感器
	USART_Configuration();	// 串口初始化
	TM_Capture_Configuration();
	GPIO_MatrixKey_Configuration();
	OLED_Init();
	ISD1820_Init();
	TRACK_Init();
	Pidinit();
	
	OLED_ShowString(0,Y_1," Key:",16);	// x的范围为0-127；y的范围为0-7，字体选择12/16（16占两行）
	OLED_ShowString(0,Y_2," Weight:",16);
	OLED_ShowString(0,Y_3," Distance:",16);
	OLED_ShowString(0,Y_4," Encoder:",16);
//	OLED_ShowNum(0*8,Y_2,11,2,16);		// 如果设置位数比实际位数大，则在最前面补空格 
//	OLED_ShowNum(2*8,Y_3,2345,4,16);
//	OLED_ShowNum(3*8,Y_4,3,1,16);
//   	SetMotor_R(1500);		//设置占空比
//	  SetMotor_L(1500);
//	
//	Get_Basic();
//	Servo_Run(0);
//	Servo_Run2(0);
//	Servo_Run3(0);
//	Servo_Run4(0);
//			for(int i = 0;i<20;i++){
//			delay_ms(100);
//			}
//	Get_Basic();
	TM_Configuration();
	while (1)
	{
		encoder_process_100ms();
		control_100ms();
	//	encoder_process_100ms();
		
//		ISD1820_Alert();
//		delay_ms(4000); 
//		Read_Encoder(0);	
//		get_encoder_data();
//		encoder_data_process();
//		
//		OLED_ShowNum(9*8,Y_4,Speed_data.actual_value_l,5,16);
//				delay_ms(100);
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
//  	Track_Getbit();
//		key_num = MatrixKey();
//		if(key_num)	// 如果按下了按键
//		{
//			OLED_ShowNum(5*8,Y_1,key_num,2,16);
//		} 
//		
	}
}


void MatrixKey_proc()
{
	key_num = MatrixKey();
	if(key_num)	// 如果按下了按键
	{
		OLED_ShowNum(5*8,Y_1,key_num,2,16);
	} 
}

float outputtestl;
float outputtestr;
//float actualtestl;
//float actualtestr;

void control_100ms(void)
{
		if(sctm_10ms > 100*2){
			Track();

			Speedl.output_last = Speedl.output;
			Speedr.output_last = Speedr.output;
			Speedl.actual_value = Speed_data.actual_value_l;
			Speedr.actual_value = Speed_data.actual_value_r;
			
//			actualtestl = Speed_data.actual_value_l;
//			actualtestr = Speed_data.actual_value_r;
			
			Direction_pid_ctrl(Dir.target_value,&Dir);//转向环
			
			Speedl.target_value = (Speedl.target_value+Dir.output);
  		Speedr.target_value = (Speedl.target_value-Dir.output);
			
			Speed_pid_ctrl(Speedl.target_value,&Speedl);//速度环
			Speed_pid_ctrl(Speedr.target_value,&Speedr);

			PWML += Speedl.output;	//Speed1.output属于err算出来的值，所以这里要用减等于
			PWMR += Speedr.output;
			
		//2023-3-29调试添加，因为如果不增加该语句，会出一种情况：PWML和PWMR在某段时刻一直增大，然后进到MotorOutput函数中使得控制在一段时间一直处于MAX值
		if(PWML > 4000)
		{
			PWML = 4000;
		}
		if(PWML < 100)
		{
			PWML = 100;
		}
		if(PWMR > 4000)
		{
			PWMR = 4000;
		}
		if(PWMR < 100)
		{
			PWMR = 100;
		}
			
			MotorOutput(PWML,PWMR);

			sctm_10ms = 0;

 }
}

void encoder_process_100ms(void)
{
		if(sctm_100ms > 100*2){
			get_encoder_data();
			encoder_data_process();
		
			OLED_ShowNum(9*8,Y_4,Speed_data.actual_value_l,5,16);
			
			sctm_100ms=0;
			
		}

}
