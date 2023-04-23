#include "pwm.h"
#include "time.h"
#include "timdelay.h"

unsigned int sctm_state1;
unsigned int sctm_state2;
unsigned int sctm_state3;
unsigned int sctm_state4;
unsigned int sctm_servos1_num;
unsigned int sctm_servos2_num;

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

//中断服务函数,0.5ms中断一次
void HTCFG_TM_IRQHandler(void)
{
  if (TM_GetIntStatus(HTCFG_TM_PORT, TM_INT_UEV) != RESET)
	{
		TM_ClearFlag(HTCFG_TM_PORT, TM_INT_UEV);
		sctm_state1++;
		sctm_state2++;
		sctm_state3++;
		sctm_state4++;  
		sctm_servos1_num++;
		sctm_servos2_num++;
	}
}

