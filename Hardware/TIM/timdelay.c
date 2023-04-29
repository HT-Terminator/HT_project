#include "pwm.h"
#include "time.h"
#include "timdelay.h"
#include "track.h"
#include "HX711.h"
#include "MQTT.h"
#include "Cloud.h"
#include "usart.h"
#include "MatrixKey.h"

unsigned int sctm_control_30ms;
unsigned int sctm_encoder_30ms;
unsigned int sctm_usart0_100ms;
unsigned int sctm_across_200ms;
unsigned int sctm_servos1_num;
unsigned int sctm_servos2_num;
unsigned int sctm_weight_500ms;
unsigned int sctm_pickup_2500ms;
unsigned int sctm_nopickup_10000ms;

unsigned int sctm_mqtt_num;
unsigned int sctm_cloud_500ms;

unsigned int sctm_left_500ms;


unsigned char pwm_pc1_value = 0;// 舵机信号线的值
unsigned char pwm_pc5_value = 0;

void TM_Configuration(void)
{
  #if 0 // Use following function to configure the IP clock speed.
  CKCU_SetPeripPrescaler(CKCU_PCLK_GPTM0, CKCU_APBCLKPRE_DIV2);
  #endif

  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.HTCFG_TM_IPN = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Time base configuration                                                                              */

    TM_TimeBaseInitTypeDef TimeBaseInit;

    TimeBaseInit.Prescaler = 1 - 1;                         					// Timer clock = CK_AHB / 1
    TimeBaseInit.CounterReload = SystemCoreClock / TM_FREQ_HZ - 1;		//SystemCoreClock=48MHz,TM_FREQ_HZ=2000,即0.5ms
    TimeBaseInit.RepetitionCounter = 0;
    TimeBaseInit.CounterMode = TM_CNT_MODE_UP;
    TimeBaseInit.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HTCFG_TM_PORT, &TimeBaseInit);

    /* Clear Update Event Interrupt flag since the "TM_TimeBaseInit()" writes the UEV1G bit                 */
    TM_ClearFlag(HTCFG_TM_PORT, TM_FLAG_UEV);
  }

  /* Enable Update Event interrupt                                                                          */
  NVIC_EnableIRQ(HTCFG_TM_IRQn);
  TM_IntConfig(HTCFG_TM_PORT, TM_INT_UEV, ENABLE);

  TM_Cmd(HTCFG_TM_PORT, ENABLE);
}

//中断服务函数
void HTCFG_TM_IRQHandler(void)
{
  if (TM_GetIntStatus(HTCFG_TM_PORT, TM_INT_UEV) != RESET)
	{
			TM_ClearFlag(HTCFG_TM_PORT, TM_INT_UEV);
			sctm_control_30ms++;
			sctm_encoder_30ms++;
			sctm_usart0_100ms++;
			sctm_servos1_num++;
			sctm_servos2_num++;
		  sctm_weight_500ms++;
			sctm_mqtt_num++;
			sctm_cloud_500ms++;
		
			if(sctm_mqtt_num>=20*2&&flag_mqqt)
			{
				memcpy(&MQTT_RxDataInPtr[2],Usart1_RxBuff,Usart1_RxCounter);  //拷贝数据到接收缓冲区
				MQTT_RxDataInPtr[0] = Usart1_RxCounter/256;                   //记录数据长度高字节
				MQTT_RxDataInPtr[1] = Usart1_RxCounter%256;                   //记录数据长度低字节
				MQTT_RxDataInPtr+=BUFF_UNIT;                                  //指针下移
				if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                       //如果指针到缓冲区尾部了
					MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                     //指针归位到缓冲区开头
				Usart1_RxCounter = 0;                                         //串口2接收数据量变量清零
				sctm_mqtt_num = 0;
				flag_mqqt = 0;
			}
			
			if(left2_state == 1)
			{
				sctm_left_500ms++;
			}
		
			if(pickup == 1)	//如果物品被拿起来，计时2500ms
			{
				sctm_pickup_2500ms++;
			}
		
			if(across_flag == 1)
				sctm_across_200ms++;
			if(sctm_across_200ms>=200*2 && across_flag == 1)
			{
				sctm_across_200ms = 0;
				across_flag = 0;
			}
			
			if(State == 2)	//送达后开始计时，10s未取货则派送下一家
			{
				sctm_nopickup_10000ms++;
			}
			
			if((pwm_pc5_value == 1) && sctm_servos2_num >= 40 - timR_num_high)
			{
				sctm_servos2_num = 0;
				GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_5, SET);
				pwm_pc5_value = !pwm_pc5_value;
			}
			else if((pwm_pc5_value == 0) && sctm_servos2_num >= timR_num_high)
			{
				sctm_servos2_num = 0;
				GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_5, RESET);
				pwm_pc5_value = !pwm_pc5_value;
			}
	
			if((pwm_pc1_value == 1) && sctm_servos1_num >= 40 - timL_num_high)
			{
				sctm_servos1_num = 0;
				GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, SET);
				pwm_pc1_value = !pwm_pc1_value;
			}
			else if((pwm_pc1_value == 0) && sctm_servos1_num >= timL_num_high)
			{
				sctm_servos1_num = 0;
				GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, RESET);
				pwm_pc1_value = !pwm_pc1_value;
			}			
	}
}









