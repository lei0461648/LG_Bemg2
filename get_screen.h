#ifndef _GET_SCREEN_H
#define _GET_SCREEN_H

#define InitializeParam		0x00
#define HandModeParam		0x01
#define PulseModeParam		0x02
#define Mode_4_20MAParam	0x03
#define BatchPulseParam		0x04
#define CyclicDosingParam	0x05
#define LonginParam		0x06
#define GetPageReturnParam	0x07
#define SetVolumeParam		0x08
#define StartStopCalParam	0x09
#define SetAllParam		0x0A
#define SetONOFFStateParam	0x0B
#define GetCaliInfoParam	0x0C
#define ZeroingParam		0x0D
#define Get04_20MAADParam	0x0E
#define Calib04_20MAParam	0x0F

#define Set04_20MAMLParam	0x0D
#define SetBatchPulseParam	0x0E
#define SetCycleDoParam		0x0F
#define GetOtherSetParam	0x10
#define SetOtherSetParam	0x11
#define GetInfoTest		0x12
#define SetManualFlowParam	0x13
#define FactoryLoginParam	0x14
#define ModifyFactoryPWDParam	0x15 //修改厂家密码
#define ModifyDeviceIDParam	0x16 //修改设备ID
#define SetUserVolumeParam	0x17 //设置用户标定
#define StartStopUserCalParam	0x18 //启停用户标定
#define GetUserCaliInfoParam	0x19 //获取用户标定信息
#define UserZeroingParam	0x1A //用户标定归零
#define GetManualFlowParam	0x1B //用户标定归零
#define ModifyUserPWDParam	0x1C //修改用户密码
#define ModifySysTimeParam	0x1D //修改系统时间
#define ModifyScreenSaverParam	0x1E //修改屏保
#define ModifySysLanguageParam	0x1F //修改系统语言
#define GETSysInfoParam	        0x20 //获取系统基础信息
#define GetAlarmInfoParam	0x21 //获取报警信息
#define ClearTotalFlowParam	0x22 //清除累计流量
#define ReSetUserPassword	0x23 //重置用户密码
#define ModifyLot		0x24 //修改批次
#define ModifyLoop		0x25 //修改循环


void SendDataToBoard(void);
void TxDataHeader(void);
void CreateCRC_TX(unsigned int arraySize);
unsigned long int ConvertToManualFlow(uint8_t receivedData[4]);
unsigned int ConvertToManualFlow16(uint8_t receivedData[2]);
void GetSetupParameters(void);
void ConvertToMinutesAndSeconds(unsigned int totalSeconds, unsigned int *minutes, unsigned int *seconds);
void SetParamCalculate(void);
void GetSlopeIntercept_KB (void);
void ConvertTime(unsigned long int seconds, unsigned int *ResidualMinutes, unsigned int *ResidualSeconds);
unsigned char setBit(unsigned char byte, int pos, int val);
void DelayWriteFlash(void);
void SetOtherParam(void);
 unsigned char hexToDec(unsigned char a);
 void MaxFlowToManualFlow(void);
 void  ValidationData(void);
 void SlowModeMaxValue(void);

#endif
