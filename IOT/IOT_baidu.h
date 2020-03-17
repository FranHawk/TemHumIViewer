#ifndef _IOT_BAIDU_H
#define _IOT_BAIDU_H

#include "stm32f1xx_hal.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"


#define  PRODUCTKEY           "a1w0XJbXwh0"                                        //��ƷID
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY)                                   //��ƷID����
#define  DEVICENAME			 "SmartLED_01"                                              //�豸��  
#define  DEVICENAME_LEN       strlen(DEVICENAME)                                   //�豸������
#define  DEVICESECRE          "uwMJYOGSoAPNdZBOi8hyDX2qBnHQYgGo"                  				 //�豸��Կ   
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)                                  //�豸��Կ����
 
#define  S_TOPIC_NAME         "/sys/a1w0XJbXwh0/SmartLED_01/thing/service/property/set"   //��Ҫ���ĵ�����  
#define  P_TOPIC_NAME         "/sys/a1w0XJbXwh0/SmartLED_01/thing/event/property/post"    //��Ҫ����������   
 


extern uint16_t buflen;
extern unsigned char buf[800];

uint8_t IOT_baidu_connect(void);
uint8_t IOT_baidu_ping(void);
uint8_t IOT_baidu_connect_publish(char *pTopic,char *pMessage);


#endif

