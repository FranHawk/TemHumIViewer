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
		
	
		u1_printf("���������Ʒ���������\r\n");
		MQTTPacket_connectData data = MQTTPacket_connectData_initializer;//���ò��ֿɱ�ͷ����ֵ
    
		buflen = sizeof(buf);
		memset(buf,0,buflen);
	
		memset(ClientID,128,0);                                               //�ͻ���ID�Ļ�����ȫ������
		sprintf(ClientID,"%s|securemode=3,signmethod=hmacsha1|",DEVICENAME);  //�����ͻ���ID�������뻺����
	
		memset(Username,128,0);                                               //�û����Ļ�����ȫ������
		sprintf(Username,"%s&%s",DEVICENAME,PRODUCTKEY);                      //�����û����������뻺����
	
		Username_len = strlen(Username);
	
		memset(temp,128,0);                                                                      //��ʱ������ȫ������
		sprintf(temp,"clientId%sdeviceName%sproductKey%s",DEVICENAME,DEVICENAME,PRODUCTKEY);     //��������ʱ������   
		utils_hmac_sha1(temp,strlen(temp),Password,DEVICESECRE,DEVICESECRE_LEN);                 //��DeviceSecretΪ��Կ��temp�е����ģ�����hmacsha1���ܣ�����������룬�����浽��������
		Password_len = strlen(Password);                                                         //�����û����ĳ���
	
		u1_printf("ClientId:%s\r\n",ClientID);
		u1_printf("Username:%s\r\n",Username);
		u1_printf("Password:%s\r\n",Password);
		
		data.MQTTVersion = 3;
		data.clientID.cstring = ClientID;						//�ͻ��˱�ʶ����������ÿ���ͻ���xxxΪ�Զ��壬����Ϊ�̶���ʽ
		data.keepAliveInterval = 120;							//�����ʱ���������˷������յ��ͻ�����Ϣ�����ʱ����,��λ����
		data.cleansession = 1;									//�ñ�־��1���������붪��֮ǰ���ֵĿͻ��˵���Ϣ������������Ϊ�������ڡ�
		data.username.cstring = Username;						//�û��� DeviceName&ProductKey
		data.password.cstring = Password;						//���룬��������
		
		len = MQTTSerialize_connect(buf, buflen, &data); 		/*1 �������ӱ���*/
		
		transport_sendPacketBuffer(3,buf, len);				//������������
		
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
    int msglen = strlen(pMessage);//���㷢����Ϣ�ĳ���
    int buflen = sizeof(buf);

		if (MQTTPacket_read(buf, buflen, transport_getdata) == PUBREC)//���յ�������Ϣ�Ļظ�
		{
				unsigned char packettype = 0;
				unsigned char dup_ack = 0;
				unsigned short packetid = 0;

				MQTTDeserialize_ack(&packettype,&dup_ack,&packetid,buf,buflen);
				err_cnt=0;
		}
		else//û���յ��ظ�
		{
				err_cnt++;
				if(err_cnt>3)//��������û�յ��ظ�  
				{
					err_cnt=0;
					u1_printf("Something Wrong\r\n");
				}
		}
		
    memset(buf,0,buflen);
    topicString.cstring = pTopic;//"�豸topic�б��еĵ�һ��";
    len = MQTTSerialize_publish(buf, buflen, 0, 2, 0, 0, topicString, (unsigned char*)pMessage, msglen); /*2 ���췢����Ϣ�ı���*/

	USART3_RX_STA = 0;
	memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
    transport_sendPacketBuffer(3,buf,len);//������Ϣ
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

