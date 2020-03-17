#include "IOT_baidu.h"
#include "utils_hmac.h"
#include "cJSON.h"
uint16_t buflen=800;
unsigned char buf[800];

char ClientID[128];
uint8_t ClientID_len;

char Username[128];
uint8_t Username_len;

char Password[128];
uint8_t Password_len;

uint8_t IOT_baidu_connect()
{
		uint32_t len;
		
		char temp[128];
		
	
		u1_printf("进入连接云服务器函数\r\n");
		MQTTPacket_connectData data = MQTTPacket_connectData_initializer;//配置部分可变头部的值
    
		buflen = sizeof(buf);
		memset(buf,0,buflen);
	
		memset(ClientID,128,0);                                               //客户端ID的缓冲区全部清零
		sprintf(ClientID,"%s|securemode=3,signmethod=hmacsha1|",DEVICENAME);  //构建客户端ID，并存入缓冲区
	
		memset(Username,128,0);                                               //用户名的缓冲区全部清零
		sprintf(Username,"%s&%s",DEVICENAME,PRODUCTKEY);                      //构建用户名，并存入缓冲区
	
		Username_len = strlen(Username);
	
		memset(temp,128,0);                                                                      //临时缓冲区全部清零
		sprintf(temp,"clientId%sdeviceName%sproductKey%s",DEVICENAME,DEVICENAME,PRODUCTKEY);     //构建加密时的明文   
		utils_hmac_sha1(temp,strlen(temp),Password,DEVICESECRE,DEVICESECRE_LEN);                 //以DeviceSecret为秘钥对temp中的明文，进行hmacsha1加密，结果就是密码，并保存到缓冲区中
		Password_len = strlen(Password);                                                         //计算用户名的长度
	
		u1_printf("ClientId:%s\r\n",ClientID);
		u1_printf("Username:%s\r\n",Username);
		u1_printf("Password:%s\r\n",Password);
		
		data.MQTTVersion = 3;
		data.clientID.cstring = ClientID;						//客户端标识，用于区分每个客户端xxx为自定义，后面为固定格式
		data.keepAliveInterval = 120;							//保活计时器，定义了服务器收到客户端消息的最大时间间隔,单位是秒
		data.cleansession = 1;									//该标志置1服务器必须丢弃之前保持的客户端的信息，将该连接视为“不存在”
		data.username.cstring = Username;						//用户名 DeviceName&ProductKey
		data.password.cstring = Password;						//密码，工具生成
		
		len = MQTTSerialize_connect(buf, buflen, &data); 		/*1 构造连接报文*/
		
		transport_sendPacketBuffer(3,buf, len);				//发送连接请求
		
		HAL_Delay(100);
		
		unsigned char sessionPresent, connack_rc;
        do
        {
			if(MQTTPacket_read(buf, buflen, transport_getdata) == CONNACK)
			{
				break;
			}
        }
        while(MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0);
					
		u1_printf("Connect Success!\r\n");
					
		return 0;
}


uint8_t IOT_baidu_connect_publish(char *pTopic,char *pMessage)
{
	static uint8_t err_cnt=0;
    int32_t len;
    MQTTString topicString = MQTTString_initializer;
    int msglen = strlen(pMessage);//计算发布消息的长度
    int buflen = sizeof(buf);

		if (MQTTPacket_read(buf, buflen, transport_getdata) == PUBREC)//接收到发布信息的回复
		{
				unsigned char packettype = 0;
				unsigned char dup_ack = 0;
				unsigned short packetid = 0;

				MQTTDeserialize_ack(&packettype,&dup_ack,&packetid,buf,buflen);
				err_cnt=0;
		}
		else//没接收到回复
		{
				err_cnt++;
				if(err_cnt>3)//连续三次没收到回复  
				{
					err_cnt=0;
					u1_printf("Something Wrong\r\n");
				}
		}
		
    memset(buf,0,buflen);
    topicString.cstring = pTopic;//"设备topic列表中的第一条";
    len = MQTTSerialize_publish(buf, buflen, 0, 2, 0, 0, topicString, (unsigned char*)pMessage, msglen); /*2 构造发布消息的报文*/

	USART3_RX_STA = 0;
	memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
    transport_sendPacketBuffer(3,buf,len);//发送消息
		free(pMessage);
    return 0;
}
uint8_t IOT_baidu_ping(void)
{
		uint32_t len;
		len = MQTTSerialize_pingreq(buf, buflen);
		transport_sendPacketBuffer(3, buf, len);
		u1_printf("Ping...\r\n");
		if (MQTTPacket_read(buf, buflen, transport_getdata) == PINGRESP){
			u1_printf("Pong\r\n");
			return 0;
		}
		else {
			u1_printf("OOPS\r\n");
			return 1;
		}

}

