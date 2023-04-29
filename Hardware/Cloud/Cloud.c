#include "Cloud.h"
#include "MQTT.h"
//#include "Timer.h"
#include "usart.h"
#include "MatrixKey.h"
#include "pwm.h"
#include "oled.h"

/*�˻�������ر���*/
//struct Interaction
//{
//	u16 Time;
//	u16 Distance;
//	//State��Send: 0 ���� || 1 ��ȡ�� || 2 ��ȡ�� 
//	//State��Sell: 0 ���� || 1 ������ || 2 �ѵ��� 
//	u8 State; 
//	u8 Pickup; // 0 OFF || 1 ON	
//	//Call : Sendר��
//	u8 Call;	 // 0 ���� || 1 ���˺���
//	u8 Change_Flag;	// 0 �ޱ�� || 1 �б��
//	int room;	//�����
//}Send,Sell;
struct Interaction Send;
struct Interaction Sell;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������е���Ϣ���ƶ�
//  @param      void
//  @return     
//  @tips		    none
//-------------------------------------------------------------------------------------------------------------------
void Cloud_All_Msg_Push(void)
{
	//�����ݺܴ󣬿���ÿ��200ms֮��ķ���һ��
	//������һ�����õĽ��飬���ǵ�ÿ���޸�Send����Sell�ṹ���Աʱ�������нṹ���ԱChange_Flag��1��
	if(Send.Change_Flag || Sell.Change_Flag)
	{
		char temp[256];
		Send.Change_Flag = 0;
		Sell.Change_Flag = 0;
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"13507075\",\"params\":{\"TIME0\":%d,\"DIS0\":%d,\"TIME1\":%d,\"DIS1\":%d,\"STATE0\":%d,\"STATE1\":%d,\"ROOM0\":%d},\"version\":\"1.0.0\"}",Send.Time,Send.Distance,Sell.Time,Sell.Distance,Send.State,Sell.State,Send.room); 
		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ƶ˿�������
//  @param      void
//  @return     
//  @tips		    none
//-------------------------------------------------------------------------------------------------------------------
void Cloud_Ctrl_CMD(void)
{
	char temp[256];	//����״̬������
	/*APP�˿����ͻ�������ָ��*/
	if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"PICKUP0\":1}"))	//�ƶ˷��Ϳ��������
	{
//		OLED_FLASH++;
		Send.Pickup = 1;	//����
		if(sale_state == 0)
			{
				ServosR_control(OPEN);
			}
		Send.Change_Flag = 1;
		//DEBUG_printf("Send_Open\r\n");
	}
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"PICKUP0\":0}"))//�ƶ˷��͹��������
	{
//		OLED_FLASH++;
		Send.Call = 0;	//����
		Send.Change_Flag = 1;
		//DEBUG_printf("Send_Close\r\n");
	}
	
	/*�ۻ���*/
	/*APP�˿����ۻ���������ָ��*/
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"PICKUP1\":1}"))	//�ƶ˷��Ϳ��������
	{
//		OLED_FLASH++;
		Sell.Pickup = 1;	//����
//		if(sale_state == 1)
			{
				ServosL_control(OPEN);
			}
		Sell.Change_Flag = 1;
		//DEBUG_printf("Sell_Open\r\n");
	}
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"PICKUP1\":0}"))//�ƶ˷��͹��������
	{
//		OLED_FLASH++;
		Sell.Call = 0;	//����
		Sell.Change_Flag = 1;
		//DEBUG_printf("Sell_Close\r\n");
	}
	/*APP�˺���С��ָ��*/
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"SELL_CALL\":0}"))//�ƶ�����
	{
//		OLED_FLASH++;
		Sell.Call = 0;	//����
		Sell.Change_Flag = 1;
//		sale_state = 1;
		//DEBUG_printf("Waiting...\r\n");
	}
	else if (strstr((char*)MQTT_CMDOutPtr, "\"params\":{\"SELL_CALL\":1}"))//�ƶ�����
	{
//		OLED_FLASH++;
		Sell.Call = 1;	//���˺���
		Sell.Change_Flag = 1;
		sale_state = 1;
		State = 1;
		Sell.State = 1;//������
		Cloud_All_Msg_Push();
		state_show();
		//DEBUG_printf("Comming...\r\n");
	}
	/*APP��ѡ���ۻ������ָ��(���������)*/  
	else if (2==sscanf((char*)MQTT_CMDOutPtr, "{\"method\":\"thing.service.property.set\",\"id\":\"%d\",\"params\":{\"ROOM1\":%d},\"version\":\"1.0.0\"}", &Sell.id,&Sell.room))//�ƶ�����

	{
//		if(sale_state==1)
			RoomNum = Sell.room+100;
		  OLED_ShowNum(6*8,Y_1,RoomNum,4,16);//������ʾ
//		OLED_FLASH++;
			//DEBUG_printf("Send Room:%d\r\n", Sell.room);
	}
	
	//���б��������״̬���ƶ�
	/*�����ͻ�״̬*/
	if(Send.Change_Flag)
	{
		Send.Change_Flag = 0; //��ԭ״̬
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"13507075\",\"params\":{\"PICK_UP0\":%d},\"version\":\"1.0.0\"}",Send.Pickup); 
		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
	}
	/*�����ջ�״̬*/
	else if(Sell.Change_Flag)
	{
		Sell.Change_Flag = 0; //��ԭ״̬
		sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"13507075\",\"params\":{\"PICK_UP1\":%d,\"SELL_CALL\":%d},\"version\":\"1.0.0\"}", Sell.Pickup, Sell.Call); 
		MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������
	}
	
	MQTT_CMDOutPtr += BUFF_UNIT;                             	 //ָ������
	if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //���ָ�뵽������β����
		MQTT_CMDOutPtr = MQTT_CMDBuf[0]; 
}



u8 Beef_State;
u8 OLED_FLASH;	//OLEDˢ��״̬
u8 tempdata,humidata;	//�¶�||ʪ��
float IC_temp;	//ϵͳ�ڲ��¶�
//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ƶ˽���
//  @param      void
//  @return     0 �ɹ� | 1 ʧ��
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
u8 Handle_buff_about_cloud(void)
{

	if(Connect_flag==1)//���ӳɹ����������ݺͽ�������
	{     
//		char temp[256];	//����״̬������
		/*------------------------�����ͻ���������--------------------------*/
		if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)
		{    //˵�����ͻ�������������
			if(SubcribePack_flag==1)
			{    
				//////DEBUG_printf("\r\n--------TEST---------\r\n");
				//////DEBUG_printf("�������ݵ���:%s\r\n",&MQTT_TxDataOutPtr[2]);  //������ʾ��Ϣ
				//Attention ������BUG
//					//////DEBUG_printf("%s\r\n", MQTT_TxDataOutPtr);
				Cloud_printf("%s\r\n", &MQTT_TxDataOutPtr[2]);                       //��������
				MQTT_TxDataOutPtr += BUFF_UNIT;                       //ָ������
				if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //���ָ�뵽������β����
					MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
			} 				
		}//�����ͻ��������ݵ�else if��֧��β
		
		/*---------------------- ������ջ���������------------------------*/
		if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr)
		{  //˵�����ջ�������������														
//				//////DEBUG_printf("���յ�����:");
			//if�жϣ������һ���ֽ���0x7B����ʾ�յ����Ƿ�������������������
			//����Ҫ��ȡ��������
			if(MQTT_RxDataOutPtr[2] == 0x7B)
			{ 
				//////DEBUG_printf("�������ȼ�0����\r\n"); 		   //���������Ϣ 
				MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //����ȼ�0��������
			}
							
			MQTT_RxDataOutPtr += BUFF_UNIT;                     //ָ������
			if(MQTT_RxDataOutPtr == MQTT_RxDataEndPtr)            //���ָ�뵽������β����
				MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //ָ���λ����������ͷ                        
		}
		/*-------------------------------------------------------------*/
		/*                     ���������������                      */
		/*-------------------------------------------------------------*/
		if(MQTT_CMDOutPtr != MQTT_CMDInPtr)
		{//˵�����������������	
			//////DEBUG_printf("\r\n--------TEST---------\r\n");
			//////DEBUG_printf("�����ƶ˵�����:%s\r\n",MQTT_CMDOutPtr);                 //���������Ϣ
			//�ƶ˿���С����ָ���
			Cloud_Ctrl_CMD();
		
		}//��������������ݵ�else if��֧��β
		return 0;
	}
	else	//����ʧ��
	{
		return 1;	
	}
}

