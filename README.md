# TemHumIViewer
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200317192820477.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_16,color_FFFFFF,t_70)

# 引言
出生于互联网时代的我们，见证了智能手机和个人电脑通过计算机网络互相进行连接，把每个人紧密地连接在一起，形成了一个地球村。而今，我们即将迈入5G时代，在通信技术不断发展的今天，连接到互联网的将不仅是昂贵精密的手机电脑，身边的各种物体也可以通过物联网连接到云端，由物联网平台管理上云的物联网设备，使用者通过浏览器，小程序等接收物发出的通知并且完成想过操作。物联网不仅将物与物连接在一起，也将物与人紧密地连接在一起，让生活更加智慧便捷。这，就是即将到来的AIOT时代。
本次实验主要通过自制一个家庭物联网温湿度检测模块，学习具体的基于MQTT协议的物联网设计制作。在应用的过程中，也加深了对计算机网络各层次之间的理解。
硬件设备原代码附于文末。

# 需求分析
 - 硬件设备可以读取温湿度信息
 - 硬件设备可连接互联网上传消息
 - 云端物联网平台24小时不间断工作，完成接入设备的管理，消息存储与转发
 - web服务器接收物联网平台的信息，经过处理使用户通过网页访问的方式查询信息

# 系统设计
### 系统总体数据流
- 硬件设备stm32作为终端结点，通过MQTT协议将数据上传至物联网平台，同时接受物联网平台下发的信息。
- 云服务器作为与用户直接联系的平台，既作为http服务端，使用http协议向用户展示温湿度等信息接收用户发送的命令信息，又作为mqtt客户端与物联网平台交换信息。
- 物联网平台作为消息的中转站，负责物联网设备的管理与消息的转发。
- 总体计算机网络应用层协议数据流如下图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200317091316852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_1,color_FFFFFF,t_70)
### 硬件设备端
#### 各模块设计
 - 为实现开发的便捷性和稳定性，使用意法半导体的STM32F103ZET6作为主控MCU，接收各传感器的消息。负责与服务器建立连接，并将消息构造成MQTT报文，上传至云端物联网平台。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316225134702.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_1,color_FFFFFF,t_70)
 -  使用DHT11温湿度传感器，可同时检测温湿度，测量范围为0-100，精度为1度，采用单总线与主控芯片完成数据传递，单总线协议不是本文主要内容，概不介绍
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316225951636.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_1,color_FFFFFF,t_70)
 - 使用ESP8266模块完成硬件设备的物联网功能，ESP8266在数据链路层使用WIFI协议，适用于家庭室内这种不需要经常移动的场合。ESP8266已完整封装TCP/IP协议，只需要通过AT指令连接WIFI网络，然后进入透传模式，主控MCU发送过来的MQTT消息包就会经ESP8266发送至互联网，完成设备之间的解耦。ESP8266模块通过串口与主控MCU完成通讯。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316230956379.png)

#### 硬件设备数据流

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200316231550547.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_1,color_FFFFFF,t_70)

### 云端物联网服务平台和Web服务器
 - 云端服务全部使用阿里云平台的服务，阿里云的服务具有稳定性强，配置便捷的特点，方便部署物联网应用，云端总体应用架构如下。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/2020031709364994.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_1,color_FFFFFF,t_70)
 - 云端物联网平台使用**阿里云物联网平台**。阿里云物联网平台为设备提供安全可靠的连接通信能力，向下连接海量设备，支撑设备数据采集上云；向上提供云端API，服务端通过调用云端API将指令下发至设备端，实现远程控制。提供设备接入，设备管理，安全能力和规则引擎。其中规则引擎用来完成数据转发，负责沟通数据在设备间，设备与数据库间，设备与服务器间流转。
 
 - Web服务器使用**阿里云服务器ECS**，云服务器（Elastic Compute Service，简称ECS）是阿里云提供的性能卓越、稳定可靠、弹性扩展的IaaS（Infrastructure as a Service）级别云计算服务。云服务器ECS免去了您采购IT硬件的前期准备，让您像使用水、电、天然气等公共资源一样便捷、高效地使用服务器，实现计算资源的即开即用和弹性伸缩。阿里云ECS持续提供创新型服务器，解决多种业务需求，助力业务发展。
 
 # MQTT协议
 ### 简介
 MQTT（Message Queuing Telemetry Transport，消息队列遥测传输协议），是一种基于发布/订阅（publish/subscribe）模式的"轻量级"通讯协议，该协议构建于TCP/IP协议上，由IBM在1999年发布。MQTT最大优点在于，可以以极少的代码和有限的带宽，为连接远程设备提供实时可靠的消息服务。作为一种低开销、低带宽占用的即时通讯协议，使其在物联网、小型设备、移动应用等方面有较广泛的应用。

MQTT是一个基于客户端-服务器的消息发布/订阅传输协议。MQTT协议是轻量、简单、开放和易于实现的，这些特点使它适用范围非常广泛。在很多情况下，包括受限的环境中，如：机器与机器（M2M）通信和物联网（IoT）。其在，通过卫星链路通信传感器、偶尔拨号的医疗设备、智能家居、及一些小型化设备中已广泛使用。

### 主要特性

 - 工作在低带宽、不可靠的网络的协议，数据精简，节约流量，不添加冗余功能
 - 使用TCP/IP提供网络连接
 - 使用订阅/发布消息模式，可以完成一对多的消息传递，消息并不是通过端到端传递，而是经过代理端转发，比如硬件设备端并不直接将数据发送到服务器，而是硬件设备将数据发送到指定话题，然后服务器订阅该话题，完成数据的传递。使用订阅/发布消息模式，可完成设备之间的解耦，同一个话题可以有多个发布者和多个订阅者。
 - 有三种消息发布服务质量（Qos）用于不同的通讯环境中，分为Qos0，Qos1，和Qos2，分别代表消息至少到达一次，消息重复一次，消息至少重复两次。灵活可变的消息服务质量可以用于不同的场合。
 - 使用遗嘱机制，通知有关各方客户端异常中断的机制，当设备断开连接时，通过遗嘱信息通知各方，以便于后续处理。

### 协议原理
实现MQTT协议需要客户端和服务器端通讯完成，在通讯过程中，MQTT协议中有三种身份：发布者（Publish）、代理（Broker）（服务器）、订阅者（Subscribe）。其中，消息的发布者和订阅者都是客户端，消息代理是服务器，消息发布者可以同时是订阅者。

MQTT传输的消息分为：主题（Topic）和负载（payload）两部分：

 - Topic，可以理解为消息的类型，订阅者订阅（Subscribe）后，就会收到该主题的消息内容（payload）
 -  payload，可以理解为消息的内容，是指订阅者具体要使用的内容。


在这次实验中，硬件设备和阿里云服务器作为客户端订阅和发布相关话题，阿里云物联网平台作为消息代理服务器
#### 客户端
一个使用MQTT协议的应用程序或者设备，它总是建立到服务器的网络连接。客户端可以：

（1）发布其他客户端可能会订阅的信息；
（2）订阅其它客户端发布的消息；
（3）退订或删除应用程序的消息；
（4）断开与服务器连接。


#### MQTT服务器
MQTT服务器以称为"消息代理"（Broker），可以是一个应用程序或一台设备。它是位于消息发布者和订阅者之间，它可以：

（1）接受来自客户的网络连接；
（2）接受客户发布的应用信息；
（3）处理来自客户端的订阅和退订请求；
（4）向订阅的客户转发应用程序消息。

### 主要方法
 - Connect。等待与服务器建立连接。
 - Disconnect。等待MQTT客户端完成所做的工作，并与服务器断开TCP/IP会话。
 - Subscribe。等待完成订阅。
 - UnSubscribe。等待服务器取消客户端的一个或多个topics订阅。
 - Publish。MQTT客户端发送消息请求，发送完成后返回应用程序线程。

### 报文结构

MQTT报文由三部分组成
 - **固定报头**
 - **可变报头**
 - **有效载荷**

#### 固定报头
每个MQTT报文都包含一个固定报头，下图描述了固定报头的格式
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200317172229863.png)
其中MQTT控制报文的类型决定了这个报文是Connect，还是Disconnect，还是Subscribe，7到4位不同数值代表的数字如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200317173204950.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_1,color_FFFFFF,t_70)

 - 用于指定控制类型的标志位只有PUBLISH有用，用来标志这个消息的Qos，
 - 剩余长度用于显示该报文端剩余字节的长度

#### 可变报头
它在固定报头和负载之间。可变报头的内容根据报文类型的不同而不同。可变报头的报文标识符（Packet Identifier）字段存在于在多个类型的报文里。

#### 有效载荷
有效载荷搭载报文段的有效内容，比如在PUBLISH中，有效载荷就是硬件设备要发送到物联网平台的内容。在硬件设备作为客户端连接到服务端使用Connect时，有效载荷就是用户名和密码等内容。

# 实验内容
### 硬件设备端
硬件设备端需要完成温湿度数据的读取，wifi的连接，MQTT服务端的连接和数据的上报。因为任务比较简单，不使用操作系统，直接使用裸机开发。MQTT报文需要用JSON格式收发，所以发送数据之前需要自行封装成JSON格式。之前分析过MQTT协议，这里为了使用方便，我们使用开源的Paho MQTT协议栈，可完成多个平台间的切换，该代码不仅可使用阿里云物联网平台，也适用于其他物联网平台。详细步骤为：

1.硬件初始化
2.连接WIFI，TCP/IP协议建立与阿里云服务器建立连接
3.使用MQTT协议发送CONNECT报文连接阿里云物联网平台
4.大循环中读取温湿度传感器的数据，并且封装成JSON格式使用MQTT协议的PUBLISH向指定话题发布消息，将消息发送到阿里云物联网服务器

JSON格式数据如下
```javascript
{
	"id":"12345",
	"method":"thing.event.property.post",
	"params":{
		"LEDSwitch":当前LED灯的状态,
		"CurrentHumidity":当前湿度,
		"CurrentTemperature",当前温度
	}
}
```

大循环主要代码如下

```c
while (1)
  {
    /* USER CODE END WHILE */
	
    /* USER CODE BEGIN 3 */
	  HAL_Delay(1000);
	  if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin))
	  {
		KEY_STATUS = 1;
	  }
	  else{
		KEY_STATUS = 0;
	  }
	  DHT11_Read_Data(&temperature,&humidity);
	  
	  cJSON * pJsonRoot = NULL;
	  pJsonRoot = cJSON_CreateObject();//
	  cJSON_AddStringToObject(pJsonRoot, "id", "12345");//
	  cJSON_AddStringToObject(pJsonRoot, "method", "thing.event.property.post");//
	  cJSON * pJsonChild = cJSON_CreateObject();//
	  cJSON_AddNumberToObject(pJsonChild, "LEDSwitch", KEY_STATUS);
		cJSON_AddNumberToObject(pJsonChild, "CurrentHumidity",humidity);
		cJSON_AddNumberToObject(pJsonChild, "CurrentTemperature",temperature);
	  cJSON_AddItemToObject(pJsonRoot, "params", pJsonChild);//
	  
	  char * lpJsonStr = cJSON_Print(pJsonRoot);
		//cJSON_Delete(pJsonChild);
	  cJSON_Delete(pJsonRoot);//
	  u1_printf("time:%s\r\n",lpJsonStr);
	  IOT_baidu_connect_publish(P_TOPIC_NAME,lpJsonStr);
		free(lpJsonStr);
		
  }		
```

连接实物图

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200317181222570.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_16,color_FFFFFF,t_70)

### 阿里云物联网平台端
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200317184702106.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_16,color_FFFFFF,t_70)
连接物联网平台需要先创建设备，根据这个设备的证书信息，和密码。设备可通过一个三元组证书信息连接到云服务器。
阿里云物联网服务器作为设备的接收和中转站，管理所有通过MQTT协议发送和接收的信息。
将Web服务器和硬件设备定义成两个设备，通过设置规则引擎，完成数据的接收和转发。

下图为规则引擎设计
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200317185742480.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_1,color_FFFFFF,t_70)

### 阿里云Web服务器端
#### 技术架构
- 原生Linux系统
- 使用node.js框架作为网站后端
- 使用node.js软件包中的node-red数据流处理工具

#### 具体步骤

使用node-red，node-red是一种图形化的后端处理工具，适用于物联网服务等小型服务端应用搭建，不需要写代码，部署比较快捷。
使用node-red主要完成以下几个功能
 
 - 作为MQTT协议中的客户端，连接阿里云物联网平台，订阅相关话题，接收上传的JSON格式MQTT报文
 - 对JSON格式的报文进行处理，获得上传的温湿度信息
 - 将接收到的温湿度信息以表盘和曲线图的形式显示出来

下图为在node-red上完成的配置，没有写一点代码，点击部署后完成部署，便可以在网页上看到
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200317191840160.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_16,color_FFFFFF,t_70)


打开硬件设备，待其连接到阿里云物联网平台，然后进入以下网址

[http://47.105.207.251:1880/ui](http://47.105.207.251:1880/ui)

截至这篇文章提交，我使用的阿里云服务器免费七天试用期已到，应该不可访问。。。

就可以看到实时的温湿度信息。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200317192820477.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjQ4NzkwNg==,size_16,color_FFFFFF,t_70)

湿度比较大的两个尖峰是我用嘴吹了两口气。人工增加了湿度。

**至此完成了基于MQTT应用层协议的物联网家庭温湿度监测系统的设计与制作。**


# 总结与展望
这次基于MQTT应用层协议的物联网家庭温湿度监测系统的实验过程中，我查询了很多资料，解决了不少bug。主要锻炼了学习新知识并且运用新知识的能力，在此过程中运用到了嵌入式，服务器前后端等知识。通过物联网实验，对计算机网络各个层次的理解和应用更加深刻透彻，充分理解了TCP/IP协议在实际计算机网络的应用中起到了什么样的作用。深入学习了MQTT协议，初步掌握了物联网产品的架构和开发流程，为以后制作物联网相关应用打下了基础。
本实验初步打通了物联网的整个数据链路，通过该实验，下一步可以完成更加复杂智能的物联网应用，比如实时操控家里的家电，家中电器通过检测环境变化自动开关机。添加微信小程序访问Web服务器，从而监控物联网设备等功能。
总之，在5G时代即将到来之际，物联网及其应用将迎来新的发展机遇，值得我们好好研究。


**硬件设备端stm32源码**
[https://github.com/FranHawk/TemHumIViewer.git](https://github.com/FranHawk/TemHumIViewer.git)