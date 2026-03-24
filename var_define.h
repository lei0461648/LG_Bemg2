#ifndef _VAR_DEFINE_H
#define _VAR_DEFINE_H

/************************Base Info*********************/
#define ON_OFF			P7_bit.no1  //开关机
#define PUMP_Direction   	P7_bit.no2  //给驱动板光电信号
#define INPT_1   		P5_bit.no0  //获取光电信号
#define RS485_Direction   	P12_bit.no0    /*1:send 2:receive  485驱动*/ 
#define RS485_R   	        0
#define RS485_T   	        1
//状态	
#define TRUE	               	   0xAA
#define FALSE	                   0x55
#define WAIT	                   0xCC


/*********************CLOCK 时钟芯片参数******************/
#define WRITE_SECOND     0x80
#define WRITE_MINUTE     0x82
#define WRITE_HOUR       0x84
#define WRITE_DAY        0x86
#define WRITE_MONTH      0x88
#define WRITE_YEAR       0x8C
#define READ_SECOND      0x81
#define READ_MINUTE      0x83	
#define READ_HOUR        0x85
#define READ_DAY         0x87
#define READ_MONTH       0x89	
#define READ_YEAR        0x8D
#define WRITE_PROTECT    0x8e
#define DIO		 P6_bit.no1
#define CE		 P6_bit.no2

/*********************初始化参数 ******************/
#define CALI_STTROKE_COUNT	200
//正常速度
#define STTROKE_COUNT_HOUR	11250
#define MAX_PMW_US	   	32000     
//慢速模式50%
#define STTROKE_COUNT_HOUR_50	5625
#define MAX_PMW_US_50	   	16000
//慢速模式25%
#define STTROKE_COUNT_HOUR_25	2812
#define MAX_PMW_US_25	   	8000 

#define RATE_FLOW_200	   	2000//额定功率  200.0L
#define USERCal_ZS	   	2000

#define MANUAL_MODE 		0x01
#define PULSE_MODE  	 	0x02
#define MA_0_20_MODE  	 	0x03
#define BATCH_MODE      	0x04
#define CYCLIC_MODE      	0x05

#define MAXFLOWML_H2    	25000000 //250L/H
#define RUNINGCAL_PERIOD      	20    //10*20ms=200ms
#define OPEN			1
#define CLOSE			0
#define TOTALFLOW_S      	30 //累计流量多少秒增加一次

/**************************Alarm State*************************/
#define LOW_Voltage		0x10	//qianya 
#define OVER_Voltage		0x20	//guoya
#define LOW_Flow		0x30	//di yewei
#define OVER_Flow		0x40	//gao yewei
#define ALARM_Normal		0x50	//zhengchang
#define GUOQI_Alarm		0x60	//GUOqi 
#define PUMP_ERROR		0x70
#define PUMP_NORMAL		0x71
#define MA4_20_ERROR		0x80	
#define MA4_20_NORMAL		0x81	
#define GD_ERROR		0x90 	
#define GD_NORMAL		0x91 	
#define DIA_ERROR		0xA0 	 //隔膜报警

/*********************REMOTE 远程开关*************************/
#define REMOTE_SWITCH_PIN       P6_bit.no3
#define HI       		0x01
#define LO       		0x00
#define REMOTE_NON_STOP  	0x00  //远程不停机
#define REMOTE_STOP  		0x01  //远程停机


/*********************AD*************************/
#define ADC_4_20MA_PERIOD      10    //10*20ms=200ms
#define ADC_4_20MA_YL          50   //100*20ms=1s
#define OUTPUT_MA_PERIOD       100    //100*20ms=2s

#define	Data_IN_OUT		P1_bit.no5		//ê?3?D?o???
#define	SCLK			P1_bit.no6
#define	nSYNC			P1_bit.no7

/********************FLASH***************************/
#define R_WP			P2_bit.no1
#define	SCL			P2_bit.no2
#define	SDA			P2_bit.no3
#define SUC_VEL_PERCENT    	6000 //60%

/********************Time***************************/
#define SLK              P6_bit.no0	        	      
#define DIO		 P6_bit.no1
#define CE		 P6_bit.no2

/********************继电器输出***************************/
#define SYS_PWR_OUTPUT   P7_bit.no0   //开关机继电器输出
#define RELAY_ALARM_CTRL P3_bit.no0  //异常报警继电器输出
#define ON_H		1
#define OFF_L		0

/********************空桶开关量***************************/
#define KT_PIN			P2_bit.no7   //空桶开关量
#define KT_NON_ALARM  		0x00  //空桶不报警
#define KT_ALARM  		0x01  //空桶报警

/********************掉电记忆***************************/
#define PWOER_SAVE   		P7_bit.no3

// 激活功能五种模式对应的位掩码（每个位独立，避免冲突）
#define MODE_SLOW        0x01  // 慢速模式（bit0）
#define MODE_BUS   	 0x02  // 总线模式（bit1）
#define MODE_FLOW_CTRL   0x04  // 流量控制（bit2）
#define MODE_AUTO_VENT   0x08  // 自动排气（bit3）
#define MODE_SYS_LOCK    0x10  // 系统锁 （bit4）

/********************产品基础信息***************************/
#define PRODUCT_MODEL 	        "DBG"   //产品型号
#define PRODUCT_SERIAL_NUMBER   "SN12345678" //产品序列号
#define SOFTWARE_NUMBER 	"V1.30301" //软件  1.3控制板 03程序屏幕程序01驱动板程序


#endif