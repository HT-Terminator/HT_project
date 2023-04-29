//#include "stm32f10x.h"    //包含需要的头文件
#include "mqtt.h"         //包含需要的头文件
#include "string.h"       //包含需要的头文件
#include "stdio.h"        //包含需要的头文件
#include "usart.h"       //包含需要的头文件
//#include "utils_hmac.h"   //包含需要的头文件

#include "ht32.h"

unsigned char  MQTT_RxDataBuf[R_NUM][BUFF_UNIT];            //数据的接收缓冲区,所有服务器发来的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
unsigned char *MQTT_RxDataInPtr;                            //指向接收缓冲区存放数据的位置
unsigned char *MQTT_RxDataOutPtr;                           //指向接收缓冲区读取数据的位置
unsigned char *MQTT_RxDataEndPtr;                           //指向接收缓冲区结束的位置

unsigned char  MQTT_TxDataBuf[T_NUM][BUFF_UNIT];            //数据的发送缓冲区,所有发往服务器的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
unsigned char *MQTT_TxDataInPtr;                            //指向发送缓冲区存放数据的位置
unsigned char *MQTT_TxDataOutPtr;                           //指向发送缓冲区读取数据的位置
unsigned char *MQTT_TxDataEndPtr;                           //指向发送缓冲区结束的位置

unsigned char  MQTT_CMDBuf[C_NUM][BUFF_UNIT];               //命令数据的接收缓冲区
unsigned char *MQTT_CMDInPtr;                               //指向命令缓冲区存放数据的位置
unsigned char *MQTT_CMDOutPtr;                              //指向命令缓冲区读取数据的位置
unsigned char *MQTT_CMDEndPtr;                              //指向命令缓冲区结束的位置

char ClientID[128];                                          //存放客户端ID的缓冲区
int  ClientID_len;                                           //存放客户端ID的长度

char Username[128];                                          //存放用户名的缓冲区
int  Username_len;											 //存放用户名的长度

char Passward[128];                                          //存放密码的缓冲区
int  Passward_len;											 //存放密码的长度

char ServerIP[128];                                          //存放服务器IP或是域名
int  ServerPort;                                             //存放服务器的端口号

int   Fixed_len;                       					     //固定报头长度
int   Variable_len;                     					 //可变报头长度
int   Payload_len;                       					 //有效负荷长度
unsigned char  temp_buff[BUFF_UNIT];						 //临时缓冲区，构建报文用

char Ping_flag;           //ping报文状态      0：正常状态，等待计时时间到，发送Ping报文
                          //ping报文状态      1：Ping报文已发送，当收到 服务器回复报文的后 将1置为0
char Connect_flag;        //同服务器连接状态  0：还没有连接服务器  1：连接上服务器了
char ConnectPack_flag;    //CONNECT报文状态   1：CONNECT报文成功
char SubcribePack_flag;   //订阅报文状态      1：订阅报文成功


//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化接收,发送,命令数据的 缓冲区 以及各状态参数
//  @param      void
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void MQTT_Buff_Init(void)
{	
	MQTT_RxDataInPtr=MQTT_RxDataBuf[0];               //指向发送缓冲区存放数据的指针归位
	MQTT_RxDataOutPtr=MQTT_RxDataInPtr;               //指向发送缓冲区读取数据的指针归位
    MQTT_RxDataEndPtr=MQTT_RxDataBuf[R_NUM-1];        //指向发送缓冲区结束的指针归位
	
	MQTT_TxDataInPtr=MQTT_TxDataBuf[0];               //指向发送缓冲区存放数据的指针归位
	MQTT_TxDataOutPtr=MQTT_TxDataInPtr;               //指向发送缓冲区读取数据的指针归位
	MQTT_TxDataEndPtr=MQTT_TxDataBuf[T_NUM-1];        //指向发送缓冲区结束的指针归位
	
	MQTT_CMDInPtr=MQTT_CMDBuf[0];                     //指向命令缓冲区存放数据的指针归位
	MQTT_CMDOutPtr=MQTT_CMDInPtr;                     //指向命令缓冲区读取数据的指针归位
	MQTT_CMDEndPtr=MQTT_CMDBuf[C_NUM-1];              //指向命令缓冲区结束的指针归位
	
	Connect_flag = 1;                                      //建立连接
	SubcribePack_flag = 1;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      阿里云初始化参数，得到客户端ID，用户名和密码
//  @param      void
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
//void AliIoT_Parameter_Init(void)
//{	
//	char temp[128];                                                       //计算加密的时候，临时使用的缓冲区

//	memset(ClientID,128,0);                                               //客户端ID的缓冲区全部清零
//	sprintf(ClientID,"%s|securemode=3,signmethod=hmacsha1|",DEVICENAME);  //构建客户端ID，并存入缓冲区
//	ClientID_len = strlen(ClientID);                                      //计算客户端ID的长度
//	
//	memset(Username,128,0);                                               //用户名的缓冲区全部清零
//	sprintf(Username,"%s&%s",DEVICENAME,PRODUCTKEY);                      //构建用户名，并存入缓冲区
//	Username_len = strlen(Username);                                      //计算用户名的长度
//	
//	memset(temp,128,0);                                                                      //临时缓冲区全部清零
//	sprintf(temp,"clientId%sdeviceName%sproductKey%s",DEVICENAME,DEVICENAME,PRODUCTKEY);     //构建加密时的明文   
//	utils_hmac_sha1(temp,strlen(temp),Passward,DEVICESECRE,DEVICESECRE_LEN);                 //以DeviceSecret为秘钥对temp中的明文，进行hmacsha1加密，结果就是密码，并保存到缓冲区中
//	Passward_len = strlen(Passward);                                                         //计算用户名的长度
//	
//	memset(ServerIP,128,0);  
//	sprintf(ServerIP,"%s.iot-as-mqtt.cn-shanghai.aliyuncs.com",PRODUCTKEY);                  //构建服务器域名
//	ServerPort = 1883;                                                                       //服务器端口号1883
//	
//	//DEBUG_printf("服 务 器：%s:%d\r\n",ServerIP,ServerPort); //串口输出调试信息
//	//DEBUG_printf("客户端ID：%s\r\n",ClientID);               //串口输出调试信息
//	//DEBUG_printf("用 户 名：%s\r\n",Username);               //串口输出调试信息
//	//DEBUG_printf("密    码：%s\r\n",Passward);               //串口输出调试信息
//}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PING报文，心跳包
//  @param      void
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
//void MQTT_PingREQ(void)
//{
//	temp_buff[0]=0xC0;              //第1个字节 ：固定0xC0                      
//	temp_buff[1]=0x00;              //第2个字节 ：固定0x00 

//	TxDataBuf_Deal(temp_buff, 2);   //加入数据到缓冲区
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      发布消息报文
//  @param      topic_name||topic名称，data||数据，data_len||数据长度
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void MQTT_PublishQs0(char *topic, char *data, int data_len)
{	
	//DEBUG_printf("推送云端数据的长度为：%d\n", data_len);
	memcpy(temp_buff, data, data_len);
	TxDataBuf_Deal(temp_buff, data_len);  //加入发送数据缓冲区

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      处理服务器发来的等级0的推送
//  @param      unsigned char *redata ||接收到的数据
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void MQTT_DealPushdata_Qs0(unsigned char *redata)
{
	int cmd_len;   //定义一个变量，存放推送中包含的命令数据的长度
	unsigned char tempbuff[BUFF_UNIT];       //临时缓冲区
	unsigned char* data;
	cmd_len = redata[0] * 256 + redata[1];
	data = &redata[2];

	memcpy(tempbuff, data, cmd_len);                   //命令数据拷贝出来

	CMDBuf_Deal(tempbuff, cmd_len);                             //加入命令到缓冲区
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      处理发送缓冲区
//  @param      unsigned char *data, int size
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void TxDataBuf_Deal(unsigned char *data, int size)
{
	memcpy(&MQTT_TxDataInPtr[2],data,size);      //拷贝数据到发送缓冲区	
	MQTT_TxDataInPtr[0] = size/256;              //记录数据长度
	MQTT_TxDataInPtr[1] = size%256;              //记录数据长度
	
	MQTT_TxDataInPtr+=BUFF_UNIT;                 //指针下移
	if(MQTT_TxDataInPtr==MQTT_TxDataEndPtr)      //如果指针到缓冲区尾部了
		MQTT_TxDataInPtr = MQTT_TxDataBuf[0];    //指针归位到缓冲区开头
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      处理命令缓冲区
//  @param      unsigned char *data, int size
//  @return     none
//  @tips		none
//-------------------------------------------------------------------------------------------------------------------
void CMDBuf_Deal(unsigned char *data, int size)
{
	memcpy(&MQTT_CMDInPtr[0],data,size);      //拷贝数据到命令缓冲区
//	MQTT_CMDInPtr[0] = size/256;              //记录数据长度
//	MQTT_CMDInPtr[1] = size%256;              //记录数据长度
	MQTT_CMDInPtr[size] = '\0';             //加入字符串结束符
	MQTT_CMDInPtr+=BUFF_UNIT;                 //指针下移
	if(MQTT_CMDInPtr==MQTT_CMDEndPtr)         //如果指针到缓冲区尾部了
		MQTT_CMDInPtr = MQTT_CMDBuf[0];       //指针归位到缓冲区开头
}
