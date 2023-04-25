#include "track.h"
#include "pid.h"
#include "pwm.h"
#include "oled.h"


uint8_t right1_state,right2_state,left1_state,left2_state,middle_state;
uint8_t state_sum;
uint8_t across_room;

uint8_t roomnum = 0;

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

}

#if		1
/*   左2   左1   中间   右1   右2   */
void Track(void){
	Track_Getbit();
	//中间
	if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == findBlack && right1_state == notFindBlack && right2_state == notFindBlack){
//		Dir.actual_value = 0;	
		Speedl.target_value=12;
		Speedr.target_value=12;
	}
	
	//右1
	else if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == findBlack && right2_state == notFindBlack){
//		Dir.actual_value = 1;
		Speedl.target_value=17;		//22
		Speedr.target_value=0;		
	}
	
	//左1
	else if(left1_state == findBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
//		Dir.actual_value = -1; 
		Speedl.target_value=0;
		Speedr.target_value=17;
	}
	
	//右2
	else if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == findBlack){
//		Dir.actual_value = 2;	
		Speedl.target_value=52;
		Speedr.target_value=0;
	}
	
	//左2
	else if(left1_state == notFindBlack && left2_state == findBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
//		Dir.actual_value = -2;
		Speedl.target_value=0;
		Speedr.target_value=52;		
	}
	
	//不在黑线上时
//	else if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
//		if(Dir.actual_value == 2)
//			Dir.actual_value = 2;
//		if(Dir.actual_value == -2)
//			Dir.actual_value = -2;
//	}
	//OLED_ShowNum(8*8,Y_2,Dir.actual_value,2,16);
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

