#include "track.h"
#include "pid.h"


uint8_t right1_state,right2_state,left1_state,left2_state,middle_state;

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

/*   左2   左1   中间   右1   右2   */
void Track(){
	//中间
	if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == findBlack && right1_state == notFindBlack && right2_state == notFindBlack){
		Dir.actual_value = 0;	
	}
	
	//右1
	else if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == findBlack && right2_state == notFindBlack){
		Dir.actual_value = 1;	
//		SetMotor_L(300,0);
//		SetMotor_R(900,0);
	}
	
	//左1
	else if(left1_state == findBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
		Dir.actual_value = -1; 
//		SetMotor_L(900,0);
//		SetMotor_R(300,0);
	}
	
	//右2
	else if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == findBlack){
		Dir.actual_value = 2;	
	}
	
	//左2
	else if(left1_state == notFindBlack && left2_state == findBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
		Dir.actual_value = -2;	
	}
	
	//不在黑线上时
	else if(left1_state == notFindBlack && left2_state == notFindBlack && middle_state == notFindBlack && right1_state == notFindBlack && right2_state == notFindBlack){
		if(Dir.actual_value == 2)
			Dir.actual_value = 3;
		if(Dir.actual_value == -2)
			Dir.actual_value = -3;
	}
	
		//方向环
		Speedl.target_value = (-20+Dir.output);
		Speedr.target_value = (20+Dir.output);
		
//		Speedl.kp = kpSpeed;
//    Speedl.ki = kiSpeed;
//		
//		Speedr.kp = Speedl.kp;
//		Speedr.ki = Speedl.ki;
//		
//		Dir.kp = kpDir;
//		Dir.kd = kdDir;
		
		Dir.output_Max = 20;
		Dir.output_Min = -20;

}


