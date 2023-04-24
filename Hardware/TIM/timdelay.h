#ifndef	__TIMDELAY
#define __TIMDELAY
#include "ht32f5xxxx_bftm.h"

#define HTCFG_TM_IPN			   SCTM0

#define HTCFG_TM_PORT 			 STRCAT2(HT_,HTCFG_TM_IPN)	//ƴ�Ӻ���

#define HTCFG_TM_IRQn        STRCAT2(HTCFG_TM_IPN,  _IRQn)
#define HTCFG_TM_IRQHandler  STRCAT2(HTCFG_TM_IPN,  _IRQHandler)

#define TM_FREQ_HZ					(2000)	//0.5ms

void TM_Configuration(void);
void HTCFG_TM_IRQHandler(void);

extern unsigned int sctm_10ms;
extern unsigned int sctm_100ms;
extern unsigned int sctm_state3;
extern unsigned int sctm_state4;


#endif
