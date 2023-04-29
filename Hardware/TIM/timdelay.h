#ifndef	__TIMDELAY
#define __TIMDELAY
#include "ht32f5xxxx_bftm.h"

#define HTCFG_TM_IPN			   SCTM0

#define HTCFG_TM_PORT 			 STRCAT2(HT_,HTCFG_TM_IPN)	//Æ´½Óº¯Êý

#define HTCFG_TM_IRQn        STRCAT2(HTCFG_TM_IPN,  _IRQn)
#define HTCFG_TM_IRQHandler  STRCAT2(HTCFG_TM_IPN,  _IRQHandler)

#define TM_FREQ_HZ					(2000)	//0.5ms

void TM_Configuration(void);
void HTCFG_TM_IRQHandler(void);

extern unsigned int sctm_control_30ms;
extern unsigned int sctm_encoder_30ms;
extern unsigned int sctm_usart0_100ms;
extern unsigned int sctm_servos1_num;
extern unsigned int sctm_servos2_num;
extern unsigned int sctm_weight_500ms;
extern unsigned int sctm_pickup_2500ms;
extern unsigned int sctm_mqtt_num;
extern unsigned int sctm_cloud_500ms;
extern unsigned int sctm_nopickup_10000ms;
extern unsigned int sctm_left_500ms;



#endif
