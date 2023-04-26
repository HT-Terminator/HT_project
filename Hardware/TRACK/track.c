#include "track.h"
#include "pid.h"
#include "pwm.h"
#include "oled.h"
#include "usart.h"
#include "isd1820.h"
#include "MatrixKey.h"
#include "HX711.H"


uint8_t right1_state,right2_state,left1_state,left2_state,middle_state;
uint8_t state_sum;			//五路光电总数

uint8_t across_room;		//记录是否通过房间
uint8_t across_flag;

uint8_t nowroom = 100;	//记录当前房间号

uint8_t stop_state =0;	//记录上一时刻是否是因为遇到障碍物停车
uint8_t stop_state_room = 0;	//记录上一时刻是否是因为送达目的地停车
uint8_t stop_state_startingpoint = 0;	//记录上一时刻是否是因为到达起点停车

int8_t now_turn = 0;				//记录当前那个灯扫到
int8_t pre_stop_state = 0;	//记录停车的前一个转向状态

void TRACK_Init(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PC = 1;
		CKCUClock.Bit.PA = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as input mode                                                                         */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PC, AFIO_PIN_10|AFIO_PIN_0,  AFIO_FUN_GPIO);
		AFIO_GPxConfig(GPIO_PA, AFIO_PIN_10|AFIO_PIN_0|AFIO_PIN_1,  AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_10|GPIO_PIN_0, GPIO_PR_DOWN);
		GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_1, GPIO_PR_DOWN);

    /* Configure GPIO direction as input                                                                    */
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_10|GPIO_PIN_0, GPIO_DIR_IN);
		GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_1, GPIO_DIR_IN);

    /* Enable input function for read                                                                       */
		GPIO_InputConfig(HT_GPIOC, GPIO_PIN_10|GPIO_PIN_0, ENABLE);
		GPIO_InputConfig(HT_GPIOA, GPIO_PIN_10|GPIO_PIN_0|GPIO_PIN_1, ENABLE);
  }
}

void Track_Getbit(void)
{	
	left2_state=GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_10);

	middle_state=GPIO_ReadInBit(HT_GPIOC, GPIO_PIN_0);

	left1_state=GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_10);

	right2_state=GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_0);

	right1_state=GPIO_ReadInBit(HT_GPIOA, GPIO_PIN_1);
	
	state_sum = left1_state+left2_state+middle_state+right1_state+right2_state;
	
	if(left2_state == 1)
		now_turn = -1;
	else if(left1_state == 1)
		now_turn = -1;
	else if(middle_state == 1)
		now_turn = 0;
	else if(right2_state == 1)
		now_turn = 1;
	else if(right1_state == 1)
		now_turn = 1;
	
//	if(distance < 500 && distance != 0)
//	{
//		pre_stop_state = now_turn;
//		ISD1820_Alert();
//	}
	pre_stop_state = now_turn;
		
	if(state_sum >= 3 && across_room == 0)
	{
		across_room = 1;
		state_sum = 0;
		nowroom ++;
		across_flag = 1;
		
		OLED_ShowNum(8*8,Y_3,nowroom,4,16);
	}
	else if(state_sum < 3 && across_flag == 0)
	{
		across_room = 0;
		across_flag = 0;
	}

}

//到达目标房间后停止与开始
void Stop_Begin_room(void)
{
	if(nowroom == RoomNum && pickup == 0)	//到达目的房间并且未取货
	{
		Speedl.kp=70;
		Speedr.kp=70;
		Speedl.target_value=0;
		Speedr.target_value=0;
		stop_state_room =1 ;
	}
	else if(stop_state_room==1 && pickup == 1)
	{
			Speedl.kp=90;
			Speedr.kp=90;
			if( pre_stop_state == 0){
				Speedl.target_value=12;
				Speedr.target_value=12;		
			}
			else if(pre_stop_state == -1){
				Speedl.target_value=0;		//8
				Speedr.target_value=15;		//16
			}
			else if(pre_stop_state == 1){
				Speedl.target_value=15;
				Speedr.target_value=0;
			}
			stop_state_room = 0;
	}
}

//到达终点后停止与开始
void Stop_Begin_StartingPoint(void)
{
	if(nowroom >= 100+RoomSum+1)	//到达起点
	{
		Speedl.kp=70;
		Speedr.kp=70;
		Speedl.target_value=0;
		Speedr.target_value=0;
		
		stop_state_startingpoint =1 ;
		State = 0;
		nowroom = 100;
		OLED_ShowNum(6*8,Y_4,State,4,16);
		OLED_ShowNum(8*8,Y_3,nowroom,4,16);
	}
	else if(State == 1 && stop_state_startingpoint == 1)
	{
		Speedl.kp=90;
		Speedr.kp=90;
		if( pre_stop_state == 0){
			Speedl.target_value=12;
			Speedr.target_value=12;		
		}
		else if(pre_stop_state == -1){
			Speedl.target_value=0;		//
			Speedr.target_value=10;		//15
		}
		else if(pre_stop_state == 1){
			Speedl.target_value=10;	
			Speedr.target_value=0;
		}
		stop_state_startingpoint = 0;
		OLED_ShowNum(6*8,Y_4,State,4,16);
	}

}

#if		1
/*   左2   左1   中间   右1   右2   */
void Track(void){
	
	if(distance < 500 && distance != 0)
	{
		Speedl.kp=70;
		Speedr.kp=70;
		Speedl.target_value=0;
		Speedr.target_value=0;
		stop_state =1 ;
		ISD1820_Alert();
	}
	else if(distance >=500){
		if(stop_state == 1){
			Speedl.kp=90;
			Speedr.kp=90;
			if( pre_stop_state == 0){
				Speedl.target_value=12;
				Speedr.target_value=12;		
			}
			else if(pre_stop_state == -1){
				Speedl.target_value=0;		//8
				Speedr.target_value=15;		//16
			}
			else if(pre_stop_state == 1){
				Speedl.target_value=15;
				Speedr.target_value=0;
			}
			stop_state = 0;
		}
		
		Stop_Begin_room();
		Stop_Begin_StartingPoint();

		//中间
		if(stop_state_room==0&&stop_state_startingpoint==0&&left1_state == notFindBlack && left2_state == notFindBlack && middle_state == findBlack && right1_state == notFindBlack && right2_state == notFindBlack){
			Speedl.kp=52;
			Speedr.kp=52;
			Speedl.target_value=12;
			Speedr.target_value=12;
		}
	
		//右1
		else if(stop_state_room==0&&stop_state_startingpoint==0&&left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == findBlack && right2_state == notFindBlack){
			Speedl.kp=52;
			Speedr.kp=52;
			Speedl.target_value=15;		//22 17
			Speedr.target_value=0;	
		}
	
		//左1
		else if(stop_state_room==0&&stop_state_startingpoint==0&&left1_state == findBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){ 
			Speedl.kp=52;
			Speedr.kp=52;
			Speedl.target_value=0;
			Speedr.target_value=15;
		}
	
		//右2
		else if(stop_state_room==0&&stop_state_startingpoint==0&&left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == findBlack){	
			Speedl.kp=52;
			Speedr.kp=52;
			Speedl.target_value=52; //52
			Speedr.target_value=0;
		}
	
		//左2
		else if(stop_state_room==0&&stop_state_startingpoint==0&&left1_state == notFindBlack && left2_state == findBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
			Speedl.kp=52;
			Speedr.kp=52;
			Speedl.target_value=0;
			Speedr.target_value=52;	//52	
		}
	}
}
#endif


#if 0
/*   左2   左1   中间   右1   右2   */
void Track(void){
	Track_Getbit();
	//中间
	if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == findBlack && right1_state == notFindBlack && right2_state == notFindBlack){
		Dir.actual_value = 0;	

	}
	
	//右1
	 else if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == findBlack && right2_state == notFindBlack){
		Dir.actual_value = 2;
		
	}
	
	//左1
	 else if(left1_state == findBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
		Dir.actual_value = -1; 
	}
	
	//右2
	else if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == findBlack){
		Dir.actual_value = 3;	
	}
	
	//左2
	else if(left1_state == notFindBlack && left2_state == findBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
		Dir.actual_value = -2;
	}
	
	//不在黑线上时
	else if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
		if(Dir.actual_value == 2)
			Dir.actual_value = 2;
		if(Dir.actual_value == -2)
			Dir.actual_value = -2;
//	}
	OLED_ShowNum(8*8,Y_2,Dir.actual_value,2,16);
//	state_sum = left1_state+left2_state+middle_state+right1_state+right2_state;
	
//	if(state_sum == 3)
//	{
//		across_room = 1;
//		state_sum = 0;
//		
//	}
//	
//	if(across_room)
//	{
//		roomnum ++;
//		across_room = 0;
//	}
//	
//	if(roomnum > roomsum)
//	{
//			SetMotor_Stop();
//			roomnum = 0;
//	}


}
	 }
#endif

