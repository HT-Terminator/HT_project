#ifndef _CLOUD_H
#define _CLOUD_H
//#include "sys.h" 
#include "ht32f5xxxx_ckcu.h"
#include "ht32f5xxxx_gpio.h"
#define ON 1
#define OFF 0
extern u8 Beef_State;
extern u8 tempdata,humidata;
extern u8 OLED_FLASH;
extern float IC_temp;
u8 Handle_buff_about_cloud(void);
void Cloud_Ctrl_CMD(void);
void Cloud_All_Msg_Push(void);
void cloud_init(void);

struct Interaction
{
	u16 Time;
	u16 Distance;
	//State：Send: 0 空闲 || 1 已取货 || 2 请取货 
	//State：Sell: 0 空闲 || 1 运输中 || 2 已到达 
	u8 State; 
	u8 Pickup; // 0 OFF || 1 ON	
	//Call : Send专属
	u8 Call;	 // 0 空闲 || 1 有人呼叫
	u8 Change_Flag;	// 0 无变更 || 1 有变更
	int room;	//房间号
	int id;
};

extern struct Interaction Send;
extern struct Interaction Sell;

#endif 
