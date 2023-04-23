#include "pwm.h"
#include "time.h"

unsigned char pwm_tim_mun = 2;
unsigned char pwm_pin_value = 1,tim1_num1; // 

//输出四路PWM波控制后轮两个电机
//arr：自动重装值
//psc：时钟预分频数
//MT   PA14 PB0 PB2 PD3
//产生一个20ms 定时周期 
void MCTM_PWM_init()
{
	TM_TimeBaseInitTypeDef MCTM_TimeBaseIniture;   //结构体
	TM_OutputInitTypeDef   MCTM_OutputInitStructure;

	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	//CKCUClock.Bit.PA         = 1;       //GPIOA时钟打开
	CKCUClock.Bit.AFIO       = 1;       //复用时钟
	CKCUClock.Bit.MCTM0      = 1;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);      //时钟使能
		
<<<<<<< HEAD
	/* Configure MCTM Channel 0 output pin                                                                    */
	AFIO_GPxConfig(HTCFG_MCTM_CH0_GPIO_ID, HTCFG_MCTM_CH0_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
	/* Configure MCTM Channel 1 output pin                                                                    */
	AFIO_GPxConfig(HTCFG_MCTM_CH1_GPIO_ID, HTCFG_MCTM_CH1_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
	/* Configure MCTM Channel 2 output pin                                                                    */
	AFIO_GPxConfig(HTCFG_MCTM_CH2_GPIO_ID, HTCFG_MCTM_CH2_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
	/* Configure MCTM Break pin                                                                               */
	AFIO_GPxConfig(HTCFG_MCTM_CH3_GPIO_ID, HTCFG_MCTM_CH3_AFIO_PIN, AFIO_FUN_MCTM_GPTM);

	{ /* Configure AFIO mode as GPIO                                                                          */
		AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1|AFIO_PIN_5, AFIO_FUN_GPIO);

		//    MCTM_TimeBaseIniture.CounterMode = TM_CNT_MODE_UP;            //边沿对齐向上模式
		//    MCTM_TimeBaseIniture.CounterReload = 200-1;                   //计数重装载计数器
		//    MCTM_TimeBaseIniture.Prescaler = 4800 -1;                      //预分频系数
		//    MCTM_TimeBaseIniture.PSCReloadTime=TM_PSC_RLD_IMMEDIATE;    //立即重装载
		//    TM_TimeBaseInit(HT_GPTM0,&MCTM_TimeBaseIniture);
			
		/* MCTM Time Base configuration                                                                           */
		MCTM_TimeBaseIniture.CounterReload = HTCFG_MCTM_RELOAD - 1;
		MCTM_TimeBaseIniture.Prescaler = 0;
		MCTM_TimeBaseIniture.RepetitionCounter = 0;
		MCTM_TimeBaseIniture.CounterMode = TM_CNT_MODE_UP;
		MCTM_TimeBaseIniture.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
		TM_TimeBaseInit(HT_MCTM0, &MCTM_TimeBaseIniture);
		TM_CRRPreloadCmd(HT_MCTM0, ENABLE);

		/* MCTM Channel 0, 1, 2 3output configuration                                                              */
		MCTM_OutputInitStructure.Channel = TM_CH_0;
		MCTM_OutputInitStructure.OutputMode = TM_OM_PWM1;
		MCTM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
		MCTM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
		MCTM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
		MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 1;	//占空比1
		MCTM_OutputInitStructure.AsymmetricCompare = 0;
		TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

		MCTM_OutputInitStructure.Channel = TM_CH_1;
		MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 0;
		MCTM_OutputInitStructure.Polarity = TM_CHP_INVERTED;
		TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

		MCTM_OutputInitStructure.Channel = TM_CH_2;
		MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 1 ;
		MCTM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
		TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

		MCTM_OutputInitStructure.Channel = TM_CH_3;
		MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 0;
		MCTM_OutputInitStructure.Polarity = TM_CHP_INVERTED;
		TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);	

		/* MCTM counter enable                                                                                    */
		TM_Cmd(HT_MCTM0, ENABLE);

		/* MCTM Channel Main Output enable                                                                        */
		MCTM_CHMOECmd(HT_MCTM0, ENABLE);
	}
=======
  /* Configure MCTM Channel 0 output pin                                                                    */
  AFIO_GPxConfig(HTCFG_MCTM_CH0_GPIO_ID, HTCFG_MCTM_CH0_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
   /* Configure MCTM Channel 1 output pin                                                                    */
  AFIO_GPxConfig(HTCFG_MCTM_CH1_GPIO_ID, HTCFG_MCTM_CH1_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
   /* Configure MCTM Channel 2 output pin                                                                    */
  AFIO_GPxConfig(HTCFG_MCTM_CH2_GPIO_ID, HTCFG_MCTM_CH2_AFIO_PIN, AFIO_FUN_MCTM_GPTM);
    /* Configure MCTM Break pin                                                                               */
  AFIO_GPxConfig(HTCFG_MCTM_CH3_GPIO_ID, HTCFG_MCTM_CH3_AFIO_PIN, AFIO_FUN_MCTM_GPTM);

  { /* Configure AFIO mode as GPIO                                                                          */
	AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1|AFIO_PIN_5, AFIO_FUN_GPIO);

//    MCTM_TimeBaseIniture.CounterMode = TM_CNT_MODE_UP;            //边沿对齐向上模式
//    MCTM_TimeBaseIniture.CounterReload = 200-1;                   //计数重装载计数器
//    MCTM_TimeBaseIniture.Prescaler = 4800 -1;                      //预分频系数
//    MCTM_TimeBaseIniture.PSCReloadTime=TM_PSC_RLD_IMMEDIATE;    //立即重装载
//    TM_TimeBaseInit(HT_GPTM0,&MCTM_TimeBaseIniture);
		
	/* MCTM Time Base configuration                                                                           */
  MCTM_TimeBaseIniture.CounterReload = HTCFG_MCTM_RELOAD - 1;
  MCTM_TimeBaseIniture.Prescaler = 0;
  MCTM_TimeBaseIniture.RepetitionCounter = 0;
  MCTM_TimeBaseIniture.CounterMode = TM_CNT_MODE_UP;
  MCTM_TimeBaseIniture.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
  TM_TimeBaseInit(HT_MCTM0, &MCTM_TimeBaseIniture);
	TM_CRRPreloadCmd(HT_MCTM0, ENABLE);

   /* MCTM Channel 0, 1, 2 3output configuration                                                              */
  MCTM_OutputInitStructure.Channel = TM_CH_0;
  MCTM_OutputInitStructure.OutputMode = TM_OM_PWM1;
  MCTM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
  MCTM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
  MCTM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
  MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 1;	//占空比1
  MCTM_OutputInitStructure.AsymmetricCompare = 0;
  TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);
 
	MCTM_OutputInitStructure.Channel = TM_CH_1;
  MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 0;
	MCTM_OutputInitStructure.Polarity = TM_CHP_INVERTED;
  TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);

	MCTM_OutputInitStructure.Channel = TM_CH_2;
  MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 1 ;
	MCTM_OutputInitStructure.Polarity = TM_CHP_NONINVERTED;
  TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);
	
	MCTM_OutputInitStructure.Channel = TM_CH_3;
  MCTM_OutputInitStructure.Compare = HTCFG_MCTM_RELOAD * 0;
	MCTM_OutputInitStructure.Polarity = TM_CHP_INVERTED;
  TM_OutputInit(HT_MCTM0, &MCTM_OutputInitStructure);	
	
  /* MCTM counter enable                                                                                    */
  TM_Cmd(HT_MCTM0, ENABLE);

  /* MCTM Channel Main Output enable                                                                        */
  MCTM_CHMOECmd(HT_MCTM0, ENABLE);
 }
>>>>>>> 17d76bb033b825a50b312bc88a87a1be51259af9
}
//设置PWM周期
//初始化函数中赋值为HTCFG_MCTM_RELOAD=24000
void PWM_SetFreq(u32 uReload)
{
  TM_SetCounterReload(HT_MCTM0, uReload);
}

//通道0和1为一组
//计数周期HTCFG_MCTM_RELOAD
//输入：PWM范围：0-4800
void SetMotor_L(int PWMN)
{
	TM_SetCaptureCompare(HT_MCTM0, TM_CH_0, 4800);
	TM_SetCaptureCompare(HT_MCTM0, TM_CH_1, PWMN);
	TM_CHCCRPreloadConfig(HT_MCTM0, TM_CH_0, ENABLE);
	TM_CHCCRPreloadConfig(HT_MCTM0, TM_CH_1, ENABLE);
}

//通道2和3为一组
//计数周期HTCFG_MCTM_RELOAD
//输入：PWM范围：0-4800
void SetMotor_R(int PWML)
{
	TM_SetCaptureCompare(HT_MCTM0, TM_CH_2, HTCFG_MCTM_RELOAD-PWML);
	TM_SetCaptureCompare(HT_MCTM0, TM_CH_3, 0);
	TM_CHCCRPreloadConfig(HT_MCTM0, TM_CH_2, ENABLE);
	TM_CHCCRPreloadConfig(HT_MCTM0, TM_CH_3, ENABLE);
}

//停止
void SetMotor_Stop(void)
{
	TM_CHCCRPreloadConfig(HT_MCTM0, TM_CH_0, DISABLE);
	TM_CHCCRPreloadConfig(HT_MCTM0, TM_CH_1, DISABLE);
	TM_CHCCRPreloadConfig(HT_MCTM0, TM_CH_2, DISABLE);
	TM_CHCCRPreloadConfig(HT_MCTM0, TM_CH_3, DISABLE);
}
