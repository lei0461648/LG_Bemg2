#include "r_cg_macrodriver.h"
#include "r_cg_wdt.h"
#include "get_screen.h"
#include "get_adc.h"
#include "var_define.h"
#include "UART_SET.h"
#include "r_cg_serial.h"
#include "m_manual_1.h"
#include "m_pulse_2.h"
#include "m_04_20ma_3.h"
#include "m_lot_4.h"
#include "m_loop_5.h"
#include "b_cali.h"
#include "get_adc.h"
#include "b_basic.h"
#include "set_other.h"
#include "r_cg_userdefine.h"
#include "string.h"

/*****************Comm***********************/
extern  uint8_t UI_URT2_REC_D[];
unsigned char UC_URART2_TxDataArray_A[100];
extern unsigned char UC_C_FunctionCode_D;
extern unsigned int crc_r;
extern unsigned char crc_H;	
extern unsigned char crc_L;
extern unsigned char *puch;

extern unsigned char UC_C_WriteFlash_S;

/************Biao Ding volume*************/
unsigned long int UI_CalibrateVolML_H1_DF;  //厂家标定容积
unsigned long int UI_UserCalibrateVolML_H1_DF;  //用户标定容积
unsigned char UC_IsCalibrated_DF;

/************Mode_SET_PARAM*************/
extern unsigned char UC_ModeRun_SF;
unsigned long int LI_ManualFlow_L_DF;
unsigned long int  LI_PulseFlow_ML_DF;
unsigned char  UC_PulseMemory_SF;   //(没用 存储跟换 批次手动)
unsigned char  UC_BatchManual_SF;    //批次手动
unsigned char  UC_04_20MAType_SF;
unsigned int  UC_04_20MA_I1_DF;
unsigned int  UC_04_20MA_Q1_DF;
unsigned int  UC_04_20MA_I2_DF;
unsigned int  UC_04_20MA_Q2_DF;
unsigned long int LI_LotVolume_DF;
unsigned char UC_LotVolumeUnit_SF;
unsigned long int UC_AddTimeSec_DF;
unsigned long int LI_LoopLotVolume_DF;
unsigned char UC_LoopLotVolumeUnit_SF;
unsigned long int UC_LoopADDTime_DF;
unsigned long int UC_LoopCycleTime_DF;
unsigned long int UC_LoopStartDelayTime_DF;

unsigned int UI_C_SetMinAdValue4_DF;
unsigned int UI_C_SetMaxAdValue20_DF;
extern unsigned int UI_4_20MA_AD_K_DF;
extern unsigned int UI_4_20MA_AD_B_DF;
unsigned char UC_SignalFault_D;//单独报警状态
unsigned char UC_ActiveMode_S;//激活状态


unsigned long int LI_SetMaxFlow_DF;
unsigned int UI_RatedMAXFlow_DF; //额定最大流量
unsigned int UI_FactoryRatedMAXFlow_DF; //厂家额定最大流量
unsigned int UI_UserRatedMAXFlow_DF; //厂家额定最大流量
unsigned int UI_FactoryRatedMAXFlowSlowMode_DF; //厂家额定最大流量-慢速模式


/************Tool PARAM*************/
unsigned char UC_ReceivedData_4A[4];
unsigned char UC_ReceivedData_2A[2];
unsigned int minutes, seconds;
unsigned long int LI_ResidualFlow_ViewD; //剩余批量容积界面显示
unsigned long int LI_LoopResidualFlow_ViewD;


/************extern PARAM*************/
extern unsigned char UC_C_OnOff_S;
extern unsigned char UC_C_OnOffStatus_S;
extern unsigned int UI_Percent_H2_DF;
extern unsigned int UI_04_20MA_D;
extern unsigned long int LI_TargetFlowML_H2_DF;
extern unsigned long int LI_ResidualFlow_D; //剩余批量容积
extern unsigned char UC_SwitchCali_S;
extern unsigned int UI_ZeroCali_C;
extern unsigned int UI_4_20MA_AD_D;
extern unsigned int UI_C_MA_H1_D;
extern unsigned long int LI_TimeINFlowML_H2_D;
extern unsigned long int LI_LoopResidualFlow_D;
extern unsigned long int LI_LoopResidualCycleTime_D;
unsigned int ResidualMinutes, ResidualSeconds;
extern unsigned char UC_RemoteStatusHL_S;
extern unsigned long int UI_MaxFlowML_H2_DF;
extern unsigned char UC_SetPageg_S;
extern unsigned int TDR07_Percent_C; //当前脉宽
extern unsigned int UI_PulseWidth_D;//设置脉宽
extern unsigned long int LI_LotVolumeTemp_DF; //批次容积的临时变量
extern unsigned char UC_LotVolumeUnitTemp_SF; //批次容积的临时变量单位
extern unsigned long int UC_AddTimeSecTemp_DF; //批次容积时间
extern unsigned char UC_StartZeroing_S;//电机归零状态
extern unsigned char UI_BaudRateStatusIN_S;//进行485读写
extern unsigned int  UI_CurPressure_Level;//当前压力液位
extern unsigned int KS_PWMPULSE; //进入快吸慢推的脉宽

/************Temp PARAM*************/
unsigned long int LI_ManualFlow_L_RE_Temp;
unsigned long int LI_ManualFlow_0x12_Temp;
unsigned long int LI_PulseFlow_L_RE_Temp;
unsigned int UI_Percent_H1_RE_Temp;
unsigned char UC_C_OnOff_Temp;
unsigned long int LI_TargetFlowML_H2_RE_Temp;
unsigned int UI_ZeroCali_Temp;
unsigned long int UI_RatedFlow_Temp; //额定功率临时变量
unsigned long int UI_MaxFlowML_H2_Temp; //额定功率临时变量
unsigned long int UI_RatedPercent_D; //额定功率和最大功率的百分比
unsigned long int LI_RatedMAXPulseWidth_DF; //额定脉宽根据额定流量计算得出
extern unsigned char UC_ModeRunTemp_SF;//用于切换模式的临时变量

unsigned int UI_BaudRateStatusTemp_S;

unsigned char UC_DelayWriteFlash_S;
unsigned char UC_DelayWriteFlash_C;


//验证批次数据和 验证循环投加数据参数

unsigned long int LI_LotVolumeVerify_D;
unsigned char UC_LotVolumeUnitVerify_S;
unsigned long int UC_AddTimeSecVerify_D;
unsigned long int UC_AddTimeSecVerifyMax_D;
unsigned long int UC_AddTimeSecVerifyMin_D;

unsigned char UC_DataException_S;//批次数据状态
unsigned long int LI_ResidualFlowVerify_D; //剩余批量容积

unsigned long int tempTime;


/************Test PARAM*************/

/************Other PARAM*************/
unsigned int UI_BaudRateStatus_SF;
unsigned int UI_CalibParam_MA_DF;
unsigned int UI_SlowModeStatus_SF;
unsigned int UI_SlowModeStatus_SFTemp;
unsigned int UI_IsPessureSwitch_SF; //压力 开关量 0x55 关 0xAA开 
unsigned int UI_IsLevelSwitch_SF; //空桶开关量 开关量 0x55 关 0xAA开 
unsigned int UI_PessureRange_DF;
unsigned int UI_PessureHigh_DF;
unsigned int UI_PessureLow_DF;
unsigned int UI_LevelRange_DF;
unsigned int UI_LevelHigh_DF;
unsigned int UI_LevelLow_DF;
unsigned char UI_Year_DF;
unsigned char UI_Month_DF;
unsigned char UI_Day_DF;
unsigned char UI_Hour_DF;
unsigned char UI_Minute_DF;
unsigned char UI_RemoteControl_SF;
unsigned char UI_RunRelayOutput_SF;
unsigned char UI_FaultyRelayOutput_SF;
unsigned char UI_ScreenSaver_SF;
unsigned int UI_ScreenSaverTime_DF;
unsigned char UI_MachineCode_DF;
unsigned char UI_LanguageSet_SF;
unsigned int UI_LockScreenTime_DF;
unsigned char UC_PowerOffLock_SF; // 0=关闭断电保护，1=开启断电保护 (需存入EEPROM)
unsigned char UC_PowerOffLocked_S; //// 0=正常，1=触发断电锁定报警 (内存临时变量)


/**********************标定 厂家******************************/
unsigned  long int UC_SetZhuanSu_D;//设置标定转速
unsigned long int UI_MAXFlow_D; //最大流量
extern unsigned char UC_PWDUser_A[8];//用户密码
extern unsigned char UC_PWDFactory_A[8];//工厂密码
extern unsigned char UC_DeviceId_A[8];//设备ID

/**********************时钟时间******************************/
unsigned char UC_C_Time_Y;
unsigned char UC_C_Time_MM;
unsigned char UC_C_Time_D;
unsigned char UC_C_Time_M;
unsigned char UC_C_Time_H;

/**********************当前时间******************************/
extern unsigned char Time_Second;
extern unsigned char Time_Minute;
extern unsigned char Time_Hour;
extern unsigned char Time_Day;
extern unsigned char Time_Month;
extern unsigned char Time_Year;

/**********************批次参数******************************/
extern unsigned int UI_CountSeconds_D;
extern unsigned char UC_IsPulseLaunch_S;
extern unsigned char UC_PulseTrigger_S;

/**********************循环投加******************************/
extern unsigned char UC_IsDelayedLaunch_S;
extern unsigned int UC_IsDelayedLaunch_C;
extern unsigned int UC_LaunchTime_C;
extern unsigned long int LI_LoopLotVolumeTemp_DF; //循环投机容积的临时临时变量
extern unsigned char UC_LoopLotVolumeUnitTemp_SF; //循环投机容积临时临时变量单位
extern unsigned long int UC_LoopADDTimeTemp_DF; //循环投机添加时间临时变量
extern unsigned long int UC_LoopCycleTimeTemp_DF;//循环投机周期时间临时变量
extern unsigned long int UC_LoopStartDelayTimeTemp_DF;//循环投机开始延时时间临时变量

/**********************累计流量******************************/
extern unsigned int UI_TotalFlowL_H1_DF;//累计流量L小数点后一位
extern unsigned int UI_FlowInM3_DF;//累计M3
/**********************报警消息******************************/
unsigned char offset,i; 
extern AlarmMsg alarm_buffer[MAX_ALARMS];

/**********************产品的基本信息******************************/
unsigned char product_model_array[8];//产品型号
unsigned char product_number_array[10];//产品序列号
unsigned char software_number_array[8];//软件号

/************************************************
* Function Name: ADGetEC  
* Description  : Get electric current
* Arguments    : None
* Return Value : None
*************************************************/
void SendDataToBoard(void)
{
	UC_C_FunctionCode_D=UI_URT2_REC_D[3];
	switch(UC_C_FunctionCode_D)
	{
		case InitializeParam://0x00  初始化
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x0B;//leagth
			UC_URART2_TxDataArray_A[4]=UC_ModeRun_SF;//mode*********************************************
			//UC_URART2_TxDataArray_A[5]=UC_C_OnOffStatus_S;//ON/OFF
			UC_URART2_TxDataArray_A[5]=UC_C_OnOff_S;//ON/OFF
			UC_URART2_TxDataArray_A[6]=(unsigned char)((UI_UserRatedMAXFlow_DF>>8)&0xFF); //额定最大流量
			UC_URART2_TxDataArray_A[7]=(unsigned char)(UI_UserRatedMAXFlow_DF&0xFF);
			UC_URART2_TxDataArray_A[8]=UI_ScreenSaver_SF;//屏保开关
			UC_URART2_TxDataArray_A[9]=(unsigned char)((UI_ScreenSaverTime_DF>>8)&0xFF); //屏保时间
			UC_URART2_TxDataArray_A[10]=(unsigned char)(UI_ScreenSaverTime_DF&0xFF);
			UC_URART2_TxDataArray_A[11]=UI_LanguageSet_SF; //系统语言
			CreateCRC_TX(12);
			break;
		case HandModeParam://0x01  手动模式
			UC_SetPageg_S=0;//退出设置界面
			
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x12;//leagth
			
			if(LI_ManualFlow_L_DF>=10000) //>LI_ManualFlow_L_DF 0.0025L (MIN) 2.50ML 10000=1L
			{
				LI_ManualFlow_L_RE_Temp=LI_ManualFlow_L_DF/100; //L--1.0000L = 1.00L
				UC_URART2_TxDataArray_A[17]=0x01;//1 l/h 2 ml/h
			}
			else
			{
				LI_ManualFlow_L_RE_Temp=LI_ManualFlow_L_DF*10;  //LI_ManualFlow_L_DF 0.0025 L Min = 2.50ml 00.00 ml
				UC_URART2_TxDataArray_A[17]=0x02;//1 l/h 2 ml/h
			}
			
			UC_URART2_TxDataArray_A[4]=(unsigned char)((LI_ManualFlow_L_RE_Temp>>24)&0xFF);//Target traffic
			UC_URART2_TxDataArray_A[5]=(unsigned char)((LI_ManualFlow_L_RE_Temp>>16)&0xFF);
			UC_URART2_TxDataArray_A[6]=(unsigned char)((LI_ManualFlow_L_RE_Temp>>8)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)(LI_ManualFlow_L_RE_Temp&0xFF);

			if(UC_C_OnOffStatus_S==TRUE)
			{
				UC_URART2_TxDataArray_A[8]=(unsigned char)((LI_ManualFlow_L_RE_Temp>>24)&0xFF);////Real-time traffic -----------****Change****--
				UC_URART2_TxDataArray_A[9]=(unsigned char)((LI_ManualFlow_L_RE_Temp>>16)&0xFF);
				UC_URART2_TxDataArray_A[10]=(unsigned char)((LI_ManualFlow_L_RE_Temp>>8)&0xFF);
				UC_URART2_TxDataArray_A[11]=(unsigned char)(LI_ManualFlow_L_RE_Temp&0xFF);
				
				UI_Percent_H1_RE_Temp=UI_Percent_H2_DF/10;
				UC_URART2_TxDataArray_A[12]=(unsigned char)((UI_Percent_H1_RE_Temp>>8)&0xFF);//percent
				UC_URART2_TxDataArray_A[13]=(unsigned char)(UI_Percent_H1_RE_Temp&0xFF);
				
			}
			else
			{
				UC_URART2_TxDataArray_A[8]=0x00;//Target traffic
				UC_URART2_TxDataArray_A[9]=0x00;
				UC_URART2_TxDataArray_A[10]=0x00;
				UC_URART2_TxDataArray_A[11]=0x00;
				
				UI_Percent_H1_RE_Temp=UI_Percent_H2_DF/10;
				UC_URART2_TxDataArray_A[12]=0x00;//percent
				UC_URART2_TxDataArray_A[13]=0x00;
			}
			
			UC_URART2_TxDataArray_A[14]=UC_SignalFault_D;//Alarm message
			UC_URART2_TxDataArray_A[15]=UC_C_OnOffStatus_S;//RunStatus
			
			UC_URART2_TxDataArray_A[16]=UC_ActiveMode_S;//激活状态
			
			//外部停机状态
			UC_URART2_TxDataArray_A[18]=UC_RemoteStatusHL_S;
			
			CreateCRC_TX(19);
			break;
		case PulseModeParam://0x02   脉冲模式
			UC_SetPageg_S=0;//退出设置界面
			
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x13;//leagth
			
			UC_URART2_TxDataArray_A[4]=(unsigned char)((LI_PulseFlow_ML_DF>>24)&0xFF);//pulse flow
			UC_URART2_TxDataArray_A[5]=(unsigned char)((LI_PulseFlow_ML_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[6]=(unsigned char)((LI_PulseFlow_ML_DF>>8)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)(LI_PulseFlow_ML_DF&0xFF);
			
			UC_URART2_TxDataArray_A[8]=0x02;//pulse flow unit ML
			
			if(UC_C_OnOffStatus_S==TRUE)
			{
				UI_Percent_H1_RE_Temp=UI_Percent_H2_DF/10;
				UC_URART2_TxDataArray_A[9]=(unsigned char)((UI_Percent_H1_RE_Temp>>8)&0xFF);//percent
				UC_URART2_TxDataArray_A[10]=(unsigned char)(UI_Percent_H1_RE_Temp&0xFF);
			}
			else 
			{
				UC_URART2_TxDataArray_A[9]=0x00;//percent
				UC_URART2_TxDataArray_A[10]=0x00;
			}
			
			UC_URART2_TxDataArray_A[11]=UC_C_OnOffStatus_S;//RunStatus
			
			if(LI_TargetFlowML_H2_DF>=100000) //>LI_TargetFlowML_H2_DF 0.01ML (MIN)  100000=1L
			{
				LI_PulseFlow_L_RE_Temp=LI_TargetFlowML_H2_DF/1000; //L--1.00000L = 1.00L
				UC_URART2_TxDataArray_A[16]=0x01;//1 l/h 2 ml/h
			}
			else
			{
				LI_PulseFlow_L_RE_Temp=LI_TargetFlowML_H2_DF;  //LI_TargetFlowML_H2_DF 90000 =900.00ML
				UC_URART2_TxDataArray_A[16]=0x02;//1 l/h 2 ml/h
			}
			
			UC_URART2_TxDataArray_A[12]=(unsigned char)((LI_PulseFlow_L_RE_Temp>>24)&0xFF);//Target traffic
			UC_URART2_TxDataArray_A[13]=(unsigned char)((LI_PulseFlow_L_RE_Temp>>16)&0xFF);
			UC_URART2_TxDataArray_A[14]=(unsigned char)((LI_PulseFlow_L_RE_Temp>>8)&0xFF);
			UC_URART2_TxDataArray_A[15]=(unsigned char)(LI_PulseFlow_L_RE_Temp&0xFF);
			
			UC_URART2_TxDataArray_A[17]=UC_SignalFault_D;//Alarm message
			UC_URART2_TxDataArray_A[18]=UC_ActiveMode_S;//activation feature
			
			//外部停机
			UC_URART2_TxDataArray_A[19]=UC_RemoteStatusHL_S;
			
			CreateCRC_TX(20);
			break;
			
		case Mode_4_20MAParam://0x03   4-20MA 模式
			UC_SetPageg_S=0;//退出设置界面
		
			//-------------------------------Tx--------------------------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x14;//leagth
			
			if(LI_TargetFlowML_H2_DF>=100000) //>LI_TargetFlowML_H2_DF 00.00ML 100000 = 1L
			{
				LI_TargetFlowML_H2_RE_Temp=LI_TargetFlowML_H2_DF/1000; //L--1.00000L = 1.00L
				UC_URART2_TxDataArray_A[8]=0x01;//1 l/h 2 ml/h
			}
			else
			{
				LI_TargetFlowML_H2_RE_Temp=LI_TargetFlowML_H2_DF;  //LI_TargetFlowML_H2_DF 00.00 ML Min = 2.50ml 00.00 ml
				UC_URART2_TxDataArray_A[8]=0x02;//1 l/h 2 ml/h
			}
			
			UC_URART2_TxDataArray_A[4]=(unsigned char)((LI_TargetFlowML_H2_RE_Temp>>24)&0xFF);//Target traffic
			UC_URART2_TxDataArray_A[5]=(unsigned char)((LI_TargetFlowML_H2_RE_Temp>>16)&0xFF);
			UC_URART2_TxDataArray_A[6]=(unsigned char)((LI_TargetFlowML_H2_RE_Temp>>8)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)(LI_TargetFlowML_H2_RE_Temp&0xFF);
			
			if(UC_C_OnOffStatus_S==TRUE)
			{
				UI_Percent_H1_RE_Temp=UI_Percent_H2_DF/10;
				UC_URART2_TxDataArray_A[9]=(unsigned char)((UI_Percent_H1_RE_Temp>>8)&0xFF);//percent
				UC_URART2_TxDataArray_A[10]=(unsigned char)(UI_Percent_H1_RE_Temp&0xFF);
			}
			else 
			{
				UC_URART2_TxDataArray_A[9]=0x00;//percent
				UC_URART2_TxDataArray_A[10]=0x00;
			}
			
			UC_URART2_TxDataArray_A[11]=UC_C_OnOffStatus_S;//RunStatus
			
			if(UC_04_20MAType_SF==M_0_20MATYPE)
			{
				UC_04_20MA_I1_DF=0;
				UC_04_20MA_I2_DF=200;
			}
			else if(UC_04_20MAType_SF==M_4_20MATYPE)
			{
				UC_04_20MA_I1_DF=40;
				UC_04_20MA_I2_DF=200;
			}
			else if(UC_04_20MAType_SF==M_0_20MAZTTYPE)
			{
				
			}
			else if(UC_04_20MAType_SF==M_0_20MAFTTYPE)
			{
				
			}
			UC_URART2_TxDataArray_A[12]=(unsigned char)((UC_04_20MA_I1_DF>>8)&0xFF);//UC_04_20MA_I1_DF
			UC_URART2_TxDataArray_A[13]=(unsigned char)(UC_04_20MA_I1_DF&0xFF);
			UC_URART2_TxDataArray_A[14]=(unsigned char)((UC_04_20MA_I2_DF>>8)&0xFF);//UC_04_20MA_I2_DF
			UC_URART2_TxDataArray_A[15]=(unsigned char)(UC_04_20MA_I2_DF&0xFF);
			
			UC_URART2_TxDataArray_A[16]=(unsigned char)((UI_C_MA_H1_D>>8)&0xFF);//Input curr MA  
			UC_URART2_TxDataArray_A[17]=(unsigned char)(UI_C_MA_H1_D&0xFF);
			
			UC_URART2_TxDataArray_A[18]=UC_SignalFault_D;//Alarm message
			UC_URART2_TxDataArray_A[19]=UC_ActiveMode_S;//activation feature
			
			//外部停机
			UC_URART2_TxDataArray_A[20]=UC_RemoteStatusHL_S;
			
			CreateCRC_TX(21);
			break;
		case BatchPulseParam://0x04
			UC_SetPageg_S=0;//退出设置界面
		
			//-------------------------------Tx--------------------------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x13;//leagth
			
			UC_URART2_TxDataArray_A[4]=(unsigned char)((LI_LotVolume_DF>>24)&0xFF);//LI_LotVolume_DF
			UC_URART2_TxDataArray_A[5]=(unsigned char)((LI_LotVolume_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[6]=(unsigned char)((LI_LotVolume_DF>>8)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)(LI_LotVolume_DF&0xFF);
			
			if(UC_LotVolumeUnit_SF==1)
			{
				LI_ResidualFlow_ViewD=LI_ResidualFlow_D/1000;// 如何是升，需要缩小1000倍
			}
			else 
			{
				LI_ResidualFlow_ViewD=LI_ResidualFlow_D;
			}
			
			UC_URART2_TxDataArray_A[8]=(unsigned char)((LI_ResidualFlow_ViewD>>24)&0xFF);//LI_ResidualFlow_D
			UC_URART2_TxDataArray_A[9]=(unsigned char)((LI_ResidualFlow_ViewD>>16)&0xFF);
			UC_URART2_TxDataArray_A[10]=(unsigned char)((LI_ResidualFlow_ViewD>>8)&0xFF);
			UC_URART2_TxDataArray_A[11]=(unsigned char)(LI_ResidualFlow_ViewD&0xFF);
			
			UC_URART2_TxDataArray_A[12]=UC_LotVolumeUnit_SF;//UC_LotVolumeUnit_SF Lot Volume l L 2ml
			
			UC_URART2_TxDataArray_A[13]=UC_C_OnOffStatus_S;//RunStatus

			UC_URART2_TxDataArray_A[14]=UC_SignalFault_D;//Alarm message
			UC_URART2_TxDataArray_A[15]=UC_ActiveMode_S;//activation feature
			
			if(UC_C_OnOffStatus_S==TRUE)
			{
				UI_Percent_H1_RE_Temp=UI_Percent_H2_DF/10;
				UC_URART2_TxDataArray_A[16]=(unsigned char)((UI_Percent_H1_RE_Temp>>8)&0xFF);//percent
				UC_URART2_TxDataArray_A[17]=(unsigned char)(UI_Percent_H1_RE_Temp&0xFF);
			}
			else
			{
				UC_URART2_TxDataArray_A[16]=0x00;//percent
				UC_URART2_TxDataArray_A[17]=0x00;
			}
			
			//外部停机
			UC_URART2_TxDataArray_A[18]=UC_RemoteStatusHL_S;
			//批次手动
			UC_URART2_TxDataArray_A[19]=UC_BatchManual_SF;
			
			CreateCRC_TX(20);
			
			
			
			break;
		case CyclicDosingParam://0x05
			UC_SetPageg_S=0;//退出设置界面
		
		 	//-------------------------------Tx--------------------------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x15;//leagth
			
			UC_URART2_TxDataArray_A[4]=(unsigned char)((LI_LoopLotVolume_DF>>24)&0xFF);//LI_LotVolume_DF
			UC_URART2_TxDataArray_A[5]=(unsigned char)((LI_LoopLotVolume_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[6]=(unsigned char)((LI_LoopLotVolume_DF>>8)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)(LI_LoopLotVolume_DF&0xFF);
			
			if(UC_LoopLotVolumeUnit_SF==1)
			{
				LI_LoopResidualFlow_ViewD=LI_LoopResidualFlow_D/1000;// 如何是升，需要缩小1000倍
			}
			else 
			{
				LI_LoopResidualFlow_ViewD=LI_LoopResidualFlow_D;
			}
			
			
			UC_URART2_TxDataArray_A[8]=(unsigned char)((LI_LoopResidualFlow_ViewD>>24)&0xFF);//LI_ResidualFlow_D
			UC_URART2_TxDataArray_A[9]=(unsigned char)((LI_LoopResidualFlow_ViewD>>16)&0xFF);
			UC_URART2_TxDataArray_A[10]=(unsigned char)((LI_LoopResidualFlow_ViewD>>8)&0xFF);
			UC_URART2_TxDataArray_A[11]=(unsigned char)(LI_LoopResidualFlow_ViewD&0xFF);
			
			UC_URART2_TxDataArray_A[12]=UC_LoopLotVolumeUnit_SF;//UC_LotVolumeUnit_SF Lot Volume l L 2ml
			
			UC_URART2_TxDataArray_A[13]=UC_C_OnOffStatus_S;//RunStatus

			UC_URART2_TxDataArray_A[14]=UC_SignalFault_D;//Alarm message
			UC_URART2_TxDataArray_A[15]=UC_ActiveMode_S;//activation feature
			
			if(UC_C_OnOffStatus_S==TRUE)
			{
				UI_Percent_H1_RE_Temp=UI_Percent_H2_DF/10;
				UC_URART2_TxDataArray_A[16]=(unsigned char)((UI_Percent_H1_RE_Temp>>8)&0xFF);//percent
				UC_URART2_TxDataArray_A[17]=(unsigned char)(UI_Percent_H1_RE_Temp&0xFF);
			}
			else
			{
				UC_URART2_TxDataArray_A[16]=0x00;//percent
				UC_URART2_TxDataArray_A[17]=0x00;
			}
			
			 //把 LI_LoopResidualCycleTime_D 剩余总时间 分解成 分钟和秒
    			ConvertTime(LI_LoopResidualCycleTime_D, &ResidualMinutes, &ResidualSeconds);
			UC_URART2_TxDataArray_A[18]=(unsigned char)((ResidualMinutes>>8)&0xFF);//percent;
			UC_URART2_TxDataArray_A[19]=(unsigned char)(ResidualMinutes&0xFF);
			UC_URART2_TxDataArray_A[20]=ResidualSeconds;
			
			//外部停机
			UC_URART2_TxDataArray_A[21]=UC_RemoteStatusHL_S;
			
			CreateCRC_TX(22);
			break;
		case LonginParam: //0x06 用户登录
		 	//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x13;//leagth
			UC_URART2_TxDataArray_A[4]=UC_DeviceId_A[0];//设备ID
			UC_URART2_TxDataArray_A[5]=UC_DeviceId_A[1];
			UC_URART2_TxDataArray_A[6]=UC_DeviceId_A[2];
			UC_URART2_TxDataArray_A[7]=UC_DeviceId_A[3];
			UC_URART2_TxDataArray_A[8]=UC_DeviceId_A[4];
			UC_URART2_TxDataArray_A[9]=UC_DeviceId_A[5];
			UC_URART2_TxDataArray_A[10]=UC_DeviceId_A[6];
			UC_URART2_TxDataArray_A[11]=UC_DeviceId_A[7];
			UC_URART2_TxDataArray_A[12]=UC_PWDUser_A[0];//用户密码
			UC_URART2_TxDataArray_A[13]=UC_PWDUser_A[1];
			UC_URART2_TxDataArray_A[14]=UC_PWDUser_A[2];
			UC_URART2_TxDataArray_A[15]=UC_PWDUser_A[3];
			UC_URART2_TxDataArray_A[16]=UC_PWDUser_A[4];
			UC_URART2_TxDataArray_A[17]=UC_PWDUser_A[5];
			UC_URART2_TxDataArray_A[18]=UC_PWDUser_A[6];
			UC_URART2_TxDataArray_A[19]=UC_PWDUser_A[7];
			
			CreateCRC_TX(20); //返回数据
			break;
		case GetPageReturnParam: //0x07
		 	UC_SetPageg_S=1;//进入设置界面
			
			/*****************************TX**********************************/
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x2F;//leagth
			UC_URART2_TxDataArray_A[4]=UC_ModeRun_SF;//userName
			UC_URART2_TxDataArray_A[5]=(unsigned char)((LI_ManualFlow_L_DF>>24)&0xFF); //LI_ManualFlow_L_DF
			UC_URART2_TxDataArray_A[6]=(unsigned char)((LI_ManualFlow_L_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)((LI_ManualFlow_L_DF>>8)&0xFF);
			UC_URART2_TxDataArray_A[8]=(unsigned char)(LI_ManualFlow_L_DF&0xFF);
			UC_URART2_TxDataArray_A[9]=(unsigned char)((LI_PulseFlow_ML_DF>>24)&0xFF);//LI_PulseFlow_ML_DF
			UC_URART2_TxDataArray_A[10]=(unsigned char)((LI_PulseFlow_ML_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[11]=(unsigned char)((LI_PulseFlow_ML_DF>>8)&0xFF);
			UC_URART2_TxDataArray_A[12]=(unsigned char)(LI_PulseFlow_ML_DF&0xFF);
			//UC_URART2_TxDataArray_A[13]=UC_PulseMemory_SF; //UC_PulseMemory_SF 脉冲记忆 不要
			UC_URART2_TxDataArray_A[13]=UC_BatchManual_SF; //UC_BatchManual_SF //脉冲记忆 没用 改为批次手动
			UC_URART2_TxDataArray_A[14]=UC_04_20MAType_SF;//UC_04_20MAType_SF
			UC_URART2_TxDataArray_A[15]=(unsigned char)(( UC_04_20MA_I1_DF>>8)&0xFF); //UC_04_20MA_I1_DF
			UC_URART2_TxDataArray_A[16]=(unsigned char)(UC_04_20MA_I1_DF&0xFF);
			UC_URART2_TxDataArray_A[17]=(unsigned char)(( UC_04_20MA_Q1_DF>>8)&0xFF); //UC_04_20MA_Q1_DF;
			UC_URART2_TxDataArray_A[18]=(unsigned char)(UC_04_20MA_Q1_DF&0xFF);
			UC_URART2_TxDataArray_A[19]=(unsigned char)(( UC_04_20MA_I2_DF>>8)&0xFF); //UC_04_20MA_I2_DF;			
			UC_URART2_TxDataArray_A[20]=(unsigned char)(UC_04_20MA_I2_DF&0xFF);;
			UC_URART2_TxDataArray_A[21]=(unsigned char)(( UC_04_20MA_Q2_DF>>8)&0xFF); //UC_04_20MA_Q2_DF;;
			UC_URART2_TxDataArray_A[22]=(unsigned char)(UC_04_20MA_Q2_DF&0xFF);
			UC_URART2_TxDataArray_A[23]=(unsigned char)((LI_LotVolume_DF>>24)&0xFF);//LI_LotVolume_DF
			UC_URART2_TxDataArray_A[24]=(unsigned char)((LI_LotVolume_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[25]=(unsigned char)((LI_LotVolume_DF>>8)&0xFF);
			UC_URART2_TxDataArray_A[26]=(unsigned char)(LI_LotVolume_DF&0xFF);
			UC_URART2_TxDataArray_A[27]=UC_LotVolumeUnit_SF; //UC_LotVolumeUnit_SF
			
			ConvertToMinutesAndSeconds(UC_AddTimeSec_DF,&minutes, &seconds);//UC_AddTimeSec_DF
			UC_URART2_TxDataArray_A[28]=(unsigned char)((minutes>>8)&0xFF);
			UC_URART2_TxDataArray_A[29]=(unsigned char)(minutes&0xFF);
			UC_URART2_TxDataArray_A[30]=seconds;
			
			UC_URART2_TxDataArray_A[31]=(unsigned char)((LI_LoopLotVolume_DF>>24)&0xFF);//LI_LoopLotVolume_DF
			UC_URART2_TxDataArray_A[32]=(unsigned char)((LI_LoopLotVolume_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[33]=(unsigned char)((LI_LoopLotVolume_DF>>8)&0xFF);
			UC_URART2_TxDataArray_A[34]=(unsigned char)(LI_LoopLotVolume_DF&0xFF);
			UC_URART2_TxDataArray_A[35]=UC_LoopLotVolumeUnit_SF; //UC_LoopLotVolumeUnit_SF
			
			ConvertToMinutesAndSeconds(UC_LoopADDTime_DF,&minutes, &seconds); //UC_LoopADDTime_DF
			UC_URART2_TxDataArray_A[36]=(unsigned char)((minutes>>8)&0xFF);
			UC_URART2_TxDataArray_A[37]=(unsigned char)(minutes&0xFF);
			UC_URART2_TxDataArray_A[38]=seconds;
			
			ConvertToMinutesAndSeconds( UC_LoopCycleTime_DF,&minutes, &seconds); // UC_LoopCycleTime_DF
			UC_URART2_TxDataArray_A[39]=(unsigned char)((minutes>>8)&0xFF);
			UC_URART2_TxDataArray_A[40]=(unsigned char)(minutes&0xFF);
			UC_URART2_TxDataArray_A[41]=seconds;
			
			ConvertToMinutesAndSeconds(UC_LoopStartDelayTime_DF,&minutes, &seconds); //UC_LoopStartDelayTime_DF
			UC_URART2_TxDataArray_A[42]=(unsigned char)((minutes>>8)&0xFF);
			UC_URART2_TxDataArray_A[43]=(unsigned char)(minutes&0xFF);
			UC_URART2_TxDataArray_A[44]=seconds;
			
			UC_URART2_TxDataArray_A[45]=UC_IsCalibrated_DF; //UC_IsCalibrated_DF
			
			UC_URART2_TxDataArray_A[46]=(unsigned char)((UI_UserCalibrateVolML_H1_DF>>8)&0xFF); //用户标定容积
			UC_URART2_TxDataArray_A[47]=(unsigned char)(UI_UserCalibrateVolML_H1_DF&0xFF);
			
			CreateCRC_TX(48);
			break;
		case SetVolumeParam: //0x08 厂家标定
		
			UC_ReceivedData_4A[0]=UI_URT2_REC_D[4];
			UC_ReceivedData_4A[1]=UI_URT2_REC_D[5];
			UC_ReceivedData_4A[2]=UI_URT2_REC_D[6];
			UC_ReceivedData_4A[3]=UI_URT2_REC_D[7];
			UI_CalibrateVolML_H1_DF=ConvertToManualFlow(UC_ReceivedData_4A); //获取标定流量 比如 4200ML
			
			//标定获取最大流量
			if(UI_CalibrateVolML_H1_DF!=0x00)
			{
				RatedFlow(UI_CalibrateVolML_H1_DF);//get Base UI_MaxFlowML_H2_DF  	//通过标定容积获取最大流量
			}
			UI_UserCalibrateVolML_H1_DF=UI_CalibrateVolML_H1_DF;//厂家标定数据赋值给用过标定流量
			
			UI_MAXFlow_D=UI_MaxFlowML_H2_DF/10000;//最大流量 ml_H2 显示 L_H1 
			
			UC_ReceivedData_2A[0]=UI_URT2_REC_D[8];
			UC_ReceivedData_2A[1]=UI_URT2_REC_D[9];
			UI_FactoryRatedMAXFlow_DF=ConvertToManualFlow16(UC_ReceivedData_2A);//额定流量  200.0L 精确到L 后没有小数点  厂家额定流量
			
			
			
			UI_UserRatedMAXFlow_DF=UI_FactoryRatedMAXFlow_DF;//赋值给我用户额定流
			UI_RatedMAXFlow_DF=UI_FactoryRatedMAXFlow_DF;//基础计算额定流量
			/*********************计算数据数据*********************************************/
			UI_RatedFlow_Temp=UI_RatedMAXFlow_DF;//设置的额定流量
			UI_RatedFlow_Temp=UI_RatedFlow_Temp*1000000;//为了精度扩大2000*1000000= 200.000.0000 L
			
			UI_MaxFlowML_H2_Temp=UI_MaxFlowML_H2_DF/100; //减小精度到250.000 L
			
			//额定功率占最大功率的百分比
			UI_RatedPercent_D=UI_RatedFlow_Temp/UI_MaxFlowML_H2_Temp;//得到额定功率是最大功率的百分之多少 比如是9523 就是95.23%
			
			//通过额定功率获取额定最大脉宽
			LI_RatedMAXPulseWidth_DF=UI_RatedPercent_D*MAX_PMW_US;
			LI_RatedMAXPulseWidth_DF=LI_RatedMAXPulseWidth_DF/10000;
			
			KS_PWMPULSE=LI_RatedMAXPulseWidth_DF;//快吸慢推最大转速
			
			//修改手动最大流量
			LI_ManualFlow_L_DF=UI_FactoryRatedMAXFlow_DF;
			LI_ManualFlow_L_DF=LI_ManualFlow_L_DF*1000;
			
			UC_IsCalibrated_DF=TRUE;
			/*****************************TX**********************************/
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x05;//leagth
			UC_URART2_TxDataArray_A[4]=(unsigned char)((UI_UserRatedMAXFlow_DF>>8)&0xFF); //UI_MAXFlow_D 最大流量
			UC_URART2_TxDataArray_A[5]=(unsigned char)(UI_UserRatedMAXFlow_DF&0xFF);
			CreateCRC_TX(6);
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;
		case StartStopCalParam: //0x09  厂家启动
			
			UC_SwitchCali_S=UI_URT2_REC_D[4];
			UC_ReceivedData_2A[0]=UI_URT2_REC_D[5];
			UC_ReceivedData_2A[1]=UI_URT2_REC_D[6];
			UC_SetZhuanSu_D=ConvertToManualFlow16(UC_ReceivedData_2A);//设置标定转速
			
			if(UI_SlowModeStatus_SF>0)
			{
				UC_SetZhuanSu_D=KS_PWMPULSE*0.8;//快吸的80%
			}
			else 
			{
				UC_SetZhuanSu_D=UC_SetZhuanSu_D*10.8;//转速转化成脉冲
			}
			
			/*
			if(UC_SwitchCali_S==TRUE)
			{
				UI_ZeroCali_C=0;//冲程次数
			}*/
			UI_ZeroCali_C=0;//冲程次数
			UC_StartZeroing_S=FALSE;//关闭归零
			Switch_Cali(UC_SetZhuanSu_D); //启动电机运行 设置运行脉宽
			//---------------------------------------------------------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			
			CreateCRC_TX(5);
			break;
		case SetAllParam: //0x0A
			GetSetupParameters(); //set ALL Param
			
			/*****************************TX**********************************/
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5);
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;			
		case SetONOFFStateParam: //0x0B
			
			UC_C_OnOff_Temp=UI_URT2_REC_D[4];
			if(UC_C_OnOff_Temp==0x55||UC_C_OnOff_Temp==0xAA)
			{
				UC_C_OnOff_S=UC_C_OnOff_Temp;
				UC_C_OnOffStatus_S=UC_C_OnOff_Temp;
				//设备开机设置脉宽
				if(UC_C_OnOff_S==TRUE)
				{
					UC_C_OnOffStatus_S=WAIT;
					RunCal();//设置脉宽
					UI_PulseWidth_D=TDR07_Percent_C;//当前脉宽
					setPWMPulseWidth(); 
					ClearStatus();//清除状态位
					
					//判断批次和循环投机 开机状态是否应该为暂停
					if(UC_ModeRun_SF==BATCH_MODE)
					{
						 get_ResidualFlow(); 
					}
					else if(UC_ModeRun_SF==CYCLIC_MODE)
					{
						get_LoopRun();   
					}
				}
				else
				{
					CLoseClearAlarmS();
				}
				
			}
			else 
			{
				UC_C_OnOff_S=FALSE; //数据传输异常关机
				UC_C_OnOffStatus_S=FALSE; //数据传输异常关机
			}
			
			ClearStatusAndData();//清除一些异常的状态和数据
			
			//--------------------TX--------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5);
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;	
		case GetCaliInfoParam: //0x0C
			
			//--------------------TX--------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x08;//leagth
			
			//UI_ZeroCali_Temp=UI_ZeroCali_C/2;
			UI_ZeroCali_Temp=UI_ZeroCali_C;
			UC_URART2_TxDataArray_A[4]=(unsigned char)((UI_ZeroCali_Temp>>8)&0xFF); //UI_ZeroCali_C 冲程数量
			UC_URART2_TxDataArray_A[5]=(unsigned char)(UI_ZeroCali_Temp&0xFF);
			
			UC_URART2_TxDataArray_A[6]=UC_SwitchCali_S; //UC_SwitchCali_S 开关
			
			UC_URART2_TxDataArray_A[7]=(unsigned char)((UI_MAXFlow_D>>8)&0xFF); //UI_MAXFlow_D 最大流量
			UC_URART2_TxDataArray_A[8]=(unsigned char)(UI_MAXFlow_D&0xFF);
			
			CreateCRC_TX(9);
			break;	
		case ZeroingParam: //0x0D 厂家归零
			//--------------------TX--------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x0A;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			
			
			UC_URART2_TxDataArray_A[5]=(unsigned char)((UI_CalibrateVolML_H1_DF>>24)&0xFF);//UI_CalibrateVolML_H1_DF 标定容积 
			UC_URART2_TxDataArray_A[6]=(unsigned char)((UI_CalibrateVolML_H1_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)((UI_CalibrateVolML_H1_DF>>8)&0xFF);  
			UC_URART2_TxDataArray_A[8]=(unsigned char)(UI_CalibrateVolML_H1_DF&0xFF);
			
			UC_URART2_TxDataArray_A[9]=(unsigned char)((UI_RatedMAXFlow_DF>>8)&0xFF);  //UI_RatedFlow_D 额定最大流量 
			UC_URART2_TxDataArray_A[10]=(unsigned char)(UI_RatedMAXFlow_DF&0xFF);
			
			StartZero();//归零上支点
			CreateCRC_TX(11);
			break;	
		case Get04_20MAADParam: //0x0E
			//--------------------TX--------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x05;//leagth
			
			ADGet4_20MA();//Obtain the 4-20 mA AD value
			UC_URART2_TxDataArray_A[4]=(unsigned char)((UI_4_20MA_AD_D>>8)&0xFF); //UI_ZeroCali_C
			UC_URART2_TxDataArray_A[5]=(unsigned char)(UI_4_20MA_AD_D&0xFF);
			
			CreateCRC_TX(6);
			break;	
		case Calib04_20MAParam: //0x0F
		
			UC_ReceivedData_2A[0]=UI_URT2_REC_D[4];
			UC_ReceivedData_2A[1]=UI_URT2_REC_D[5];
			UI_C_SetMinAdValue4_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //4mA AD value
			
			UC_ReceivedData_2A[0]=UI_URT2_REC_D[6];
			UC_ReceivedData_2A[1]=UI_URT2_REC_D[7];
			UI_C_SetMaxAdValue20_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //20mA AD value
			
			/*get 4-20mA AD K and B*/
			GetSlopeIntercept_KB();
			
			//--------------------TX--------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5);
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;	
		case GetOtherSetParam: //0x10
		
			/*****************************TX**********************************/
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x2C;//leagth
			UC_URART2_TxDataArray_A[4]=(unsigned char)((UI_BaudRateStatus_SF>>8)&0xFF); //UI_BaudRateStatus_SF;
			UC_URART2_TxDataArray_A[5]=(unsigned char)(UI_BaudRateStatus_SF&0xFF);
			//UI_BaudRateStatusTemp_S=UI_BaudRateStatus_SF;//保存一个 485 状态临时变量
			
			UC_URART2_TxDataArray_A[6]=(unsigned char)((UI_CalibParam_MA_DF>>8)&0xFF);//UI_CalibParam_MA_DF;
			UC_URART2_TxDataArray_A[7]=(unsigned char)(UI_CalibParam_MA_DF&0xFF);
			
			UC_URART2_TxDataArray_A[8]=(unsigned char)((UI_SlowModeStatus_SF>>8)&0xFF);//UI_SlowModeStatus_SF
			UC_URART2_TxDataArray_A[9]=(unsigned char)(UI_SlowModeStatus_SF&0xFF);
			
			UC_URART2_TxDataArray_A[10]=(unsigned char)((UI_IsPessureSwitch_SF>>8)&0xFF);//UI_IsPessureSwitch_SF;  压力开关
			UC_URART2_TxDataArray_A[11]=(unsigned char)(UI_IsPessureSwitch_SF&0xFF);
			
			UC_URART2_TxDataArray_A[12]=(unsigned char)((UI_PessureRange_DF>>8)&0xFF);//UI_PessureRange_DF;
			UC_URART2_TxDataArray_A[13]=(unsigned char)(UI_PessureRange_DF&0xFF);
			
			UC_URART2_TxDataArray_A[14]=(unsigned char)((UI_PessureHigh_DF>>8)&0xFF);//UI_PessureHigh_DF
			UC_URART2_TxDataArray_A[15]=(unsigned char)(UI_PessureHigh_DF&0xFF);
			
			UC_URART2_TxDataArray_A[16]=(unsigned char)((UI_PessureLow_DF>>8)&0xFF);//UI_PessureLow_DF;
			UC_URART2_TxDataArray_A[17]=(unsigned char)(UI_PessureLow_DF&0xFF);
			
			UC_URART2_TxDataArray_A[18]=(unsigned char)((UI_IsLevelSwitch_SF>>8)&0xFF);//UI_IsLevelSwitch_SF  低液位开关
			UC_URART2_TxDataArray_A[19]=(unsigned char)(UI_IsLevelSwitch_SF&0xFF);
			
			UC_URART2_TxDataArray_A[20]=(unsigned char)((UI_LevelHigh_DF>>8)&0xFF);//UI_LevelHigh_DF
			UC_URART2_TxDataArray_A[21]=(unsigned char)(UI_LevelHigh_DF&0xFF);
			
			UC_URART2_TxDataArray_A[22]=(unsigned char)((UI_LevelLow_DF>>8)&0xFF);//UI_LevelLow_DF
			UC_URART2_TxDataArray_A[23]=(unsigned char)(UI_LevelLow_DF&0xFF);
			
			UC_URART2_TxDataArray_A[24]=UI_Year_DF;//UI_Year_DF
			
			UC_URART2_TxDataArray_A[25]=UI_Month_DF;//UI_Month_DF
			
			UC_URART2_TxDataArray_A[26]=UI_Day_DF;//UI_Day_DF
			
			UC_URART2_TxDataArray_A[27]=UI_Hour_DF;//UI_Hour_DF
			
			UC_URART2_TxDataArray_A[28]=UI_Minute_DF;//UI_Minute_DF
			
			UC_URART2_TxDataArray_A[29]=UI_RunRelayOutput_SF;//UI_RemoteControl_SF 运行继电器
			
			UC_URART2_TxDataArray_A[30]=UI_RemoteControl_SF;//UI_RemoteControl_SF  外部停机
			
			UC_URART2_TxDataArray_A[31]=UI_FaultyRelayOutput_SF;//UI_RemoteControl_SF  故障继电器
			
			/**************************password*************************/
			
			UC_URART2_TxDataArray_A[32]=UC_PWDUser_A[0];
			UC_URART2_TxDataArray_A[33]=UC_PWDUser_A[1];
			UC_URART2_TxDataArray_A[34]=UC_PWDUser_A[2];
			UC_URART2_TxDataArray_A[35]=UC_PWDUser_A[3];
			UC_URART2_TxDataArray_A[36]=UC_PWDUser_A[4];
			UC_URART2_TxDataArray_A[37]=UC_PWDUser_A[5];
			UC_URART2_TxDataArray_A[38]=UC_PWDUser_A[6];
			UC_URART2_TxDataArray_A[39]=UC_PWDUser_A[7];
			
			/**************************password*************************/
			UC_URART2_TxDataArray_A[40]=UI_ScreenSaver_SF;//UI_ScreenSaver_SF;
			
			UC_URART2_TxDataArray_A[41]=(unsigned char)((UI_ScreenSaverTime_DF>>8)&0xFF);//UI_ScreenSaverTime_DF;
			UC_URART2_TxDataArray_A[42]=(unsigned char)(UI_ScreenSaverTime_DF&0xFF);
			
			UC_URART2_TxDataArray_A[43]=UI_MachineCode_DF;//机器码
			
			UC_URART2_TxDataArray_A[44]=UI_LanguageSet_SF;//UI_LanguageSet_SF  语言设置
			
			UC_URART2_TxDataArray_A[45]=(unsigned char)((UC_PowerOffLock_SF>>8)&0xFF);// 0=关闭断电保护，1=开启断电保护
			UC_URART2_TxDataArray_A[46]=(unsigned char)(UC_PowerOffLock_SF&0xFF);
			
			CreateCRC_TX(47);
			break;
		case SetOtherSetParam: //0x11
			SetOtherParam(); //设置其他参数
			
			/*****************************TX**********************************/
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x05;//leagth
			UC_URART2_TxDataArray_A[4]=(unsigned char)((UI_UserRatedMAXFlow_DF>>8)&0xFF); //UI_MAXFlow_D 最大流量
			UC_URART2_TxDataArray_A[5]=(unsigned char)(UI_UserRatedMAXFlow_DF&0xFF);
			CreateCRC_TX(6);
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;
		
		case GetInfoTest: //0x12  
			// 将宏定义的字符串复制到数组中 --产品型号
    			strcpy((char *)product_model_array, PRODUCT_MODEL);//8位
			strcpy((char *)product_number_array, PRODUCT_SERIAL_NUMBER);//10位 产品序列号
			strcpy((char *)software_number_array, SOFTWARE_NUMBER);//8位 产品软件号
			//--------------------TX--------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x1D;//leagth
			
			// 从 product_model_array 数组的第 0 个元素开始复制 8 个元素到 UC_URART2_TxDataArray_A 数组 第4个元素开始
			memcpy(UC_URART2_TxDataArray_A + 4, product_model_array, 8);
			//memcpy(UC_URART2_TxDataArray_A + 12, product_number_array, 10);   厂家定八位序列号，就是设备ID号
			UC_URART2_TxDataArray_A[12]=UC_DeviceId_A[0];
			UC_URART2_TxDataArray_A[13]=UC_DeviceId_A[1];
			UC_URART2_TxDataArray_A[14]=UC_DeviceId_A[2];
			UC_URART2_TxDataArray_A[15]=UC_DeviceId_A[3];
			UC_URART2_TxDataArray_A[16]=UC_DeviceId_A[4];
			UC_URART2_TxDataArray_A[17]=UC_DeviceId_A[5];
			UC_URART2_TxDataArray_A[18]=UC_DeviceId_A[6];
			UC_URART2_TxDataArray_A[19]=UC_DeviceId_A[7];
			UC_URART2_TxDataArray_A[20]=0x20;
			UC_URART2_TxDataArray_A[21]=0x20;
			
			memcpy(UC_URART2_TxDataArray_A + 22, software_number_array, 8);
			
			CreateCRC_TX(30);
			break;
		case SetManualFlowParam: //0x13设置手动目标流量
		
			UC_ReceivedData_4A[0]=UI_URT2_REC_D[4];
			UC_ReceivedData_4A[1]=UI_URT2_REC_D[5];
			UC_ReceivedData_4A[2]=UI_URT2_REC_D[6];
			UC_ReceivedData_4A[3]=UI_URT2_REC_D[7];
			LI_ManualFlow_L_DF=ConvertToManualFlow(UC_ReceivedData_4A); //Manual L  0.0025L min, 2.5ML  LI_ManualFlowML_H1_DF 
			
			/*****************************TX**********************************/
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5);
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;	
		case FactoryLoginParam: //0x14厂家登录
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x13;//leagth
			UC_URART2_TxDataArray_A[4]=UC_DeviceId_A[0];//设备ID
			UC_URART2_TxDataArray_A[5]=UC_DeviceId_A[1];
			UC_URART2_TxDataArray_A[6]=UC_DeviceId_A[2];
			UC_URART2_TxDataArray_A[7]=UC_DeviceId_A[3];
			UC_URART2_TxDataArray_A[8]=UC_DeviceId_A[4];
			UC_URART2_TxDataArray_A[9]=UC_DeviceId_A[5];
			UC_URART2_TxDataArray_A[10]=UC_DeviceId_A[6];
			UC_URART2_TxDataArray_A[11]=UC_DeviceId_A[7];
			UC_URART2_TxDataArray_A[12]=UC_PWDFactory_A[0];//厂家密码
			UC_URART2_TxDataArray_A[13]=UC_PWDFactory_A[1];
			UC_URART2_TxDataArray_A[14]=UC_PWDFactory_A[2];
			UC_URART2_TxDataArray_A[15]=UC_PWDFactory_A[3];
			UC_URART2_TxDataArray_A[16]=UC_PWDFactory_A[4];
			UC_URART2_TxDataArray_A[17]=UC_PWDFactory_A[5];
			UC_URART2_TxDataArray_A[18]=UC_PWDFactory_A[6];
			UC_URART2_TxDataArray_A[19]=UC_PWDFactory_A[7];
			
			CreateCRC_TX(20); //返回数据
			break;	
		case ModifyFactoryPWDParam: //0x15 修改厂家密码
			
			UC_PWDFactory_A[0]=UI_URT2_REC_D[4];
			UC_PWDFactory_A[1]=UI_URT2_REC_D[5];
			UC_PWDFactory_A[2]=UI_URT2_REC_D[6];
			UC_PWDFactory_A[3]=UI_URT2_REC_D[7];
			UC_PWDFactory_A[4]=UI_URT2_REC_D[8];
			UC_PWDFactory_A[5]=UI_URT2_REC_D[9];
			UC_PWDFactory_A[6]=UI_URT2_REC_D[10];
			UC_PWDFactory_A[7]=UI_URT2_REC_D[11];
		
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5); //返回数据
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;	
		case ModifyDeviceIDParam: //0x16 修改设备ID
			
			UC_DeviceId_A[0]=UI_URT2_REC_D[4];
			UC_DeviceId_A[1]=UI_URT2_REC_D[5];
			UC_DeviceId_A[2]=UI_URT2_REC_D[6];
			UC_DeviceId_A[3]=UI_URT2_REC_D[7];
			UC_DeviceId_A[4]=UI_URT2_REC_D[8];
			UC_DeviceId_A[5]=UI_URT2_REC_D[9];
			UC_DeviceId_A[6]=UI_URT2_REC_D[10];
			UC_DeviceId_A[7]=UI_URT2_REC_D[11];
		
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5); //返回数据
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;
		case SetUserVolumeParam: //0x17 设置用户标定
			UC_ReceivedData_4A[0]=UI_URT2_REC_D[4];
			UC_ReceivedData_4A[1]=UI_URT2_REC_D[5];
			UC_ReceivedData_4A[2]=UI_URT2_REC_D[6];
			UC_ReceivedData_4A[3]=UI_URT2_REC_D[7];
			UI_UserCalibrateVolML_H1_DF=ConvertToManualFlow(UC_ReceivedData_4A); //获取用户标定流量 比如 4200ML
			
			//标定获取最大流量
			if(UI_UserCalibrateVolML_H1_DF!=0x00)
			{
				RatedFlow(UI_UserCalibrateVolML_H1_DF);//get Base UI_MaxFlowML_H2_DF  	//通过用户标定容积获取最大流量
			}
			UI_MAXFlow_D=UI_MaxFlowML_H2_DF/10000;//最大流量 ml_H2 显示 L_H1  用户标定的新溶液的新的最大流量
			
			MaxFlowToManualFlow();//是否重新设置手动流量
			
			/*************************************用户标定如果最大流量小于200L 如180L那180L 就是100%  bug**************************************************/
			//50% max
			if(UI_SlowModeStatus_SF==1)
			{
				UI_FactoryRatedMAXFlowSlowMode_DF=UI_FactoryRatedMAXFlow_DF/2;
			}
			else if(UI_SlowModeStatus_SF==2) //25% max
			{
				UI_FactoryRatedMAXFlowSlowMode_DF=UI_FactoryRatedMAXFlow_DF/4;
			}
			else //UI_SlowModeStatus_SF  off
			{
				UI_FactoryRatedMAXFlowSlowMode_DF=UI_FactoryRatedMAXFlow_DF;
			}
			//用户标定的最大流量小于用户额定流量
			if(UI_MAXFlow_D<(UI_FactoryRatedMAXFlowSlowMode_DF))
			{
				UI_UserRatedMAXFlow_DF=UI_MAXFlow_D;//最大流量 等于额定流
				UI_RatedMAXFlow_DF=UI_UserRatedMAXFlow_DF; //赋值基础额定流
			}
			else
			{
				UI_UserRatedMAXFlow_DF=UI_FactoryRatedMAXFlowSlowMode_DF; //变量流量粘度正常 泵的最大流量 大于厂家标定流量
				UI_RatedMAXFlow_DF=UI_FactoryRatedMAXFlowSlowMode_DF; //赋值基础参数
				
			}
			/*************************************用户标定如果最大流量小于200L 如180L那180L 就是100%  bug**************************************************/
			
			
			/*********************计算数据数据*********************************************/
			UI_RatedFlow_Temp=UI_RatedMAXFlow_DF;//设置的额定最大流量
			UI_RatedFlow_Temp=UI_RatedFlow_Temp*1000000;//为了精度扩大2000*1000000= 200.000.0000 L
			
			UI_MaxFlowML_H2_Temp=UI_MaxFlowML_H2_DF/100; //减小精度到250.000 L
			
			//额定功率占最大功率的百分比
			UI_RatedPercent_D=UI_RatedFlow_Temp/UI_MaxFlowML_H2_Temp;//得到额定功率是最大功率的百分之多少 比如是9523 就是95.23%
			
			//通过额定功率获取额定最大脉宽
			if(UI_SlowModeStatus_SF==1)
			{
				LI_RatedMAXPulseWidth_DF=UI_RatedPercent_D*MAX_PMW_US_50;
			}
			else if(UI_SlowModeStatus_SF==2) //25% max
			{
				LI_RatedMAXPulseWidth_DF=UI_RatedPercent_D*MAX_PMW_US_25;
			}
			else 
			{
				LI_RatedMAXPulseWidth_DF=UI_RatedPercent_D*MAX_PMW_US;
			}
			//LI_RatedMAXPulseWidth_DF=UI_RatedPercent_D*MAX_PMW_US;
			LI_RatedMAXPulseWidth_DF=LI_RatedMAXPulseWidth_DF/10000;
			
			KS_PWMPULSE=LI_RatedMAXPulseWidth_DF;//快吸慢推 当前的最大速度
			
			UC_IsCalibrated_DF=TRUE;
			/*****************************TX**********************************/
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x05;//leagth
			UC_URART2_TxDataArray_A[4]=(unsigned char)((UI_UserRatedMAXFlow_DF>>8)&0xFF); //UI_MAXFlow_D 最大流量
			UC_URART2_TxDataArray_A[5]=(unsigned char)(UI_UserRatedMAXFlow_DF&0xFF);
			CreateCRC_TX(6);
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;
		case StartStopUserCalParam: //0x18 启停用户标定
			UC_SwitchCali_S=UI_URT2_REC_D[4]; //开关量
			
			if(UI_SlowModeStatus_SF>0)
			{
				UC_SetZhuanSu_D=KS_PWMPULSE*0.8;//快吸的80%
			}
			else 
			{
				UC_SetZhuanSu_D=USERCal_ZS*10.8;//用户标定固定转速2000*10.8 系数得到脉宽
			}
			
			/*
			if(UC_SwitchCali_S==TRUE)
			{
				UI_ZeroCali_C=0;//冲程次数
			}*/
			UI_ZeroCali_C=0;//冲程次数
			UC_StartZeroing_S=FALSE;//关闭归零
			Switch_Cali(UC_SetZhuanSu_D); //启动电机运行 设置运行脉宽
			//---------------------------------------------------------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			
			CreateCRC_TX(5);
			break;	
		case GetUserCaliInfoParam: //0x19 获取用户标定信息
			
			//--------------------TX--------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x06;//leagth
			
			//UI_ZeroCali_Temp=UI_ZeroCali_C/2;
			UI_ZeroCali_Temp=UI_ZeroCali_C;
			UC_URART2_TxDataArray_A[4]=(unsigned char)((UI_ZeroCali_Temp>>8)&0xFF); //UI_ZeroCali_C 冲程数量
			UC_URART2_TxDataArray_A[5]=(unsigned char)(UI_ZeroCali_Temp&0xFF);
			
			UC_URART2_TxDataArray_A[6]=UC_SwitchCali_S; //UC_SwitchCali_S 开关
			
			CreateCRC_TX(7);
			break;	
		case UserZeroingParam: //0x1A 用户标点归零
			//--------------------TX--------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x08;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;

			UC_URART2_TxDataArray_A[5]=(unsigned char)((UI_UserCalibrateVolML_H1_DF>>24)&0xFF);//UI_CalibrateVolML_H1_DF 标定容积 
			UC_URART2_TxDataArray_A[6]=(unsigned char)((UI_UserCalibrateVolML_H1_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)((UI_UserCalibrateVolML_H1_DF>>8)&0xFF);  
			UC_URART2_TxDataArray_A[8]=(unsigned char)(UI_UserCalibrateVolML_H1_DF&0xFF);
			
			StartZero();//归零上支点---------------------------没有电机暂时测试先注释后面要打开
			CreateCRC_TX(9);
			break;	
		case GetManualFlowParam: //0x1B 获取手动流量
			//--------------------TX--------------------------
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x07;//leagth

			UC_URART2_TxDataArray_A[4]=(unsigned char)((LI_ManualFlow_L_DF>>24)&0xFF);//UI_CalibrateVolML_H1_DF 标定容积 
			UC_URART2_TxDataArray_A[5]=(unsigned char)((LI_ManualFlow_L_DF>>16)&0xFF);
			UC_URART2_TxDataArray_A[6]=(unsigned char)((LI_ManualFlow_L_DF>>8)&0xFF);  
			UC_URART2_TxDataArray_A[7]=(unsigned char)(LI_ManualFlow_L_DF&0xFF);
			CreateCRC_TX(8);
			break;	
		case ModifyUserPWDParam: //0x1C 修改用户密码
			
			UC_PWDUser_A[0]=UI_URT2_REC_D[4];//userpassword;
			UC_PWDUser_A[1]=UI_URT2_REC_D[5];
			UC_PWDUser_A[2]=UI_URT2_REC_D[6];
			UC_PWDUser_A[3]=UI_URT2_REC_D[7];
			UC_PWDUser_A[4]=UI_URT2_REC_D[8];
			UC_PWDUser_A[5]=UI_URT2_REC_D[9];
			UC_PWDUser_A[6]=UI_URT2_REC_D[10];
			UC_PWDUser_A[7]=UI_URT2_REC_D[11];
		
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5); //返回数据
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;	
		case ModifySysTimeParam: //0x1D 修改系统时间
			
			UI_Year_DF=UI_URT2_REC_D[4];//UI_Year_DF
			UC_C_Time_Y=hexToDec(UI_Year_DF);
			
			UI_Month_DF=UI_URT2_REC_D[5];//UI_Month_DF
			UC_C_Time_MM=hexToDec(UI_Month_DF);
			
			UI_Day_DF=UI_URT2_REC_D[6];//UI_Day_DF
			UC_C_Time_D=hexToDec(UI_Day_DF);
			
			UI_Hour_DF=UI_URT2_REC_D[7];//UI_Hour_DF
			UC_C_Time_H=hexToDec(UI_Hour_DF);
			
			UI_Minute_DF=UI_URT2_REC_D[8];//UI_Minute_DF
			UC_C_Time_M=hexToDec(UI_Minute_DF);
			
			Change_SystemTime(UC_C_Time_M,UC_C_Time_H,UC_C_Time_D,UC_C_Time_MM,UC_C_Time_Y); //设置时间方法

		
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5); //返回数据
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;	
		case ModifyScreenSaverParam: //0x1E 修改屏保参数
			
			UI_ScreenSaver_SF=UI_URT2_REC_D[4];//UI_ScreenSaver_SF
			UC_ReceivedData_2A[0]=UI_URT2_REC_D[5];
			UC_ReceivedData_2A[1]=UI_URT2_REC_D[6];
			UI_ScreenSaverTime_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_ScreenSaverTime_DF
		
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5); //返回数据
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;	
		case ModifySysLanguageParam: //0x1F 修改系统语言
			
			UI_LanguageSet_SF=UI_URT2_REC_D[4];//UI_LanguageSet_SF
		
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5); //返回数据
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;	
		case GETSysInfoParam: //0x20 获取系统基础信息
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x18;//leagth
			//日期
			UC_URART2_TxDataArray_A[4]=Time_Year;
			UC_URART2_TxDataArray_A[5]=Time_Month;
			UC_URART2_TxDataArray_A[6]=Time_Day;
			UC_URART2_TxDataArray_A[7]=Time_Hour;
			UC_URART2_TxDataArray_A[8]=Time_Minute;
			//累计立方
			UC_URART2_TxDataArray_A[9]=(unsigned char)((UI_FlowInM3_DF>>8)&0xFF);  
			UC_URART2_TxDataArray_A[10]=(unsigned char)(UI_FlowInM3_DF&0xFF);
			//累计流量L
			UC_URART2_TxDataArray_A[11]=(unsigned char)((UI_TotalFlowL_H1_DF>>8)&0xFF);  
			UC_URART2_TxDataArray_A[12]=(unsigned char)(UI_TotalFlowL_H1_DF&0xFF);
			
			UC_URART2_TxDataArray_A[13]=UI_BaudRateStatus_SF;//波特率
			UC_URART2_TxDataArray_A[14]=UI_SlowModeStatus_SF;//慢速模式
			if(UI_IsPessureSwitch_SF==0xAA)
			{
				UC_URART2_TxDataArray_A[15]=0x01;//压力检测开关
			}
			else 
			{
				UC_URART2_TxDataArray_A[15]=0x00;//压力检测开关
			}
			UC_URART2_TxDataArray_A[16]=UI_RemoteControl_SF;//外部停机
			UC_URART2_TxDataArray_A[17]=UI_RunRelayOutput_SF;//运行继电器
			UC_URART2_TxDataArray_A[18]=UI_FaultyRelayOutput_SF;//故障继电器
			UC_URART2_TxDataArray_A[19]=UI_MachineCode_DF;//机器码	
			
			UC_URART2_TxDataArray_A[20]=(unsigned char)((UI_UserRatedMAXFlow_DF>>8)&0xFF);   //用户标定最大流量
			UC_URART2_TxDataArray_A[21]=(unsigned char)(UI_UserRatedMAXFlow_DF&0xFF);
			
			if(UI_IsLevelSwitch_SF==0xAA)
			{
				UC_URART2_TxDataArray_A[22]=0x01;//空桶开关
			}
			else 
			{
				UC_URART2_TxDataArray_A[22]=0x00;//空桶开关
			}
			
			UC_URART2_TxDataArray_A[23]=(unsigned char)((UI_CurPressure_Level>>8)&0xFF);   //当前压力
			UC_URART2_TxDataArray_A[24]=(unsigned char)(UI_CurPressure_Level&0xFF);
			
			if(UC_PowerOffLock_SF==0xAA) // 0=关闭断电保护，1=开启断电保护
			{
				UC_URART2_TxDataArray_A[25]=0x01;//压力检测开关
			}
			else 
			{
				UC_URART2_TxDataArray_A[25]=0x00;//压力检测开关
			}
			
			
			CreateCRC_TX(25);
			break;	
		case GetAlarmInfoParam: //0x21 获取报警信息
			//--------------------TX--------------------------
			offset = 4;  // 从索引4开始填充
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x21;//leagth
			
			for (i = 0; i < MAX_ALARMS; i++)
			{
				  // 报警类型（1字节）
				  UC_URART2_TxDataArray_A[offset++] = alarm_buffer[i].type;
				        
				  // 时间信息（5字节）
				  UC_URART2_TxDataArray_A[offset++] = alarm_buffer[i].year;
				  UC_URART2_TxDataArray_A[offset++] = alarm_buffer[i].month;
				  UC_URART2_TxDataArray_A[offset++] = alarm_buffer[i].day;
				  UC_URART2_TxDataArray_A[offset++] = alarm_buffer[i].hour;
				  UC_URART2_TxDataArray_A[offset++] = alarm_buffer[i].minute;
			}
			
			CreateCRC_TX(34);
			break;
			
		case ClearTotalFlowParam: //0x22 清除累计流量
		
			UI_TotalFlowL_H1_DF=0;//清除累计L
			UI_FlowInM3_DF=0;//清除累计M3
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5); //返回数据
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;
		case ReSetUserPassword: //0x23 重置用戶密碼
			//用户密码 0000  0x30 ASCII等于0
			UC_PWDUser_A[0]=0x30;UC_PWDUser_A[1]=0x30;UC_PWDUser_A[2]=0x30;UC_PWDUser_A[3]=0x30;
			UC_PWDUser_A[4]=0;UC_PWDUser_A[5]=0;UC_PWDUser_A[6]=0;UC_PWDUser_A[7]=0;
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x04;//leagth
			UC_URART2_TxDataArray_A[4]=0x00;
			CreateCRC_TX(5); //返回数据
			
			UC_DelayWriteFlash_S=1;//延时保存数据
			break;
		case ModifyLot: //0x24 修改批次
			UC_ModeRun_SF=UI_URT2_REC_D[4];// mde
	
			ValidationData(); //验证数据
			
			//切换模式进行参数处理
			if(UC_ModeRun_SF!=UC_ModeRunTemp_SF) 
			{
				Toggle_ModeRun();
				UC_ModeRunTemp_SF=UC_ModeRun_SF;   //运行模式
			}
			
			if(UC_DataException_S==0) //数据正常
			{
				UC_ReceivedData_4A[0]=UI_URT2_REC_D[5];
				UC_ReceivedData_4A[1]=UI_URT2_REC_D[6];
				UC_ReceivedData_4A[2]=UI_URT2_REC_D[7];
				UC_ReceivedData_4A[3]=UI_URT2_REC_D[8];
				LI_LotVolume_DF=ConvertToManualFlow(UC_ReceivedData_4A); //Lot Volume 批次容积
				
				UC_LotVolumeUnit_SF=UI_URT2_REC_D[9];  //Lot Volume l L 2ml 批次单位
				
				UC_ReceivedData_2A[0]=UI_URT2_REC_D[10];
				UC_ReceivedData_2A[1]=UI_URT2_REC_D[11];
				tempTime=ConvertToManualFlow16(UC_ReceivedData_2A);
				UC_AddTimeSec_DF=tempTime*60+UI_URT2_REC_D[12]; //Lot Volume Time   批次运行时间
				
				UC_BatchManual_SF=UI_URT2_REC_D[13]; //批次手动 
				
				//剩余流量为0 才能计算 应该就是第一次
				if(LI_ResidualFlow_D==0)
				{
					//清除数据状态
					LI_ResidualFlow_D=0;
					UC_IsPulseLaunch_S=0;
					UC_PulseTrigger_S=0;
					UI_CountSeconds_D=0;
					
					if(UC_LotVolumeUnit_SF==U_L)
					{
						LI_ResidualFlow_D=LI_LotVolume_DF*1000;
					}
					else if(UC_LotVolumeUnit_SF==U_ML)
					{
						LI_ResidualFlow_D=LI_LotVolume_DF;
					}
					//更新临时数据
					LI_LotVolumeTemp_DF=LI_LotVolume_DF;
					UC_LotVolumeUnitTemp_SF=UC_LotVolumeUnit_SF;
					UC_AddTimeSecTemp_DF=UC_AddTimeSec_DF;
				}
				else if(LI_LotVolumeTemp_DF!=LI_LotVolume_DF||UC_LotVolumeUnitTemp_SF!=UC_LotVolumeUnit_SF||UC_AddTimeSecTemp_DF!=UC_AddTimeSec_DF) //数据出现变动了
				{
					//清除数据状态
					LI_ResidualFlow_D=0;
					UC_IsPulseLaunch_S=0;
					UC_PulseTrigger_S=0;
					UI_CountSeconds_D=0;
					if(UC_LotVolumeUnit_SF==U_L)
					{
						LI_ResidualFlow_D=LI_LotVolume_DF*1000;
					}
					else if(UC_LotVolumeUnit_SF==U_ML)
					{
						LI_ResidualFlow_D=LI_LotVolume_DF;
					}
					//更新临时数据
					LI_LotVolumeTemp_DF=LI_LotVolume_DF;
					UC_LotVolumeUnitTemp_SF=UC_LotVolumeUnit_SF;
					UC_AddTimeSecTemp_DF=UC_AddTimeSec_DF;
				}
			}
			//--------------------TX--------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x0F;//leagth
			if(UC_DataException_S==0)//
			{
				UC_URART2_TxDataArray_A[4]=0x00;
				UC_DelayWriteFlash_S=1;//延时保存数据
			}
			else 
			{
				UC_URART2_TxDataArray_A[4]=UC_ModeRun_SF;
			}
			UC_URART2_TxDataArray_A[5]=(unsigned char)((LI_LotVolumeVerify_D>>24)&0xFF);//
			UC_URART2_TxDataArray_A[6]=(unsigned char)((LI_LotVolumeVerify_D>>16)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)((LI_LotVolumeVerify_D>>8)&0xFF);
			UC_URART2_TxDataArray_A[8]=(unsigned char)(LI_LotVolumeVerify_D&0xFF);
			UC_URART2_TxDataArray_A[9]=UC_LotVolumeUnitVerify_S; //
			
			ConvertToMinutesAndSeconds(UC_AddTimeSecVerifyMax_D,&minutes, &seconds);//
			UC_URART2_TxDataArray_A[10]=(unsigned char)((minutes>>8)&0xFF);
			UC_URART2_TxDataArray_A[11]=(unsigned char)(minutes&0xFF);
			UC_URART2_TxDataArray_A[12]=seconds;
			
			ConvertToMinutesAndSeconds(UC_AddTimeSecVerifyMin_D,&minutes, &seconds);//
			UC_URART2_TxDataArray_A[13]=(unsigned char)((minutes>>8)&0xFF);
			UC_URART2_TxDataArray_A[14]=(unsigned char)(minutes&0xFF);
			UC_URART2_TxDataArray_A[15]=seconds;
			CreateCRC_TX(16); //返回数据
			
			break;
			
		case ModifyLoop: //0x25 修改循环投加
			UC_ModeRun_SF=UI_URT2_REC_D[4];// mde
	
			ValidationData(); //验证数据
			
			//切换模式进行参数处理
			if(UC_ModeRun_SF!=UC_ModeRunTemp_SF) 
			{
				Toggle_ModeRun();
				UC_ModeRunTemp_SF=UC_ModeRun_SF;   //运行模式
			}
		
			if(UC_DataException_S==0)   //数据正常
			{
				UC_ReceivedData_4A[0]=UI_URT2_REC_D[5];
				UC_ReceivedData_4A[1]=UI_URT2_REC_D[6];
				UC_ReceivedData_4A[2]=UI_URT2_REC_D[7];
				UC_ReceivedData_4A[3]=UI_URT2_REC_D[8];
				LI_LoopLotVolume_DF=ConvertToManualFlow(UC_ReceivedData_4A); //Loop Lot Volume  循环投加 容积
				
				UC_LoopLotVolumeUnit_SF=UI_URT2_REC_D[9];  //Loop Lot Volume lL 2ml  循环投加容积单位
				
				
				UC_ReceivedData_2A[0]=UI_URT2_REC_D[10];
				UC_ReceivedData_2A[1]=UI_URT2_REC_D[11];
				tempTime=ConvertToManualFlow16(UC_ReceivedData_2A);
				UC_LoopADDTime_DF=tempTime*60+UI_URT2_REC_D[12]; //Loop Lot Volume Time S   循环投加添加时长
				
				
				UC_ReceivedData_2A[0]=UI_URT2_REC_D[13];
				UC_ReceivedData_2A[1]=UI_URT2_REC_D[14];
				tempTime=ConvertToManualFlow16(UC_ReceivedData_2A);
				UC_LoopCycleTime_DF=tempTime*60+UI_URT2_REC_D[15]; //Loop Cycle Volume Time S  循环投周期时长
				
				UC_ReceivedData_2A[0]=UI_URT2_REC_D[16];
				UC_ReceivedData_2A[1]=UI_URT2_REC_D[17];
				tempTime=ConvertToManualFlow16(UC_ReceivedData_2A);
				UC_LoopStartDelayTime_DF=tempTime*60+UI_URT2_REC_D[18]; //Loop StartDelay Volume Time S  循环投启动时长
				
				//循环投加剩余流量为0 才能计算 应该就是第一次
				if(LI_LoopResidualFlow_D==0)
				{
					//清除数据状态
					UC_IsDelayedLaunch_S=0;
					UC_IsDelayedLaunch_C=0;//延时计数
					LI_LoopResidualCycleTime_D=0; //运行总周期
					UC_LaunchTime_C=0;//泵运行时间
					//剩余时间=周期时间+延时时间
					LI_LoopResidualCycleTime_D=UC_LoopCycleTime_DF+UC_LoopStartDelayTime_DF;
					
					if(UC_LoopLotVolumeUnit_SF==U_L)
					{
						LI_LoopResidualFlow_D=LI_LoopLotVolume_DF*1000;
					}
					else if(UC_LoopLotVolumeUnit_SF==U_ML)
					{
						LI_LoopResidualFlow_D=LI_LoopLotVolume_DF;
					}
					//循环投加临时变量
					LI_LoopLotVolumeTemp_DF=LI_LoopLotVolume_DF;
					UC_LoopLotVolumeUnitTemp_SF=UC_LoopLotVolumeUnit_SF;
					UC_LoopADDTimeTemp_DF=UC_LoopADDTime_DF;
					UC_LoopCycleTimeTemp_DF=UC_LoopCycleTime_DF;
					UC_LoopStartDelayTimeTemp_DF=UC_LoopStartDelayTime_DF;
				}
				else if(LI_LoopLotVolumeTemp_DF!=LI_LoopLotVolume_DF||UC_LoopLotVolumeUnitTemp_SF!=UC_LoopLotVolumeUnit_SF||UC_LoopADDTimeTemp_DF!=UC_LoopADDTime_DF||UC_LoopCycleTimeTemp_DF!=UC_LoopCycleTime_DF||UC_LoopStartDelayTime_DF!=UC_LoopStartDelayTimeTemp_DF)
				{
					//清除数据状态
					UC_IsDelayedLaunch_S=0;
					UC_IsDelayedLaunch_C=0;//延时计数
					LI_LoopResidualCycleTime_D=0; //运行总周期
					UC_LaunchTime_C=0;//泵运行时间
					//剩余时间=周期时间+延时时间
					LI_LoopResidualCycleTime_D=UC_LoopCycleTime_DF+UC_LoopStartDelayTime_DF;
					
					if(UC_LoopLotVolumeUnit_SF==U_L)
					{
						LI_LoopResidualFlow_D=LI_LoopLotVolume_DF*1000;
					}
					else if(UC_LoopLotVolumeUnit_SF==U_ML)
					{
						LI_LoopResidualFlow_D=LI_LoopLotVolume_DF;
					}
					//循环投加临时变量
					LI_LoopLotVolumeTemp_DF=LI_LoopLotVolume_DF;
					UC_LoopLotVolumeUnitTemp_SF=UC_LoopLotVolumeUnit_SF;
					UC_LoopADDTimeTemp_DF=UC_LoopADDTime_DF;
					UC_LoopCycleTimeTemp_DF=UC_LoopCycleTime_DF;
					UC_LoopStartDelayTimeTemp_DF=UC_LoopStartDelayTime_DF;
				}
			}
			
			//--------------------TX------------------------------------------------------------------------
			TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x0F;//leagth
			if(UC_DataException_S==0)//
			{
				UC_URART2_TxDataArray_A[4]=0x00;
				UC_DelayWriteFlash_S=1;//延时保存数据
			}
			else 
			{
				UC_URART2_TxDataArray_A[4]=UC_ModeRun_SF;
			}
			UC_URART2_TxDataArray_A[5]=(unsigned char)((LI_LotVolumeVerify_D>>24)&0xFF);//
			UC_URART2_TxDataArray_A[6]=(unsigned char)((LI_LotVolumeVerify_D>>16)&0xFF);
			UC_URART2_TxDataArray_A[7]=(unsigned char)((LI_LotVolumeVerify_D>>8)&0xFF);
			UC_URART2_TxDataArray_A[8]=(unsigned char)(LI_LotVolumeVerify_D&0xFF);
			UC_URART2_TxDataArray_A[9]=UC_LotVolumeUnitVerify_S;
			
			ConvertToMinutesAndSeconds(UC_AddTimeSecVerifyMax_D,&minutes, &seconds);//
			UC_URART2_TxDataArray_A[10]=(unsigned char)((minutes>>8)&0xFF);
			UC_URART2_TxDataArray_A[11]=(unsigned char)(minutes&0xFF);
			UC_URART2_TxDataArray_A[12]=seconds;
			
			ConvertToMinutesAndSeconds(UC_AddTimeSecVerifyMin_D,&minutes, &seconds);//
			UC_URART2_TxDataArray_A[13]=(unsigned char)((minutes>>8)&0xFF);
			UC_URART2_TxDataArray_A[14]=(unsigned char)(minutes&0xFF);
			UC_URART2_TxDataArray_A[15]=seconds;
			CreateCRC_TX(16); //返回数据
			break;
		default:
		   	  
			  break;
		}
}


/************************************************
* Function Name: TxDataHeader  
* Description  : 
* Arguments    : None
* Return Value : None
*************************************************/
void TxDataHeader(void)
{
	UC_URART2_TxDataArray_A[0]=UI_URT2_REC_D[0];
	UC_URART2_TxDataArray_A[1]=UI_URT2_REC_D[1];
	UC_URART2_TxDataArray_A[3]=UI_URT2_REC_D[3];
}


/************************************************
* Function Name: TxDataHeader  
* Description  : 
* Arguments    : None
* Return Value : None
*************************************************/
void CreateCRC_TX(unsigned int arraySize)
{
	puch=&(UC_URART2_TxDataArray_A[0]);
	crc_r=crc16(puch,arraySize);
	crc_H=(crc_r>>8)&0xff;
	crc_L=crc_r&0xff;
	
	UC_URART2_TxDataArray_A[arraySize]=crc_L;
	UC_URART2_TxDataArray_A[arraySize+1]=crc_H;

	trans_data(UC_URART2_TxDataArray_A,(arraySize+2));
	
	//memset(UC_URART3_RevDataArray_A,0x00,256);//
	//clearArray
	/*
	for (i = 0; i < 100; i++) 
	{
    		UC_URART2_TxDataArray_A[i] = 0;
	}*/

}

/************************************************
* Function Name: ConvertToManualFlow  
* Description  : Converts 4 bytes of received data into an unsigned long int.
* Arguments    : receivedData - An array of 4 bytes representing the data to be converted.
* Return Value : The converted unsigned long int value.
*************************************************/
unsigned long int ConvertToManualFlow(uint8_t receivedData[4])
{
    unsigned long int manualFlow = 0;
    
    manualFlow = ((unsigned long int)receivedData[0] << 24) |
                 ((unsigned long int)receivedData[1] << 16) |
                 ((unsigned long int)receivedData[2] << 8)  |
                 (unsigned long int)receivedData[3];
    
    return manualFlow;
}


/************************************************
* Function Name: ConvertToManualFlow16  
* Description  : Converts 2 bytes of received data into an int.
* Arguments    : receivedData - An array of 2 bytes representing the data to be converted.
* Return Value : The converted int value.
*************************************************/
unsigned int ConvertToManualFlow16(uint8_t receivedData[2])
{
    unsigned int calibrateVol = 0;
    
    calibrateVol = ((int)receivedData[0] << 8) |
                 (int)receivedData[1];
    
    return calibrateVol;
}



/************************************************
* Function Name: ConvertToMinutesAndSeconds
* Description  : Converts total seconds to minutes and remaining seconds.
* Arguments    : totalSeconds - The total seconds to be converted.
*                minutes - Pointer to an unsigned int where minutes will be stored.
*                seconds - Pointer to an unsigned int where remaining seconds will be stored.
* Return Value : None
*************************************************/
void ConvertToMinutesAndSeconds(unsigned int totalSeconds, unsigned int *minutes, unsigned int *seconds)
{
    *minutes = totalSeconds / 60;  // ????
    *seconds = totalSeconds % 60;  // ??????
}

/************************************************
* Function Name: GetSetupParameters  
* Description  : 
* Arguments    : None
* Return Value : None
*************************************************/
void GetSetupParameters(void)
{
	//unsigned long int tempTime = 0;
	
	UC_ModeRun_SF=UI_URT2_REC_D[4];// mde
	
	//ValidationData();
	
	//切换模式进行参数处理
	if(UC_ModeRun_SF!=UC_ModeRunTemp_SF) 
	{
		Toggle_ModeRun();
		UC_ModeRunTemp_SF=UC_ModeRun_SF;   //运行模式
	}
	
	UC_ReceivedData_4A[0]=UI_URT2_REC_D[5];
	UC_ReceivedData_4A[1]=UI_URT2_REC_D[6];
	UC_ReceivedData_4A[2]=UI_URT2_REC_D[7];
	UC_ReceivedData_4A[3]=UI_URT2_REC_D[8];
	LI_ManualFlow_L_DF=ConvertToManualFlow(UC_ReceivedData_4A); //Manual L  0.0025L min, 2.5ML  LI_ManualFlowML_H1_DF  手动流量设置
	//LI_ManualFlowML_H1_DF 

	UC_ReceivedData_4A[0]=UI_URT2_REC_D[9];
	UC_ReceivedData_4A[1]=UI_URT2_REC_D[10];
	UC_ReceivedData_4A[2]=UI_URT2_REC_D[11];
	UC_ReceivedData_4A[3]=UI_URT2_REC_D[12];
	LI_PulseFlow_ML_DF=ConvertToManualFlow(UC_ReceivedData_4A); //Pulse ml  0.0015ml min, LI_PulseFlow_ML_H4_DF  脉冲对应流量设置
	
	//UC_PulseMemory_SF=UI_URT2_REC_D[13]; //Flow memory  脉冲记忆（不要 改为批次手动）
	UC_BatchManual_SF=UI_URT2_REC_D[13]; //批次手动
	
	UC_04_20MAType_SF=UI_URT2_REC_D[14]; //0/4-20 ml type 0-20ma 类型
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[15];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[16];
	UC_04_20MA_I1_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //0/4-20 ml I1‘’ I1电流值
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[17];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[18];
	UC_04_20MA_Q1_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //0/4-20 ml Q1  Q1 流量值
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[19];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[20];
	UC_04_20MA_I2_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //0/4-20 ml I2  I2 电流值 
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[21];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[22];
	UC_04_20MA_Q2_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //0/4-20 ml Q2  Q2 流量值
	
	
	/*
	if(UC_LotData_S==0) //数据正常
	{
		UC_ReceivedData_4A[0]=UI_URT2_REC_D[23];
		UC_ReceivedData_4A[1]=UI_URT2_REC_D[24];
		UC_ReceivedData_4A[2]=UI_URT2_REC_D[25];
		UC_ReceivedData_4A[3]=UI_URT2_REC_D[26];
		LI_LotVolume_DF=ConvertToManualFlow(UC_ReceivedData_4A); //Lot Volume 批次容积
		
		UC_LotVolumeUnit_SF=UI_URT2_REC_D[27];  //Lot Volume l L 2ml 批次单位
		
		UC_ReceivedData_2A[0]=UI_URT2_REC_D[28];
		UC_ReceivedData_2A[1]=UI_URT2_REC_D[29];
		tempTime=ConvertToManualFlow16(UC_ReceivedData_2A);
		UC_AddTimeSec_DF=tempTime*60+UI_URT2_REC_D[30]; //Lot Volume Time   批次运行时间
		
		//剩余流量为0 才能计算 应该就是第一次
		if(LI_ResidualFlow_D==0)
		{
			//清除数据状态
			LI_ResidualFlow_D=0;
			UC_IsPulseLaunch_S=0;
			UC_PulseTrigger_S=0;
			UI_CountSeconds_D=0;
			
			if(UC_LotVolumeUnit_SF==U_L)
			{
				LI_ResidualFlow_D=LI_LotVolume_DF*1000;
			}
			else if(UC_LotVolumeUnit_SF==U_ML)
			{
				LI_ResidualFlow_D=LI_LotVolume_DF;
			}
			//更新临时数据
			LI_LotVolumeTemp_DF=LI_LotVolume_DF;
			UC_LotVolumeUnitTemp_SF=UC_LotVolumeUnit_SF;
			UC_AddTimeSecTemp_DF=UC_AddTimeSec_DF;
		}
		else if(LI_LotVolumeTemp_DF!=LI_LotVolume_DF||UC_LotVolumeUnitTemp_SF!=UC_LotVolumeUnit_SF||UC_AddTimeSecTemp_DF!=UC_AddTimeSec_DF) //数据出现变动了
		{
			//清除数据状态
			LI_ResidualFlow_D=0;
			UC_IsPulseLaunch_S=0;
			UC_PulseTrigger_S=0;
			UI_CountSeconds_D=0;
			if(UC_LotVolumeUnit_SF==U_L)
			{
				LI_ResidualFlow_D=LI_LotVolume_DF*1000;
			}
			else if(UC_LotVolumeUnit_SF==U_ML)
			{
				LI_ResidualFlow_D=LI_LotVolume_DF;
			}
			//更新临时数据
			LI_LotVolumeTemp_DF=LI_LotVolume_DF;
			UC_LotVolumeUnitTemp_SF=UC_LotVolumeUnit_SF;
			UC_AddTimeSecTemp_DF=UC_AddTimeSec_DF;
		}
	}
	
	if(UC_LoopData_S==0)   //数据正常
	{
		UC_ReceivedData_4A[0]=UI_URT2_REC_D[31];
		UC_ReceivedData_4A[1]=UI_URT2_REC_D[32];
		UC_ReceivedData_4A[2]=UI_URT2_REC_D[33];
		UC_ReceivedData_4A[3]=UI_URT2_REC_D[34];
		LI_LoopLotVolume_DF=ConvertToManualFlow(UC_ReceivedData_4A); //Loop Lot Volume  循环投加 容积
		
		UC_LoopLotVolumeUnit_SF=UI_URT2_REC_D[35];  //Loop Lot Volume lL 2ml  循环投加容积单位
		
		
		UC_ReceivedData_2A[0]=UI_URT2_REC_D[36];
		UC_ReceivedData_2A[1]=UI_URT2_REC_D[37];
		tempTime=ConvertToManualFlow16(UC_ReceivedData_2A);
		UC_LoopADDTime_DF=tempTime*60+UI_URT2_REC_D[38]; //Loop Lot Volume Time S   循环投加添加时长
		
		
		UC_ReceivedData_2A[0]=UI_URT2_REC_D[39];
		UC_ReceivedData_2A[1]=UI_URT2_REC_D[40];
		tempTime=ConvertToManualFlow16(UC_ReceivedData_2A);
		UC_LoopCycleTime_DF=tempTime*60+UI_URT2_REC_D[41]; //Loop Cycle Volume Time S  循环投周期时长
		
		UC_ReceivedData_2A[0]=UI_URT2_REC_D[42];
		UC_ReceivedData_2A[1]=UI_URT2_REC_D[43];
		tempTime=ConvertToManualFlow16(UC_ReceivedData_2A);
		UC_LoopStartDelayTime_DF=tempTime*60+UI_URT2_REC_D[44]; //Loop StartDelay Volume Time S  循环投启动时长
		
		//循环投加剩余流量为0 才能计算 应该就是第一次
		if(LI_LoopResidualFlow_D==0)
		{
			//清除数据状态
			UC_IsDelayedLaunch_S=0;
			UC_IsDelayedLaunch_C=0;//延时计数
			LI_LoopResidualCycleTime_D=0; //运行总周期
			UC_LaunchTime_C=0;//泵运行时间
			
			if(UC_LoopLotVolumeUnit_SF==U_L)
			{
				LI_LoopResidualFlow_D=LI_LoopLotVolume_DF*1000;
			}
			else if(UC_LoopLotVolumeUnit_SF==U_ML)
			{
				LI_LoopResidualFlow_D=LI_LoopLotVolume_DF;
			}
			//循环投加临时变量
			LI_LoopLotVolumeTemp_DF=LI_LoopLotVolume_DF;
			UC_LoopLotVolumeUnitTemp_SF=UC_LoopLotVolumeUnit_SF;
			UC_LoopADDTimeTemp_DF=UC_LoopADDTime_DF;
			UC_LoopCycleTimeTemp_DF=UC_LoopCycleTime_DF;
			UC_LoopStartDelayTimeTemp_DF=UC_LoopStartDelayTime_DF;
		}
		else if(LI_LoopLotVolumeTemp_DF!=LI_LoopLotVolume_DF||UC_LoopLotVolumeUnitTemp_SF!=UC_LoopLotVolumeUnit_SF||UC_LoopADDTimeTemp_DF!=UC_LoopADDTime_DF||UC_LoopCycleTimeTemp_DF!=UC_LoopCycleTime_DF||UC_LoopStartDelayTime_DF!=UC_LoopStartDelayTimeTemp_DF)
		{
			//清除数据状态
			UC_IsDelayedLaunch_S=0;
			UC_IsDelayedLaunch_C=0;//延时计数
			LI_LoopResidualCycleTime_D=0; //运行总周期
			UC_LaunchTime_C=0;//泵运行时间
			
			if(UC_LoopLotVolumeUnit_SF==U_L)
			{
				LI_LoopResidualFlow_D=LI_LoopLotVolume_DF*1000;
			}
			else if(UC_LoopLotVolumeUnit_SF==U_ML)
			{
				LI_LoopResidualFlow_D=LI_LoopLotVolume_DF;
			}
			//循环投加临时变量
			LI_LoopLotVolumeTemp_DF=LI_LoopLotVolume_DF;
			UC_LoopLotVolumeUnitTemp_SF=UC_LoopLotVolumeUnit_SF;
			UC_LoopADDTimeTemp_DF=UC_LoopADDTime_DF;
			UC_LoopCycleTimeTemp_DF=UC_LoopCycleTime_DF;
			UC_LoopStartDelayTimeTemp_DF=UC_LoopStartDelayTime_DF;
		}
	}*/
	
	SetParamCalculate();
}


/************************************************
* Function Name: SetParamCalculate  
* Description  : 
* Arguments    : None
* Return Value : None
*************************************************/
void SetParamCalculate(void)
{
	if(UC_ModeRun_SF==MANUAL_MODE)
	{
		m_RunManual();
	}
	else if(UC_ModeRun_SF==PULSE_MODE)
	{
		m_RunPulse();
	}
	else if(UC_ModeRun_SF==MA_0_20_MODE)
	{
		m_Run04_20MA();
	}
	else if(UC_ModeRun_SF==BATCH_MODE)
	{
		m_RunLot();
	}
	else if(UC_ModeRun_SF==CYCLIC_MODE)
	{
		//The time from the first to the next cycle
		m_RunLoop();
	}
}


/************************************************
* Function Name: SetOtherParam  
* Description  : 
* Arguments    : None
* Return Value : None
*************************************************/
void SetOtherParam(void)
{
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[4];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[5];
	UI_BaudRateStatus_SF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_BaudRateStatus_SF
	if(UI_BaudRateStatus_SF==0){UI_BaudRateStatusIN_S=0;} //清除状态485 状态
	/*********************************485 设置不同频率在切换 同频率多次设置容易出问题 ***************************************************/
	if(UI_BaudRateStatusTemp_S!=UI_BaudRateStatus_SF)
	{
		UART1_Stop_T();
		UART1_Stop_R();
		UART1_R_Rest();
		UART1_CREATE(UI_BaudRateStatus_SF);
		UART1_Start_R();
	    	RS485_Direction=RS485_R; //切换到接收	
	}
	
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[6];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[7];
	UI_CalibParam_MA_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_CalibParam_MA_DF
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[8];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[9];
	UI_SlowModeStatus_SFTemp=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_SlowModeStatus_SF
	
	if(UI_SlowModeStatus_SF!=UI_SlowModeStatus_SFTemp)//修改后
	{
		UI_SlowModeStatus_SF=UI_SlowModeStatus_SFTemp;
		SlowModeMaxValue();
	}
	
	
	/******************************************压力液位**********************************************************/
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[10];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[11];
	UI_IsPessureSwitch_SF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_IsPessureSwitch_SF  压力开关
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[12];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[13];
	UI_PessureRange_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_IsPessureRange_DF
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[14];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[15];
	UI_PessureHigh_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_PessureHigh_DF
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[16];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[17];
	UI_PessureLow_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_PessureLow_DF
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[18];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[19];
	UI_IsLevelSwitch_SF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_IsLevelSwitch_SF; 液位开关
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[20];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[21];
	UI_LevelHigh_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_LevelHigh_DF;
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[22];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[23];
	UI_LevelLow_DF=ConvertToManualFlow16(UC_ReceivedData_2A); //UI_LevelLow_DF
	
	/******************************************时间更新**********************************************************/
			
	UI_RunRelayOutput_SF=UI_URT2_REC_D[24];//UI_RunRelayOutput_SF 运行继电器
	
	UI_RemoteControl_SF=UI_URT2_REC_D[25];//UI_RemoteControl_SF
	if(UI_RemoteControl_SF==REMOTE_CLOSE_S)
	{
		UC_RemoteStatusHL_S=REMOTE_NON_STOP;   //关闭远程开关 单次远程开机
	}
	
	UI_FaultyRelayOutput_SF=UI_URT2_REC_D[26];//UI_FaultyRelayOutput_SF  //故障继电器
	
	UI_MachineCode_DF=UI_URT2_REC_D[27];//UI_MachineCode_DF 机器码
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[28];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[29];
	UC_PowerOffLock_SF=ConvertToManualFlow16(UC_ReceivedData_2A); //0=关闭断电保护，1=开启断电保护
}

// 通过最大流量来限制手动流量的输入  以为重新标定后 最大流量小于之前设置的手动流量
void MaxFlowToManualFlow(void)
{
	// 最大流量小于之前设置的手动流量
	if(UI_MaxFlowML_H2_DF<(LI_ManualFlow_L_DF*10))
	{
		LI_ManualFlow_L_DF=UI_MaxFlowML_H2_DF/10;
	}
}

// 时间
void ConvertTime(unsigned long int seconds, unsigned int *ResidualMinutes, unsigned int *ResidualSeconds) 
{
    // ?????
    *ResidualMinutes = seconds / 60;
    // ???????
    *ResidualSeconds = seconds % 60;
}

//
/************************************************
* Function Name: SetParamCalculate  
* Description  : 设置激活功能和信号故障的返回bit位
* Arguments    : byte 参数  pos 位置 val 值
* Return Value : byte
*************************************************/
unsigned char setBit(unsigned char byte, int pos, int val)
{
    if (val == 1) {
        byte |= (1 << pos); // 将 pos 位置设为 1
    } else {
        byte &= ~(1 << pos); // 将 pos 位置设为 0
    }
    return byte;
}

/************************************************
* Function Name: DelayWriteFlash  
* Description  : 延时写Flash 因为写Flash 关中断，影响串口发数据
* Arguments    : byte 参数  pos 位置 val 值
* Return Value : byte
*************************************************/
void DelayWriteFlash(void)
{
    if(++UC_DelayWriteFlash_C>=3)
    {
	    UC_DelayWriteFlash_C=0;
	    UC_DelayWriteFlash_S=0;
	    UC_C_WriteFlash_S=TRUE;
    }
}

/************************************************
* Function Name: hexToDec  
* Description  : 数据格式转换 - 现用于时钟时间
* Arguments    : none
* Return Value : none
*************************************************/
 unsigned char hexToDec(unsigned char a)
 {
    char n=1;
    char k=16;
    unsigned char Dec=0;
    while (a!=0)
    {
    	Dec+=(a%10)*n;
	a=a/10;
	n=n*k;
    }
    return Dec;
 }
 
 /************************************************
* Function Name: ValidationData  
* Description  : 验证批次和循环投加数据的准确性
* Arguments    : none
* Return Value : none
*************************************************/
void  ValidationData(void)
{
	unsigned long int tempTimeVerify_S = 0;
	unsigned int Seconds_ML_H2_100,Seconds_ML_H2_1;
	
	/*
		200.000.00 MAX_ML_H2  /3600s
		每秒打出多少 ml 后两位 100%  UI_RatedMAXFlow_DF= 200.0L/H  /0.36
	*/
	Seconds_ML_H2_100=UI_RatedMAXFlow_DF/0.36;  //100%
	Seconds_ML_H2_1=UI_RatedMAXFlow_DF/36;  //1%
	
	//-----------------------------------数据-----------------------------------------------------
	
	UC_ReceivedData_4A[0]=UI_URT2_REC_D[5];
	UC_ReceivedData_4A[1]=UI_URT2_REC_D[6];
	UC_ReceivedData_4A[2]=UI_URT2_REC_D[7];
	UC_ReceivedData_4A[3]=UI_URT2_REC_D[8];
	LI_LotVolumeVerify_D=ConvertToManualFlow(UC_ReceivedData_4A); //Lot Volume 批次容积 验证数据
	
	UC_LotVolumeUnitVerify_S=UI_URT2_REC_D[9];  //Lot Volume l L 2ml 批次单位 验证数据
	
	UC_ReceivedData_2A[0]=UI_URT2_REC_D[10];
	UC_ReceivedData_2A[1]=UI_URT2_REC_D[11];
	tempTimeVerify_S=ConvertToManualFlow16(UC_ReceivedData_2A);
	UC_AddTimeSecVerify_D=tempTimeVerify_S*60+UI_URT2_REC_D[12]; //Lot Volume Time   批次运行时间  验证数据
	
	if(UC_LotVolumeUnitVerify_S==U_L)
	{
		LI_ResidualFlowVerify_D=LI_LotVolumeVerify_D*1000;
	}
	else if(UC_LotVolumeUnit_SF==U_ML)
	{
		LI_ResidualFlowVerify_D=LI_LotVolume_DF;
	}
	
	UC_AddTimeSecVerifyMin_D = LI_ResidualFlowVerify_D/Seconds_ML_H2_100; //最快时间
	UC_AddTimeSecVerifyMax_D = LI_ResidualFlowVerify_D/Seconds_ML_H2_1;  //最慢时间
	
	if(UC_AddTimeSecVerify_D>=UC_AddTimeSecVerifyMin_D&&UC_AddTimeSecVerify_D<=UC_AddTimeSecVerifyMax_D)
	{
		UC_DataException_S=0; //数据正常
		LI_LotVolumeVerify_D=0;
		UC_LotVolumeUnitVerify_S=0;
		UC_AddTimeSecVerifyMax_D=0;
		UC_AddTimeSecVerifyMin_D=0;
	}
	else
	{
		UC_DataException_S=1; //数据异常
	}
}

//设置慢速模式最大值
void SlowModeMaxValue(void)
{	
	//50% max
	if(UI_SlowModeStatus_SF==1)
	{
		UI_FactoryRatedMAXFlowSlowMode_DF=UI_FactoryRatedMAXFlow_DF/2;
	}
	else if(UI_SlowModeStatus_SF==2) //25% max
	{
		UI_FactoryRatedMAXFlowSlowMode_DF=UI_FactoryRatedMAXFlow_DF/4;
	}
	else //UI_SlowModeStatus_SF  off
	{
		UI_FactoryRatedMAXFlowSlowMode_DF=UI_FactoryRatedMAXFlow_DF;
	}
	
	UI_UserRatedMAXFlow_DF=UI_FactoryRatedMAXFlowSlowMode_DF;//赋值给我用户额定流
	UI_RatedMAXFlow_DF=UI_FactoryRatedMAXFlowSlowMode_DF;//基础计算额定流量
	/*********************计算数据数据*********************************************/
	UI_RatedFlow_Temp=UI_RatedMAXFlow_DF;//设置的额定流量
	UI_RatedFlow_Temp=UI_RatedFlow_Temp*1000000;//为了精度扩大2000*1000000= 200.000.0000 L
	
	//UI_MaxFlowML_H2_Temp=UI_MaxFlowML_H2_DF/100; //减小精度到250.000 L
	
	//额定功率占最大功率的百分比
	//UI_RatedPercent_D=UI_RatedFlow_Temp/UI_MaxFlowML_H2_Temp;//得到额定功率是最大功率的百分之多少 比如是9523 就是95.23%
	UI_RatedPercent_D=8300; //默认百分之83 经验数据 200/240
	
	//通过额定功率获取额定最大脉宽
	if(UI_SlowModeStatus_SF==1)
	{
		LI_RatedMAXPulseWidth_DF=UI_RatedPercent_D*MAX_PMW_US_50;
	}
	else if(UI_SlowModeStatus_SF==2) //25% max
	{
		LI_RatedMAXPulseWidth_DF=UI_RatedPercent_D*MAX_PMW_US_25;
	}
	else 
	{
		LI_RatedMAXPulseWidth_DF=UI_RatedPercent_D*MAX_PMW_US;
	}
	//LI_RatedMAXPulseWidth_DF=UI_RatedPercent_D*MAX_PMW_US;
	LI_RatedMAXPulseWidth_DF=LI_RatedMAXPulseWidth_DF/10000;
	
	KS_PWMPULSE=LI_RatedMAXPulseWidth_DF;//快吸慢推最大转速
	
	//修改手动最大流量
	LI_ManualFlow_L_DF=UI_FactoryRatedMAXFlowSlowMode_DF;
	LI_ManualFlow_L_DF=LI_ManualFlow_L_DF*1000;
	
	UC_DelayWriteFlash_S=1;//延时保存数据
}


