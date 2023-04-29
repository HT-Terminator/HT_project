#include "Cloud.h"
#include "MQTT.h"
//#include "Timer.h"
#include "usart.h"
#include "MatrixKey.h"
#include "pwm.h"
#include "oled.h"
#include "delay.h"

/*人机交互相关变量*/
//struct Interaction
//{
//	u16 Time;
//	u16 Distance;
//	//State：Send: 0 空闲 || 1 已取货 || 2 请取货 
//	//State：Sell: 0 空闲 || 1 运输中 || 2 已到达 
//	u8 State; 
//	u8 Pickup; // 0 OFF || 1 ON	
//	//Call : Send专属
//	u8 Call;	 // 0 空闲 || 1 有人呼叫
//	u8 Change_Flag;	// 0 无变更 || 1 有变更
//	int room;	//房间号
//}Send,Sell;
struct Interaction Send = {0};
struct Interaction Sell = {0};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      更新所有的信息到云端
//  @param      void
//  @return     
//  @tips		    none
//-------------------------------------------------------------------------------------------------------------------
void Cloud_All_Msg_Push(void)
{
	//此数据很大，可以每隔200ms之类的发送一次
	//这里有一个更好的建议，就是当每次修改Send或者Sell结构体成员时，令其中结构体成员Change_Flag置1，
	if(Send.Change_Flag || Sell.Change_Flag)
	{
		char temp[256];
		
		if(Send.Change_Flag)
		{
			sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"13507075\",\"params\":{\"DIS0\":%d,\"STATE0\":%d,\"ROOM0\":%d},\"version\":\"1.0.0\"}",Send.Distance,Send.State,Send.room); 
			MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
		}
		else if(Sell.Change_Flag)
		{
			sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"13507075\",\"params\":{\"DIS1\":%d,\"STATE1\":%d},\"version\":\"1.0.0\"}",Sell.Distance,Sell.State); 
			MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
		}
		Send.Change_Flag = 0;
		Sell.Change_Flag = 0;
		
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      云端控制命令
//  @param      void
//  @return     
//  @tips		    none
//-------------------------------------------------------------------------------------------------------------------
void Cloud_Ctrl_CMD(void)
{
	char temp[256];	//发送状态缓冲区
	/*APP端控制送货开关箱指令*/
	if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"PICKUP0\":1}"))	//云端发送开箱的命令
	{
//		OLED_FLASH++;
		Send.Pickup = 1;	//开箱
		if(sale_state == 0)
			{
				ServosR_control(OPEN);
			}
		Send.Change_Flag = 1;
		//DEBUG_printf("Send_Open\r\n");
	}
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"PICKUP0\":0}"))//云端发送关箱的命令
	{
//		OLED_FLASH++;
		Send.Call = 0;	//空闲
		Send.Change_Flag = 1;
		//DEBUG_printf("Send_Close\r\n");
	}
	
	/*售货端*/
	/*APP端控制售货货开关箱指令*/
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"PICKUP1\":1}"))	//云端发送开箱的命令
	{
//		OLED_FLASH++;
		Sell.Pickup = 1;	//开箱
//		if(sale_state == 1)
			{
				ServosL_control(OPEN);
			}
		Sell.Change_Flag = 1;
		//DEBUG_printf("Sell_Open\r\n");
	}
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"PICKUP1\":0}"))//云端发送关箱的命令
	{
//		OLED_FLASH++;
		Sell.Call = 0;	//空闲
		Sell.Change_Flag = 1;
		//DEBUG_printf("Sell_Close\r\n");
	}
	/*APP端呼叫小车指令*/
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"SELL_CALL\":0}"))//云端命令
	{
//		OLED_FLASH++;
		Sell.Call = 0;	//空闲
		Sell.Change_Flag = 1;
//		sale_state = 1;
		//DEBUG_printf("Waiting...\r\n");
	}
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"SELL_CALL\":1}"))//云端命令
	{
//		OLED_FLASH++;
		Sell.Call = 1;	//有人呼叫
		Sell.Change_Flag = 1;
		sale_state = 1;
//		State = 1;
		Sell.Distance = RoomNum - 100;
		Sell.State = 1;//运输中
//		Cloud_All_Msg_Push();
		State = 1;
		state_show();
		//DEBUG_printf("Comming...\r\n");
	}
	/*APP端选择售货房间号指令(需更换函数)*/  
	else if (2==sscanf((char*)MQTT_CMDOutPtr, "{\"method\":\"thing.service.property.set\",\"id\":\"%d\",\"params\":{\"ROOM1\":%d},\"version\":\"1.0.0\"}", &Sell.id,&Sell.room))//云端命令

	{
//		if(sale_state==1)
			RoomNum = Sell.room+100;
		  OLED_ShowNum(6*8,Y_1,RoomNum,4,16);//更新显示
//		OLED_FLASH++;
			//DEBUG_printf("Send Room:%d\r\n", Sell.room);
	}
	
	//如有变更，更新状态到云端
	/*更新送货状态*/
//	if(Send.Change_Flag)
//	{
//		Send.Change_Flag = 0; //还原状态
//		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"13507075\",\"params\":{\"PICK_UP0\":%d},\"version\":\"1.0.0\"}",Send.Pickup); 
//		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
//	}
//	/*更新收货状态*/
//	else if(Sell.Change_Flag)
//	{
//		Sell.Change_Flag = 0; //还原状态
//		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"13507075\",\"params\":{\"PICK_UP1\":%d,\"SELL_CALL\":%d},\"version\":\"1.0.0\"}", Sell.Pickup, Sell.Call); 
//		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
//	}
	
	MQTT_CMDOutPtr += BUFF_UNIT;                             	 //指针下移
	if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //如果指针到缓冲区尾部了
		MQTT_CMDOutPtr = MQTT_CMDBuf[0]; 
}



u8 Beef_State;
u8 OLED_FLASH;	//OLED刷新状态
u8 tempdata,humidata;	//温度||湿度
float IC_temp;	//系统内部温度
//-------------------------------------------------------------------------------------------------------------------
//  @brief      云端交互
//  @param      void
//  @return     0 成功 | 1 失败
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
u8 Handle_buff_about_cloud(void)
{

	if(Connect_flag==1)//连接成功，发布数据和接收推送
	{     
//		char temp[256];	//发送状态缓冲区
		/*------------------------处理发送缓冲区数据--------------------------*/
		if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)
		{    //说明发送缓冲区有数据了
			if(SubcribePack_flag==1)
			{    
				//////DEBUG_printf("\r\n--------TEST---------\r\n");
				//////DEBUG_printf("发送数据到云:%s\r\n",&MQTT_TxDataOutPtr[2]);  //串口提示信息
				//Attention 可能有BUG
//					//////DEBUG_printf("%s\r\n", MQTT_TxDataOutPtr);
//				for(int i =0;i<2;i++)
//				{
					Cloud_printf("%s\r\n", &MQTT_TxDataOutPtr[2]); 				//发送数据
//					delay_ms(100);
				
				MQTT_TxDataOutPtr += BUFF_UNIT;                       //指针下移
				if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
					MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //指针归位到缓冲区开头
			} 				
		}//处理发送缓冲区数据的else if分支结尾
		
		/*---------------------- 处理接收缓冲区数据------------------------*/
		if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr)
		{  //说明接收缓冲区有数据了														
//				//////DEBUG_printf("接收到数据:");
			//if判断，如果第一个字节是0x7B，表示收到的是服务器发来的推送数据
			//我们要提取控制命令
			if(MQTT_RxDataOutPtr[2] == 0x7B)
			{ 
				//////DEBUG_printf("服务器等级0推送\r\n"); 		   //串口输出信息 
				MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据
			}
							
			MQTT_RxDataOutPtr += BUFF_UNIT;                     //指针下移
			if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)            //如果指针到缓冲区尾部了
				MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //指针归位到缓冲区开头                        
		}
		/*-------------------------------------------------------------*/
		/*                     处理命令缓冲区数据                      */
		/*-------------------------------------------------------------*/
		if(MQTT_CMDOutPtr != MQTT_CMDInPtr)
		{//说明命令缓冲区有数据了	
			//////DEBUG_printf("\r\n--------TEST---------\r\n");
			//////DEBUG_printf("接收云端的命令:%s\r\n",MQTT_CMDOutPtr);                 //串口输出信息
			//云端控制小车的指令处理
			Cloud_Ctrl_CMD();
		
		}//处理命令缓冲区数据的else if分支结尾
		return 0;
	}
	else	//连接失败
	{
		return 1;	
	}
}

void cloud_init(void)
{
	char temp[256];
				Send.room = 0;
				Send.Distance = 0;
				Send.Time = Send.Distance*5;
				Sell.room = 0;
				Sell.Distance = 0;
				Sell.Time = 5;
				Send.State = 0;
				Sell.State = 0;
//				Sell.Change_Flag = 1;
//				Send.Change_Flag = 1;
				sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"13507075\",\"params\":{\"TIME0\":%d,\"DIS0\":%d,\"TIME1\":%d,\"DIS1\":%d,\"STATE0\":%d,\"STATE1\":%d,\"ROOM0\":%d},\"version\":\"1.0.0\"}",Send.Time,Send.Distance,Sell.Time,Sell.Distance,Send.State,Sell.State,Send.room); 
				MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器
//				Cloud_All_Msg_Push();
}

