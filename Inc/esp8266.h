#ifndef _ESP_H
#define _ESP_H

#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"

#define SSID "Zhang Zeng Tao"
#define PASS "password0606"

#define IPBUF "192.168.0.7"
#define PORTNUM "8086"

#define IOT_DOMAIN_NAME "a1w0XJbXwh0.iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define IOT_PORTNUM 	"1883"

uint8_t esp8266_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime);
uint8_t* esp8266_check_cmd(uint8_t *str);
uint8_t esp8266_Connect_IOTServer(void);
uint8_t esp8266_Connect_AP(void);
uint8_t esp8266_Connect_Server(void);
uint8_t esp8266_quit_trans(void);

#endif
