#ifndef  __TRACK
#define  __TRACK

#include "ht32.h"


#define findBlack			1
#define notFindBlack	0

#define roomsum 		  4//房间总数

extern uint8_t right1_state,right2_state,left1_state,left2_state,middle_state;

void TRACK_Init(void);
void Track_Getbit(void);
void Track(void);













#endif
