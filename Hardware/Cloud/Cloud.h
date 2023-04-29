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

struct Interaction
{
	u16 Time;
	u16 Distance;
	//State��Send: 0 ���� || 1 ��ȡ�� || 2 ��ȡ�� 
	//State��Sell: 0 ���� || 1 ������ || 2 �ѵ��� 
	u8 State; 
	u8 Pickup; // 0 OFF || 1 ON	
	//Call : Sendר��
	u8 Call;	 // 0 ���� || 1 ���˺���
	u8 Change_Flag;	// 0 �ޱ�� || 1 �б��
	int room;	//�����
	int id;
};

extern struct Interaction Send;
extern struct Interaction Sell;

#endif 
