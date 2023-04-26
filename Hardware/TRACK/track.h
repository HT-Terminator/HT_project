#ifndef  __TRACK
#define  __TRACK

#include "ht32.h"


#define findBlack			1
#define notFindBlack	0

#define RoomSum 		  4//房间总数

extern uint8_t right1_state,right2_state,left1_state,left2_state,middle_state;

extern uint8_t nowroom;
extern uint8_t stop_state_room;
extern uint8_t across_flag;

void TRACK_Init(void);
void Track_Getbit(void);
void Track(void);
void Stop_Begin_room(void);
void Stop_Begin_StartingPoint(void);











#endif
