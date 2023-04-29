//#include "stm32f10x.h"    //������Ҫ��ͷ�ļ�
#include "mqtt.h"         //������Ҫ��ͷ�ļ�
#include "string.h"       //������Ҫ��ͷ�ļ�
#include "stdio.h"        //������Ҫ��ͷ�ļ�
#include "usart.h"       //������Ҫ��ͷ�ļ�
//#include "utils_hmac.h"   //������Ҫ��ͷ�ļ�

#include "ht32.h"

unsigned char  MQTT_RxDataBuf[R_NUM][BUFF_UNIT];            //���ݵĽ��ջ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
unsigned char *MQTT_RxDataInPtr;                            //ָ����ջ�����������ݵ�λ��
unsigned char *MQTT_RxDataOutPtr;                           //ָ����ջ�������ȡ���ݵ�λ��
unsigned char *MQTT_RxDataEndPtr;                           //ָ����ջ�����������λ��

unsigned char  MQTT_TxDataBuf[T_NUM][BUFF_UNIT];            //���ݵķ��ͻ�����,���з��������������ݣ�����ڸû�����,��������һ���ֽڴ�����ݳ���
unsigned char *MQTT_TxDataInPtr;                            //ָ���ͻ�����������ݵ�λ��
unsigned char *MQTT_TxDataOutPtr;                           //ָ���ͻ�������ȡ���ݵ�λ��
unsigned char *MQTT_TxDataEndPtr;                           //ָ���ͻ�����������λ��

unsigned char  MQTT_CMDBuf[C_NUM][BUFF_UNIT];               //�������ݵĽ��ջ�����
unsigned char *MQTT_CMDInPtr;                               //ָ���������������ݵ�λ��
unsigned char *MQTT_CMDOutPtr;                              //ָ�����������ȡ���ݵ�λ��
unsigned char *MQTT_CMDEndPtr;                              //ָ���������������λ��

char ClientID[128];                                          //��ſͻ���ID�Ļ�����
int  ClientID_len;                                           //��ſͻ���ID�ĳ���

char Username[128];                                          //����û����Ļ�����
int  Username_len;											 //����û����ĳ���

char Passward[128];                                          //�������Ļ�����
int  Passward_len;											 //�������ĳ���

char ServerIP[128];                                          //��ŷ�����IP��������
int  ServerPort;                                             //��ŷ������Ķ˿ں�

int   Fixed_len;                       					     //�̶���ͷ����
int   Variable_len;                     					 //�ɱ䱨ͷ����
int   Payload_len;                       					 //��Ч���ɳ���
unsigned char  temp_buff[BUFF_UNIT];						 //��ʱ������������������

char Ping_flag;           //ping����״̬      0������״̬���ȴ���ʱʱ�䵽������Ping����
                          //ping����״̬      1��Ping�����ѷ��ͣ����յ� �������ظ����ĵĺ� ��1��Ϊ0
char Connect_flag;        //ͬ����������״̬  0����û�����ӷ�����  1�������Ϸ�������
char ConnectPack_flag;    //CONNECT����״̬   1��CONNECT���ĳɹ�
char SubcribePack_flag;   //���ı���״̬      1�����ı��ĳɹ�


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ������,����,�������ݵ� ������ �Լ���״̬����
//  @param      void
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void MQTT_Buff_Init(void)
{	
	MQTT_RxDataInPtr=MQTT_RxDataBuf[0];               //ָ���ͻ�����������ݵ�ָ���λ
	MQTT_RxDataOutPtr=MQTT_RxDataInPtr;               //ָ���ͻ�������ȡ���ݵ�ָ���λ
    MQTT_RxDataEndPtr=MQTT_RxDataBuf[R_NUM-1];        //ָ���ͻ�����������ָ���λ
	
	MQTT_TxDataInPtr=MQTT_TxDataBuf[0];               //ָ���ͻ�����������ݵ�ָ���λ
	MQTT_TxDataOutPtr=MQTT_TxDataInPtr;               //ָ���ͻ�������ȡ���ݵ�ָ���λ
	MQTT_TxDataEndPtr=MQTT_TxDataBuf[T_NUM-1];        //ָ���ͻ�����������ָ���λ
	
	MQTT_CMDInPtr=MQTT_CMDBuf[0];                     //ָ���������������ݵ�ָ���λ
	MQTT_CMDOutPtr=MQTT_CMDInPtr;                     //ָ�����������ȡ���ݵ�ָ���λ
	MQTT_CMDEndPtr=MQTT_CMDBuf[C_NUM-1];              //ָ���������������ָ���λ
	
	Connect_flag = 1;                                      //��������
	SubcribePack_flag = 1;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����Ƴ�ʼ���������õ��ͻ���ID���û���������
//  @param      void
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
//void AliIoT_Parameter_Init(void)
//{	
//	char temp[128];                                                       //������ܵ�ʱ����ʱʹ�õĻ�����

//	memset(ClientID,128,0);                                               //�ͻ���ID�Ļ�����ȫ������
//	sprintf(ClientID,"%s|securemode=3,signmethod=hmacsha1|",DEVICENAME);  //�����ͻ���ID�������뻺����
//	ClientID_len = strlen(ClientID);                                      //����ͻ���ID�ĳ���
//	
//	memset(Username,128,0);                                               //�û����Ļ�����ȫ������
//	sprintf(Username,"%s&%s",DEVICENAME,PRODUCTKEY);                      //�����û����������뻺����
//	Username_len = strlen(Username);                                      //�����û����ĳ���
//	
//	memset(temp,128,0);                                                                      //��ʱ������ȫ������
//	sprintf(temp,"clientId%sdeviceName%sproductKey%s",DEVICENAME,DEVICENAME,PRODUCTKEY);     //��������ʱ������   
//	utils_hmac_sha1(temp,strlen(temp),Passward,DEVICESECRE,DEVICESECRE_LEN);                 //��DeviceSecretΪ��Կ��temp�е����ģ�����hmacsha1���ܣ�����������룬�����浽��������
//	Passward_len = strlen(Passward);                                                         //�����û����ĳ���
//	
//	memset(ServerIP,128,0);  
//	sprintf(ServerIP,"%s.iot-as-mqtt.cn-shanghai.aliyuncs.com",PRODUCTKEY);                  //��������������
//	ServerPort = 1883;                                                                       //�������˿ں�1883
//	
//	//DEBUG_printf("�� �� ����%s:%d\r\n",ServerIP,ServerPort); //�������������Ϣ
//	//DEBUG_printf("�ͻ���ID��%s\r\n",ClientID);               //�������������Ϣ
//	//DEBUG_printf("�� �� ����%s\r\n",Username);               //�������������Ϣ
//	//DEBUG_printf("��    �룺%s\r\n",Passward);               //�������������Ϣ
//}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PING���ģ�������
//  @param      void
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
//void MQTT_PingREQ(void)
//{
//	temp_buff[0]=0xC0;              //��1���ֽ� ���̶�0xC0                      
//	temp_buff[1]=0x00;              //��2���ֽ� ���̶�0x00 

//	TxDataBuf_Deal(temp_buff, 2);   //�������ݵ�������
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������Ϣ����
//  @param      topic_name||topic���ƣ�data||���ݣ�data_len||���ݳ���
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void MQTT_PublishQs0(char *topic, char *data, int data_len)
{	
	//DEBUG_printf("�����ƶ����ݵĳ���Ϊ��%d\n", data_len);
	memcpy(temp_buff, data, data_len);
	TxDataBuf_Deal(temp_buff, data_len);  //���뷢�����ݻ�����

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������������ĵȼ�0������
//  @param      unsigned char *redata ||���յ�������
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void MQTT_DealPushdata_Qs0(unsigned char *redata)
{
	int cmd_len;   //����һ����������������а������������ݵĳ���
	unsigned char tempbuff[BUFF_UNIT];       //��ʱ������
	unsigned char* data;
	cmd_len = redata[0] * 256 + redata[1];
	data = &redata[2];

	memcpy(tempbuff, data, cmd_len);                   //�������ݿ�������

	CMDBuf_Deal(tempbuff, cmd_len);                             //�������������
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����ͻ�����
//  @param      unsigned char *data, int size
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void TxDataBuf_Deal(unsigned char *data, int size)
{
	memcpy(&MQTT_TxDataInPtr[2],data,size);      //�������ݵ����ͻ�����	
	MQTT_TxDataInPtr[0] = size/256;              //��¼���ݳ���
	MQTT_TxDataInPtr[1] = size%256;              //��¼���ݳ���
	
	MQTT_TxDataInPtr+=BUFF_UNIT;                 //ָ������
	if(MQTT_TxDataInPtr==MQTT_TxDataEndPtr)      //���ָ�뵽������β����
		MQTT_TxDataInPtr = MQTT_TxDataBuf[0];    //ָ���λ����������ͷ
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����������
//  @param      unsigned char *data, int size
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void CMDBuf_Deal(unsigned char *data, int size)
{
	memcpy(&MQTT_CMDInPtr[0],data,size);      //�������ݵ��������
//	MQTT_CMDInPtr[0] = size/256;              //��¼���ݳ���
//	MQTT_CMDInPtr[1] = size%256;              //��¼���ݳ���
	MQTT_CMDInPtr[size] = '\0';             //�����ַ���������
	MQTT_CMDInPtr+=BUFF_UNIT;                 //ָ������
	if(MQTT_CMDInPtr==MQTT_CMDEndPtr)         //���ָ�뵽������β����
		MQTT_CMDInPtr = MQTT_CMDBuf[0];       //ָ���λ����������ͷ
}
