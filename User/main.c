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
<<<<<<< HEAD
#include "track.h"
#include "pid.h"
#include "MQTT.h"
#include "Cloud.h"


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
  
unsigned char key_num = 0;
float  PWML=0,PWMR=0;  

void MatrixKey_proc(void);
void servos_proc(void);
void encoder_process_100ms(void);
void control_100ms(void);
void get_weight_500ms(void);
void cloud_process_500ms(void);
//void state_show(void);
//void cloud_init(void);
=======

void MatrixKey_proc(void);
void Servos_proc(void);
>>>>>>> 00baff4cd54284260955c2d0773a16d4d36686ed
  
unsigned char key_num = 0;		// 矩阵键盘的按键值


int main(void)
{
<<<<<<< HEAD
	NVIC_SetPriority(UART1_IRQn,(1<<__NVIC_PRIO_BITS) - 1);
	NVIC_SetPriority(SCTM0_IRQn,(1<<__NVIC_PRIO_BITS) - 2);
	MCTM_PWM_init();		// pwm初始化
	HX711_Init();			// 初始化压力传感器
	USART_Configuration();	// 串口初始化
	UART1_Configuration();
	TM_Capture_Configuration();
	GPIO_MatrixKey_Configuration();
	OLED_Init();
	ISD1820_Init();
	TRACK_Init();
	Pidinit();
	GPIO_Key_Configuration();
	GPIO_ServosPwm_Init(); //舵机初始化
	MQTT_Buff_Init();//RX|TX|CMD发送与接收数组的初始化及各个状态标志位的初始化

	OLED_ShowString(0,Y_3," nowroom:",16);	// x的范围为0-127；y的范围为0-7，字体选择12/16（16占两行）
//	OLED_ShowString(0,Y_2," ",16);
//	OLED_ShowString(0,Y_3," state:",16);
	OLED_ShowString(0,Y_1," room:",16);
	OLED_ShowString(0,Y_4," W:",16);
	OLED_ShowString(8*8,Y_4," $:",16);
	cloud_init();
//	OLED_ShowNum(0*8,Y_2,11,2,16);		// 如果设置位数比实际位数大，则在最前面补空格 
//	OLED_ShowNum(2*8,Y_3,2345,4,16);
//	OLED_ShowNum(3*8,Y_4,3,1,16);
//   	SetMotor_R(1500);		//设置占空比
//	  SetMotor_L(1500);
//	
//	Get_Basic();
//			for(int i = 0;i<20;i++){
//			delay_ms(100);
//			}
	Get_Basic();
	TM_Configuration();
	while (1)
	{
		get_weight_500ms();
		MatrixKey_proc();
		Track_Getbit();
//		Handle_buff_about_cloud();	//云端处理
		if(State == 1){
			Track();
			control_100ms();
			Ultrasonic_Ranging();	
			
			encoder_process_100ms();
//			control_100ms();
		}
		else if(State == 0 || State == 2){
			Speedl.target_value=0;
			Speedr.target_value=0;
			encoder_process_100ms();
			control_100ms();
		}
	Handle_buff_about_cloud();	//云端处理		
	Cloud_All_Msg_Push();
	}
}

void servos_proc()
{
		ServosL_control(CLOSE);
		ServosR_control(CLOSE);
=======
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
>>>>>>> 00baff4cd54284260955c2d0773a16d4d36686ed
}

void MatrixKey_proc()
{
	key_num = MatrixKey();
	if(key_num)	// 如果按下了按键
	{
		KeyProcess(key_num);
//		OLED_ShowNum(5*8,Y_1,key_num,2,16);
	} 
}

float outputtestl;
float outputtestr;
//float actualtestl;
//float actualtestr;

void control_100ms(void)
{
		if(sctm_control_30ms > 10*2){
	//		Track();

			Speedl.output_last = Speedl.output;
			Speedr.output_last = Speedr.output;
			Speedl.actual_value = Speed_data.actual_value_l;
			Speedr.actual_value = Speed_data.actual_value_r;
			
//			actualtestl = Speed_data.actual_value_l;
//			actualtestr = Speed_data.actual_value_r;
//			
//			Direction_pid_ctrl(Dir.target_value,&Dir);//转向环
//			
//			Speedl.target_value = (12-Dir.output);
//  		Speedr.target_value = (12+Dir.output);
			
			Speed_pid_ctrl(Speedl.target_value,&Speedl);//速度环
			Speed_pid_ctrl(Speedr.target_value,&Speedr);

			PWML += Speedl.output;	//Speed1.output属于err算出来的值，所以这里要用减等于
			PWMR += Speedr.output;
			
		//2023-3-29调试添加，因为如果不增加该语句，会出一种情况：PWML和PWMR在某段时刻一直增大，然后进到MotorOutput函数中使得控制在一段时间一直处于MAX值
		if(PWML > 4700)
		{
			PWML = 4700;
		}
		if(PWML < 200)
		{
			PWML = 100;
		}
		if(PWMR > 4700)
		{
			PWMR = 4700;
		}
		if(PWMR < 200)
		{
			PWMR = 100;
		}
			
			MotorOutput(PWML,PWMR);

			sctm_control_30ms = 0;

 }
}

void encoder_process_100ms(void)
{
		if(sctm_encoder_30ms > 30*2){
			get_encoder_data();
			encoder_data_process();
		
//			OLED_ShowNum(9*8,Y_4,Speed_data.actual_value_l,5,16);
			
			sctm_encoder_30ms=0;
			
		}
}

void get_weight_500ms(void)
{
		if(sctm_weight_500ms > 1000*2){
			Get_Weight();
			sctm_weight_500ms = 0;
		}
}

void cloud_process_500ms(void)
{
		if(sctm_cloud_500ms >=100*2)
		{
			Handle_buff_about_cloud();	//云端处理
			sctm_cloud_500ms=0;
		}
}



//void state_show(void)
//{
//	if(State == 0)
//		OLED_ShowString(7*8,Y_2,"spare",16);
//	else if(State == 1)
//		OLED_ShowString(7*8,Y_2,"running",16);
//	else if(State == 2)
//		OLED_ShowString(7*8,Y_2,"notake",16);
//}

