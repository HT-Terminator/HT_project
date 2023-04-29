#include "MatrixKey.h"
#include "delay.h"
#include "oled.h"
#include "track.h"
#include "HX711.h"
#include "pwm.h"
#include "Cloud.h"

unsigned char State = 0;	//全过程状态记录 0：停车 1：开车 2：待取货 
unsigned char sale_state;	//售货与送货状态	0：送货，1：售货

unsigned int Count ;
unsigned int RoomNum ;

unsigned int WeightDelivey = 1000;
unsigned int WeightSale = 1000;

void GPIO_MatrixKey_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.AFIO = 1;
	CKCUClock.Bit.PA = 1;
	CKCUClock.Bit.PB = 1;
	CKCUClock.Bit.PC = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as input mode                                                                         */

    /* Configure AFIO mode as GPIO                                                                          */
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_FUN_GPIO);
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_3|AFIO_PIN_5|AFIO_PIN_7, AFIO_FUN_GPIO);
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_3|AFIO_PIN_8|AFIO_PIN_9|AFIO_PIN_15, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_15, GPIO_PR_UP);	// 上拉
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, GPIO_PR_UP);
    GPIO_PullResistorConfig(HT_GPIOC, GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, GPIO_PR_UP);

    /* Configure GPIO direction as input                                                                    */
    GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_IN);
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, GPIO_DIR_IN);
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, GPIO_DIR_IN);
	  
    /* Configure GPIO direction as ouput                                                                    */
    GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_15, GPIO_DIR_OUT);
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, GPIO_DIR_OUT);
    GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, GPIO_DIR_OUT);
	  
    /* Enable input function for read                                                                       */
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_15, ENABLE);
    GPIO_InputConfig(HT_GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7, ENABLE);
    GPIO_InputConfig(HT_GPIOC, GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15, ENABLE);
  }
}

void GPIO_Key_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
	CKCUClock.Bit.AFIO = 1;
	CKCUClock.Bit.PA = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as input mode                                                                         */

    /* Configure AFIO mode as GPIO                                                                          */
	AFIO_GPxConfig(GPIO_PA, AFIO_PIN_11, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
	GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_11, GPIO_PR_UP);	// 上拉

    /* Configure GPIO direction as input                                                                    */
    GPIO_DirectionConfig(HT_GPIOA, GPIO_PIN_11, GPIO_DIR_IN);
	  
	  
    /* Enable input function for read                                                                       */
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_11, ENABLE);

  }
}

void read_key(void)
{
	if(P1_11==0)
	{
		while(P1_11==0){}
		delay_ms(10);
		if(State == 0)
			State = 1;
		else 
			State = 0;
	}

}
/**
  * @brief  矩阵键盘读取按键键码
  * @param  无
  * @retval KeyNumber 按下按键的键码值
			如果按键按下不放，程序会停留在此函数，松手的一瞬间，返回按键键码，没有按键按下时，返回0
  */
unsigned char MatrixKey()
{
	unsigned char KeyNumber=0;
	
	P1_SET(); // P1=0xFF;
	P1_3_RESET(); // P1_3=0; 
	if(P1_4==0){delay_ms(10);while(P1_4==0){};delay_ms(10);KeyNumber=1;}
	if(P1_5==0){delay_ms(10);while(P1_5==0){};delay_ms(10);KeyNumber=2;}
	if(P1_6==0){delay_ms(10);while(P1_6==0){};delay_ms(10);KeyNumber=3;}
	if(P1_7==0){delay_ms(10);while(P1_7==0){};delay_ms(10);KeyNumber=4;}
	
	P1_SET(); // P1=0xFF;
	P1_2_RESET(); // P1_2=0;
	if(P1_4==0){delay_ms(10);while(P1_4==0){};delay_ms(10);KeyNumber=5;}
	if(P1_5==0){delay_ms(10);while(P1_5==0){};delay_ms(10);KeyNumber=6;}
	if(P1_6==0){delay_ms(10);while(P1_6==0){};delay_ms(10);KeyNumber=7;}
	if(P1_7==0){delay_ms(10);while(P1_7==0){};delay_ms(10);KeyNumber=8;}
	
	P1_SET(); // P1=0xFF;
	P1_1_RESET(); // P1_1=0;
	if(P1_4==0){delay_ms(10);while(P1_4==0){};delay_ms(10);KeyNumber=9;}
	if(P1_5==0){delay_ms(10);while(P1_5==0){};delay_ms(10);KeyNumber=10;}
	if(P1_6==0){delay_ms(10);while(P1_6==0){};delay_ms(10);KeyNumber=11;}
	if(P1_7==0){delay_ms(10);while(P1_7==0){};delay_ms(10);KeyNumber=12;}
	
	P1_SET(); // P1=0xFF;
	P1_0_RESET(); // P1_0=0;
	if(P1_4==0){delay_ms(10);while(P1_4==0){};delay_ms(10);KeyNumber=13;}
	if(P1_5==0){delay_ms(10);while(P1_5==0){};delay_ms(10);KeyNumber=14;}
	if(P1_6==0){delay_ms(10);while(P1_6==0){};delay_ms(10);KeyNumber=15;}
	if(P1_7==0){delay_ms(10);while(P1_7==0){};delay_ms(10);KeyNumber=16;}
	
	return KeyNumber;
}

void KeyProcess(unsigned char KeyNum)
{
		if(KeyNum<=10)	//如果S1~S10按键按下，输入密码
			{
				if(Count<3)	//如果输入次数小于3
				{
					RoomNum*=10;					//数据左移一位
					RoomNum+=KeyNum%10;		//获取一位数据
					Count++;	//计次加一
				}
				OLED_ShowNum(6*8,Y_1,RoomNum,4,16);//更新显示
			}
			if(KeyNum==11)	//如果S11按键按下，确认
			{
				if(RoomNum == 101 || RoomNum == 102 || RoomNum == 103 || RoomNum == 104)	//如果房间号等于正确房间号
				{
					OLED_ShowString(0*8,Y_2,"right",16);//显示正确
//					RoomNum=0;		//密码清零
					Count=0;		//计次清零
					pickupover = 0;
					pickup = 0;
//					OLED_ShowNum(6*8,Y_1,RoomNum,4,16);//更新显示
				}
				else				//否则
				{
					OLED_ShowString(0*8,Y_2,"error",16);//显示N,表错误
					RoomNum=0;		//密码清零
					Count=0;		//计次清零
					OLED_ShowNum(6*8,Y_1,RoomNum,4,16);//更新显示
				}
				if(State == 0)
					State = 1;
				else 
					State = 0;
//				OLED_ShowNum(6*8,Y_4,State,1,16);
				state_show();
				
		//		Get_Weight();
				stop_state_room=0;
				stop_state_startingpoint=0;
				WeightDelivey = Weight_Medicine_1;	//获取送货重量
				sale_state = 0;
				Send.room = RoomNum;
				Send.Distance = RoomNum - nowroom;
				Send.Time = Send.Distance*5;
				Send.Change_Flag = 1;
//				Cloud_All_Msg_Push();
				
			}
			if(KeyNum==12)	//如果S12按键按下，初始化
			{
				RoomNum=0;		//密码清零
				Count=0;		//计次清零
				OLED_ShowString(0*8,Y_2,"input",16);//显示N,表错误
				OLED_ShowNum(6*8,Y_1,RoomNum,4,16);//更新显示
				State = 0;
				pickup = 0;
				pickupover = 0;
				saleprice = 0;
//				Send.room = 0;
//				Send.Distance = 0;
//				Send.Time = Send.Distance*5;
//				Sell.room = 0;
//				Sell.Distance = 0;
//				Sell.Time = Sell.Distance*5;
//				Send.State = 0;
//				Sell.State = 0;
//				Sell.Change_Flag = 1;
//				Send.Change_Flag = 1;
////				Cloud_All_Msg_Push();
//				Cloud_All_Msg_Push();
				cloud_init();
				
//				OLED_ShowNum(6*8,Y_4,State,1,16);
				state_show();
				OLED_ShowNum(10*8,Y_4,saleprice,3,16);
				nowroom = 100;
				OLED_ShowNum(12*6,Y_3,nowroom,3,16);
			}
			if(KeyNum==13)	//如果S13按键按下，继续前进
			{
				pickupover = 1;
			}
			if(KeyNum==14)
			{
				if(sale_state == 1)
				{
					ServosL_control(OPEN);
	//				sale_state = 0;
				}
				else if(sale_state == 0)
				{
					ServosR_control(OPEN);
	//				sale_state = 1;	//由云端发送售货信息后更改此标志位				
				}
			}
			if(KeyNum==15)	//售货功能开关
			{
				WeightSale = Weight_Medicine_1;	//获取售货重量
				sale_state = 1; //等待云端发送售货信号，该标志位置1
				State = 1;
				state_show();		
			}
			
}
