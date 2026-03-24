/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G13 V2.03.04.01 [11 Nov 2016]
* Device(s)    : R5F100FE
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2025-04-02
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
/* Start user code for include. Do not edit comment generated here */
#include "get_adc.h"
#include "flash.h"
#include "var_define.h"	
#include "UART_SET.h"
#include "r_cg_serial.h"
#include "string.h"
#include "get_screen.h"
#include "m_manual_1.h"
#include "m_04_20ma_3.h"
#include "m_lot_4.h"
#include "m_loop_5.h"
#include "m_pulse_2.h"
#include "b_cali.h"
#include "set_other.h"
#include "i2c.h"
#include "b_basic.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/*******************CLOCK1302 时钟芯片**************************/
unsigned char ACC;
unsigned char ACC_Temp;
unsigned char ACC_7;

/************common*************/
unsigned char UC_C_WriteFlash_S;
unsigned char UC_C_OnOff_S=FALSE;
unsigned char UC_C_OnOffStatus_S=FALSE;
unsigned char UC_D_SAVE_S=0;
unsigned int  UI_C_Time_Delay=150;
unsigned char UC_C_Standby_S=FALSE;
unsigned char UC_C_MotorStart_S=FALSE;
unsigned long int LI_TargetFlowML_H2_DF;
unsigned long int ULI_C_PulseCallBack_C; //机电反馈脉冲计数

/************Flash*************/
extern unsigned char UC_F_FlashData_A[188];
extern unsigned char UC_F_FlashDataSize_D;
unsigned char UC_D_FlashCount;
unsigned long int ULI_D_FlashTotal;
unsigned char UC_D_FlashTemp;
unsigned char UC_C_Read_eeprom_Data[150]={0x00,};  //I2C 存储数组
unsigned long int ULI_D_FlashTotalCRC; //CRC 校验和

/************Set Param*************/
unsigned char UC_ModeRun_SF;
unsigned char UC_ModeRunTemp_SF;//用于切换模式的临时变量
extern unsigned long int LI_ManualFlow_L_DF;
extern unsigned long int  LI_PulseFlow_ML_DF;
extern unsigned char  UC_PulseMemory_SF; //脉冲记忆不要
extern unsigned char  UC_BatchManual_SF; //批次手动
extern unsigned char  UC_04_20MAType_SF;
extern unsigned int  UC_04_20MA_I1_DF;
extern unsigned int  UC_04_20MA_Q1_DF;
extern unsigned int  UC_04_20MA_I2_DF;
extern unsigned int  UC_04_20MA_Q2_DF;

extern unsigned long int LI_LotVolume_DF;
extern unsigned char UC_LotVolumeUnit_SF;
extern unsigned long int UC_AddTimeSec_DF;
unsigned long int LI_LotVolumeTemp_DF; //批次容积的临时变量
unsigned char UC_LotVolumeUnitTemp_SF; //批次容积的临时变量单位
unsigned long int UC_AddTimeSecTemp_DF; //批次容积时间

extern unsigned long int LI_LoopLotVolume_DF;
extern unsigned char UC_LoopLotVolumeUnit_SF;
extern unsigned long int UC_LoopADDTime_DF;
extern unsigned long int UC_LoopCycleTime_DF;
extern unsigned long int UC_LoopStartDelayTime_DF;

unsigned long int LI_LoopLotVolumeTemp_DF; //循环投机容积的临时临时变量
unsigned char UC_LoopLotVolumeUnitTemp_SF; //循环投机容积临时临时变量单位
unsigned long int UC_LoopADDTimeTemp_DF; //循环投机添加时间临时变量
unsigned long int UC_LoopCycleTimeTemp_DF;//循环投机周期时间临时变量
unsigned long int UC_LoopStartDelayTimeTemp_DF;//循环投机开始延时时间临时变量

extern unsigned long int UI_CalibrateVolML_H1_DF; //厂家标定容积
extern unsigned long int UI_UserCalibrateVolML_H1_DF;  //用户标定容积
extern unsigned char UC_IsCalibrated_DF;
extern unsigned int UI_RatedMAXFlow_DF; //额定最大流量
extern unsigned int UI_FactoryRatedMAXFlow_DF; //厂家额定最大流量
extern unsigned int UI_UserRatedMAXFlow_DF; //厂家额定最大流量
extern unsigned long int LI_RatedMAXPulseWidth_DF; //额定脉宽根据额定流量计算得出
extern unsigned long int LI_LoopResidualFlow_D;//循环投加剩余流量
extern unsigned char UC_IsDelayedLaunch_S; //循环投机延时运行状态
extern unsigned int UC_IsDelayedLaunch_C; //循环投机延时运行时间
extern unsigned long int LI_LoopResidualCycleTime_D;
extern unsigned int UC_LaunchTime_C;//循环投机泵运行时间
extern unsigned long int LI_LoopResidualFlow_D;//循环投机剩余流量

unsigned char UC_SetPageg_S;//进设置界面状态
unsigned char UC_PWDUser_A[8];//用户密码
unsigned char UC_PWDFactory_A[8];//工厂密码
unsigned char UC_DeviceId_A[8];//设备ID

/************Other Param*************/
unsigned char UC_RemoteMode_SF;
unsigned char UC_RuningCal_Delay_C;
extern unsigned int readSingleData; //接受数据
extern unsigned int address; //接收地址



/************extern PARAM*************/
extern unsigned char UC_SwitchCali_S;
extern unsigned char UC_StartZeroing_S;
extern unsigned long int UI_MaxFlowML_H2_DF;

extern unsigned int UI_C_SetMinAdValue4_DF;
extern unsigned int UI_C_SetMaxAdValue20_DF;
extern unsigned int UI_4_20MA_AD_K_DF;
extern unsigned int UI_4_20MA_AD_B_DF;
extern unsigned int UI_AddSub_B_DF;
extern unsigned int UI_Suc_Vel_Percent; //60%
extern unsigned int UI_Percent_H2_DF;
extern unsigned long int LI_ManualFlow_L_RE_Temp;
extern unsigned long int TDR07_Percent_Temp; //当前运行得脉宽
extern unsigned char UC_ChangKSModel_S;//快吸慢推模式
extern unsigned char UC_4_20MAAlarmS_S; //4-20ma 报警状态
extern unsigned char UI_BaudRateStatusIN_S;//进行485读写
unsigned char UI_BaudRateStatusIN_C;//进行485读写计数清除
extern unsigned char UI_C_GDAlarm_S; //光电报警状态
extern unsigned int TDR07_Percent_C; //当前脉宽
extern unsigned int AtPresentPluse;//当前脉冲
extern unsigned char UC_DiaphragmDelay_S;//隔膜报警运行

/************URART*************/
extern unsigned char UC_URART2_RevFlag_S;
unsigned char SendKind;
unsigned char SendReady=0x55;
extern  uint8_t UI_URT1_SEND_D[];
unsigned int UI_TargetFlowL_H1_485_VD; //485 返回目标流量展示
extern unsigned char UC_S_Para_Invalid_D; //验证数据异常
extern unsigned int UI_C_MA_H1_D;
extern unsigned long int LI_ResidualFlow_ViewD; //剩余批量容积界面显示
extern unsigned long int LI_LoopResidualFlow_ViewD;
extern unsigned char UC_URART2_TxDataArray_A[100];


/**************AD***************/
unsigned int UI_AD_4_20MA_Delay_C;

/**************other Param***************/
extern unsigned int UI_BaudRateStatus_SF;
extern unsigned int UI_CalibParam_MA_DF;
extern unsigned int UI_SlowModeStatus_SF;
extern unsigned int UI_IsPessureSwitch_SF; //压力开关
extern unsigned int UI_IsLevelSwitch_SF;  //液位开关
extern unsigned char UC_PowerOffLock_SF;  // 0=关闭断电保护，1=开启断电保护 (需存入EEPROM)
extern unsigned int UI_PessureRange_DF;
extern unsigned int UI_PessureHigh_DF;
extern unsigned int UI_PessureLow_DF;
extern unsigned int UI_LevelRange_DF;
extern unsigned int UI_LevelHigh_DF;
extern unsigned int UI_LevelLow_DF;
extern unsigned char UI_Year_DF;
extern unsigned char UI_Month_DF;
extern unsigned char UI_Day_DF;
extern unsigned char UI_Hour_DF;
extern unsigned char UI_Minute_DF;
extern unsigned char UI_RemoteControl_SF;
extern unsigned char UI_RunRelayOutput_SF;
extern unsigned char UI_FaultyRelayOutput_SF;
extern unsigned char UI_ScreenSaver_SF;
extern unsigned int UI_ScreenSaverTime_DF;
extern unsigned char UI_MachineCode_DF;
extern unsigned char UI_LanguageSet_SF;
extern unsigned int UI_LockScreenTime_DF;
extern unsigned char UC_C_OutPutMA_Count;
extern unsigned char UC_RemoteStatusHL_S;
extern unsigned char UC_GetPessureDelay_C; //获取延时压力
extern unsigned long int LI_ResidualFlow_D; //剩余批量容积
extern unsigned int UI_CountSeconds_D; //批次剩余时间
extern unsigned char UC_IsPulseLaunch_S; //批次是否已经启动
extern unsigned char UC_SignalFault_D;//单独报警状态
extern unsigned char UC_ActiveMode_S;//激活状态

/**********************累计流量******************************/
extern unsigned int UI_TotalFlowL_H1_DF;//累计流量L小数点后一位
extern unsigned int UI_FlowInM3_DF;//累计M3

/**********************报警状态******************************/
extern unsigned char UC_LevelKTStatusHL_S;//k空桶报警状态
extern unsigned char UC_PressureLevelAlarm_S; //压力液位报警状态 模式0x55 正常抓状态
extern unsigned char UC_C_PumpAlarm_S;//泵的反馈异常报警
extern unsigned int UC_DiaphragmAlarm_S;//隔膜报警状态
extern unsigned char UC_LevelKTHL_CT;//空桶异常计数

/**********************上电延时******************************/
extern unsigned char UC_ProwOnDelay_D; //上电延时
extern unsigned char UC_ProwOnDelay_S; //上电延时状态
unsigned char UC_ServerDelay_C; //开启服务延时

unsigned int KS_PWMPULSE; //进入快吸慢推的脉宽
extern unsigned int UI_PulseWidth_D;//当前的实时脉宽

extern unsigned char UC_Time5_S; //定时器5 运行状态

/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    while (1U)
    {	 
	R_WDT_Restart();
	
	//设备运行
	ComControl();
	
	//send Data for urt1 发送数据
	SendData_485URT1();
	
	//获取4-20MA 和输出4-20MA  获取压力报警 AD
	Get_Data();
	
	//on-screen communication
	ISendDataToBoard();
	
	//update time 更新时钟数据  ---setOther
	UpdateTime();
	
	//增加累计流量
	ComputeTotalFlow();
	
	//调用Time5 定时器方法
	if(UC_Time5_S==1)
	{
		T_Channel5_Function();
		UC_Time5_S=0;
	}
	
	if(UC_C_WriteFlash_S==TRUE)
	{

		FlashSetPar(); 
		DI();
		/*******************write flash******************/
		PFDL_Init();
		PFDL_Erase();
		PFDL_Write();
		F_Close();
		
		/********************write 24c02*******问AI的合理性能*********bug掉电记忆没有处理 I2C *
		if(UC_D_SAVE_S==1)
		{

		}
		else
		{
			//Check_eeproom();
		}*/
		
		EI();
		UC_C_WriteFlash_S=FALSE;
	}
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    //PID_Init();
    //if
    	InitPoweringOn();//设备上电中  bug 测试后打开
    //else
    	//EI();
    	//R_TAU0_Channel5_Start();//use 1s
    
    /*
    while(UI_C_Time_Delay>0)
    {
    	R_WDT_Restart();
    }*/
    R_TAU0_Channel0_Start();
    R_TAU0_Channel2_Start();//use js
    R_TAU0_Channel3_Start();//use pulse
    R_TAU0_Channel4_Start();// pwm out
    R_TAU0_Channel6_Start();//use 1ms AD sample
        
    R_INTC1_Start();//光电中断
    R_INTC2_Start(); //电机反馈霍尔
    
    INIT_UART2();//Screen  UART2 屏幕串口
    UART2_Stop_T();
    UART2_Start_R();
    
    R_ADC_Set_OperationOn();//Enabling AD adoption
    
    UART1_CREATE(UI_BaudRateStatus_SF);  //485  初始波特 0.9600 1. 19200 2.38400
    INIT_UART1();
    UART1_Start_R();
    RS485_Direction=RS485_R; //切换到接收
    
    DI();
    PFDL_Init();
    //PFDL_Erase();
    PFDL_Read();
    F_Close();
    FlashGetPar();
    
    EI();
    
    Initial(); //初始时钟芯片 
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */


/*****************************************************************
* Function Name: InitPoweringOn
* Description  : Device Powering On
* Arguments    : None 设备上电中
* Return Value : None
******************************************************************/
void InitPoweringOn(void)
{
	  EI();
	  R_TAU0_Channel5_Start();//use 1s
	  
	  //UC_ServerDelay_C=1;
    	  //while(UC_ServerDelay_C>0){R_WDT_Restart();}
	  while(UC_ProwOnDelay_S==0) //定时器延时了 25s
	  {
		  R_WDT_Restart(); //喂狗
	  }
}


/*****************************************************************
* Function Name: ComControl
* Description  : Control Machine
* Arguments    : None
* Return Value : None
******************************************************************/
void ComControl(void)
{
	R_WDT_Restart();
	
	if(UC_C_OnOff_S==TRUE&&UC_C_OnOffStatus_S==TRUE) //正常开机
	{
		Com_Run();
	}
	else if(UC_C_OnOff_S==TRUE&&UC_C_OnOffStatus_S==WAIT) //暂停
	{
		Com_WAIT();
	}
	else if(UC_SwitchCali_S==TRUE||UC_StartZeroing_S==TRUE)//标定和归零开机运行
	{
		Com_Run();
	}
	else //关机
	{
		Com_OFF();
	}
	
	//机头运行设置脉宽-->条件 1机头启动 2 非标定状态 3 非归零状态 4 非快吸慢推状态 5增加延时设置
	if(UC_SwitchCali_S==FALSE&&UC_StartZeroing_S==FALSE&&UC_RuningCal_Delay_C>=RUNINGCAL_PERIOD)
	{
		RunCal();
		UC_RuningCal_Delay_C=0;
	}
}


/*****************************************
* Function Name: Com_Run  开机
******************************************/
void Com_Run(void)
{
	//正常工作无掉电 远程无关机
	if(UC_D_SAVE_S==0&&UC_RemoteStatusHL_S==REMOTE_NON_STOP)
	{
		ON_OFF=OPEN; //启动电机
		EnableRunRelayOutput(); //判断是否开启继电器
	}
	else
	{
		Com_OFF();//掉电关机
	}
}

/*****************************************
* Function Name: Com_WAIT  暂停
******************************************/
void Com_WAIT(void)
{
	ON_OFF=CLOSE;//关闭电机
	SYS_PWR_OUTPUT=OFF_L;//关闭继电器
	
}

/*****************************************
* Function Name: Com_OFF  关机
******************************************/
void Com_OFF(void)
{
	ON_OFF=CLOSE;//关闭电机
	SYS_PWR_OUTPUT=OFF_L;//关闭继电器
	//getPWMForPercent(0);//百分比0 PWM脉宽为0
}

/*****************************************
* Function Name: ChangeSlowMode 进入快吸满退模式
******************************************/
void ChangeSlowMode(void)
{
	//设置快吸慢推  设备进入运行状态  光电状态正常
	//if(UI_SlowModeStatus_SF==OPEN&&UC_C_OnOffStatus_S==TRUE&&UI_C_GDAlarm_S!=GD_ERROR)
	
	//设备进入运行状态  光电状态正常 脉宽小于25000 数据才准确才有意义
	if(UI_C_GDAlarm_S!=GD_ERROR&&TDR07_Percent_C<=18000&&AtPresentPluse>0)
	{
		UC_ChangKSModel_S=1; //进入快吸慢推模式
	}
	else
	{
		UC_ChangKSModel_S=0; //设备停止退出快吸慢推状态
	}
	
}


/*****************************************************************
* Function Name: RunDevice
* Description  : All mode operation 
* Arguments    : None
* Return Value : None
******************************************************************/
void RunCal(void)
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
		m_RunLoop();
	}
}

//开机清除状态位
void ClearStatus(void)
{
	UC_ChangKSModel_S=0; //快吸慢推状态位 
	UC_LevelKTHL_CT=0;
	UC_LevelKTStatusHL_S=KT_NON_ALARM; //KT 不报警
	UC_PressureLevelAlarm_S=ALARM_Normal;  //压力报警取消
	UC_4_20MAAlarmS_S=MA4_20_NORMAL; //4-20ma 报警取消
}

//关机清除报警状态
void CLoseClearAlarmS(void)
{
	UC_LevelKTStatusHL_S=KT_NON_ALARM; //KT 不报警
	UC_PressureLevelAlarm_S=ALARM_Normal;  //压力报警取消
	UC_4_20MAAlarmS_S=MA4_20_NORMAL; //4-20ma 报警取消
	UI_C_GDAlarm_S=GD_NORMAL; //光电报警取消
}


/*****************************************************************
* Function Name: Get_Data
* Description  : Get Data
* Arguments    : 4-20ma 的输入和输出
* Return Value : None
******************************************************************/
void Get_Data(void)
{
	if(UC_ModeRun_SF==MA_0_20_MODE)
	{
	
		if(UI_AD_4_20MA_Delay_C>=ADC_4_20MA_PERIOD)
		{	
			ADGet4_20MA();
			UI_AD_4_20MA_Delay_C=0;
		}
	}
	else //非0-20mA 模式清除 无输入报警
	{
		UC_4_20MAAlarmS_S=MA4_20_NORMAL; //报警解除
	}
	
	if(UC_C_OutPutMA_Count>=OUTPUT_MA_PERIOD)	//The output updates 2S once  机器运行才能输出
   	 {
		 UC_C_OutPutMA_Count=0;
		 Get_OutPut();
   	 }
	 
	 //获取压力值 
	 if(UC_GetPessureDelay_C>=ADC_4_20MA_YL)	//The output updates 2S once
   	 {
		 ADGet_Pressure();  //压力床干起
		 UC_GetPessureDelay_C=0;
		 
		
   	 }
	 
	 //隔膜报警
	 if(UC_DiaphragmDelay_S==1)
	 {
		 CheckDiaphragmAlarm();
		 UC_DiaphragmDelay_S=0;
	 }
}


/*****************************************
* Function Name: SendDataToBoard  
* Description  : Communication between screen and motherboard
* Arguments    : None
* Return Value : None
******************************************/
void ISendDataToBoard(void)
{
	if(UC_URART2_RevFlag_S==TRUE)
	{
		UC_URART2_RevFlag_S=FALSE;
		SendDataToBoard();
	}
}

void FlashSetPar(void)
{
	UC_F_FlashData_A[0]=0x99;
	UC_F_FlashData_A[1]=0x88;
	
	UC_F_FlashData_A[4]=UC_ModeRun_SF;//UC_ModeRun_SF 当前模式
	UC_F_FlashData_A[5]=(unsigned char)((LI_ManualFlow_L_DF>>24)&0xFF); //LI_ManualFlow_L_DF 手动流量
	UC_F_FlashData_A[6]=(unsigned char)((LI_ManualFlow_L_DF>>16)&0xFF);
	UC_F_FlashData_A[7]=(unsigned char)((LI_ManualFlow_L_DF>>8)&0xFF);
	UC_F_FlashData_A[8]=(unsigned char)(LI_ManualFlow_L_DF&0xFF);
	UC_F_FlashData_A[9]=(unsigned char)((LI_PulseFlow_ML_DF>>24)&0xFF);//LI_PulseFlow_ML_DF 脉冲流量一个脉冲对应多少流量
	UC_F_FlashData_A[10]=(unsigned char)((LI_PulseFlow_ML_DF>>16)&0xFF);
	UC_F_FlashData_A[11]=(unsigned char)((LI_PulseFlow_ML_DF>>8)&0xFF);
	UC_F_FlashData_A[12]=(unsigned char)(LI_PulseFlow_ML_DF&0xFF);

	//UC_F_FlashData_A[13]=UC_PulseMemory_SF; //UC_PulseMemory_SF 脉冲记忆 不用
	UC_F_FlashData_A[13]=UC_BatchManual_SF; //UC_BatchManual_SF 脉冲记忆改成 批次手动
	UC_F_FlashData_A[14]=UC_04_20MAType_SF;//UC_04_20MAType_SF 4-20MA 模式类型
	UC_F_FlashData_A[15]=(unsigned char)(( UC_04_20MA_I1_DF>>8)&0xFF); //UC_04_20MA_I1_DF
	UC_F_FlashData_A[16]=(unsigned char)(UC_04_20MA_I1_DF&0xFF);
	UC_F_FlashData_A[17]=(unsigned char)(( UC_04_20MA_Q1_DF>>8)&0xFF); //UC_04_20MA_Q1_DF;
	UC_F_FlashData_A[18]=(unsigned char)(UC_04_20MA_Q1_DF&0xFF);
	UC_F_FlashData_A[19]=(unsigned char)(( UC_04_20MA_I2_DF>>8)&0xFF); //UC_04_20MA_I2_DF;			
	UC_F_FlashData_A[20]=(unsigned char)(UC_04_20MA_I2_DF&0xFF);;
	UC_F_FlashData_A[21]=(unsigned char)((UC_04_20MA_Q2_DF>>8)&0xFF); //UC_04_20MA_Q2_DF;;
	UC_F_FlashData_A[22]=(unsigned char)(UC_04_20MA_Q2_DF&0xFF);
	UC_F_FlashData_A[23]=(unsigned char)((LI_LotVolume_DF>>24)&0xFF);//LI_LotVolume_DF
	UC_F_FlashData_A[24]=(unsigned char)((LI_LotVolume_DF>>16)&0xFF);
	UC_F_FlashData_A[25]=(unsigned char)((LI_LotVolume_DF>>8)&0xFF);
	UC_F_FlashData_A[26]=(unsigned char)(LI_LotVolume_DF&0xFF);
	UC_F_FlashData_A[27]=UC_LotVolumeUnit_SF; //UC_LotVolumeUnit_SF
	
	UC_F_FlashData_A[28]=(unsigned char)((UC_AddTimeSec_DF>>24)&0xFF);//UC_AddTimeSec_DF
	UC_F_FlashData_A[29]=(unsigned char)((UC_AddTimeSec_DF>>16)&0xFF);
	UC_F_FlashData_A[30]=(unsigned char)((UC_AddTimeSec_DF>>8)&0xFF);
	UC_F_FlashData_A[31]=(unsigned char)(UC_AddTimeSec_DF&0xFF);
	
	UC_F_FlashData_A[32]=(unsigned char)((LI_LoopLotVolume_DF>>24)&0xFF);//LI_LoopLotVolume_DF
	UC_F_FlashData_A[33]=(unsigned char)((LI_LoopLotVolume_DF>>16)&0xFF);
	UC_F_FlashData_A[34]=(unsigned char)((LI_LoopLotVolume_DF>>8)&0xFF);
	UC_F_FlashData_A[35]=(unsigned char)(LI_LoopLotVolume_DF&0xFF);
	UC_F_FlashData_A[36]=UC_LoopLotVolumeUnit_SF; //UC_LoopLotVolumeUnit_SF
	
	UC_F_FlashData_A[37]=(unsigned char)((UC_LoopADDTime_DF>>24)&0xFF);//UC_LoopADDTime_DF
	UC_F_FlashData_A[38]=(unsigned char)((UC_LoopADDTime_DF>>16)&0xFF);
	UC_F_FlashData_A[39]=(unsigned char)((UC_LoopADDTime_DF>>8)&0xFF);
	UC_F_FlashData_A[40]=(unsigned char)(UC_LoopADDTime_DF&0xFF);
	
	UC_F_FlashData_A[41]=(unsigned char)((UC_LoopCycleTime_DF>>24)&0xFF);//UC_LoopCycleTime_DF
	UC_F_FlashData_A[42]=(unsigned char)((UC_LoopCycleTime_DF>>16)&0xFF);
	UC_F_FlashData_A[43]=(unsigned char)((UC_LoopCycleTime_DF>>8)&0xFF);
	UC_F_FlashData_A[44]=(unsigned char)(UC_LoopCycleTime_DF&0xFF);
	
	UC_F_FlashData_A[45]=(unsigned char)((UC_LoopStartDelayTime_DF>>24)&0xFF);//UC_LoopStartDelayTime_DF
	UC_F_FlashData_A[46]=(unsigned char)((UC_LoopStartDelayTime_DF>>16)&0xFF);
	UC_F_FlashData_A[47]=(unsigned char)((UC_LoopStartDelayTime_DF>>8)&0xFF);
	UC_F_FlashData_A[48]=(unsigned char)(UC_LoopStartDelayTime_DF&0xFF);
	
	UC_F_FlashData_A[49]=UC_IsCalibrated_DF; //UC_IsCalibrated_DF
	
	UC_F_FlashData_A[50] = (unsigned char)((UI_CalibrateVolML_H1_DF >> 24) & 0xFF); // UI_CalibrateVolML_H1_DF 厂家标定流量（50-53）
	UC_F_FlashData_A[51] = (unsigned char)((UI_CalibrateVolML_H1_DF >> 16) & 0xFF);
	UC_F_FlashData_A[52] = (unsigned char)((UI_CalibrateVolML_H1_DF >> 8) & 0xFF);
	UC_F_FlashData_A[53] = (unsigned char)(UI_CalibrateVolML_H1_DF & 0xFF);

	UC_F_FlashData_A[54] = (unsigned char)((UI_UserCalibrateVolML_H1_DF >> 24) & 0xFF); // UI_UserCalibrateVolML_H1_DF 用户标定流量（54-57）
	UC_F_FlashData_A[55] = (unsigned char)((UI_UserCalibrateVolML_H1_DF >> 16) & 0xFF);
	UC_F_FlashData_A[56] = (unsigned char)((UI_UserCalibrateVolML_H1_DF >> 8) & 0xFF);
	UC_F_FlashData_A[57] = (unsigned char)(UI_UserCalibrateVolML_H1_DF & 0xFF);

	UC_F_FlashData_A[58] = (unsigned char)((UI_MaxFlowML_H2_DF >> 24) & 0xFF); // UI_MaxFlowML_H2_DF（58-61）
	UC_F_FlashData_A[59] = (unsigned char)((UI_MaxFlowML_H2_DF >> 16) & 0xFF);
	UC_F_FlashData_A[60] = (unsigned char)((UI_MaxFlowML_H2_DF >> 8) & 0xFF);
	UC_F_FlashData_A[61] = (unsigned char)(UI_MaxFlowML_H2_DF & 0xFF);

	UC_F_FlashData_A[62] = (unsigned char)((UI_C_SetMinAdValue4_DF >> 8) & 0xFF); // UI_C_SetMinAdValue4_DF（62-63）
	UC_F_FlashData_A[63] = (unsigned char)(UI_C_SetMinAdValue4_DF & 0xFF);

	UC_F_FlashData_A[64] = (unsigned char)((UI_C_SetMaxAdValue20_DF >> 8) & 0xFF); // UI_C_SetMaxAdValue20_DF（64-65）
	UC_F_FlashData_A[65] = (unsigned char)(UI_C_SetMaxAdValue20_DF & 0xFF);

	UC_F_FlashData_A[66] = (unsigned char)((UI_4_20MA_AD_K_DF >> 8) & 0xFF); // UI_4_20MA_AD_K_DF（66-67）
	UC_F_FlashData_A[67] = (unsigned char)(UI_4_20MA_AD_K_DF & 0xFF);

	UC_F_FlashData_A[68] = (unsigned char)((UI_4_20MA_AD_B_DF >> 8) & 0xFF); // UI_4_20MA_AD_B_DF（68-69）
	UC_F_FlashData_A[69] = (unsigned char)(UI_4_20MA_AD_B_DF & 0xFF);

	UC_F_FlashData_A[70] = (unsigned char)(UI_AddSub_B_DF & 0xFF); // UI_AddSub_B_DF（70）

	UC_F_FlashData_A[71] = (unsigned char)(UI_BaudRateStatus_SF & 0xFF); // UI_BaudRateStatus_SF（71）

	UC_F_FlashData_A[72] = (unsigned char)((UI_CalibParam_MA_DF >> 8) & 0xFF); // UI_CalibParam_MA_DF（72-73）
	UC_F_FlashData_A[73] = (unsigned char)(UI_CalibParam_MA_DF & 0xFF);

	UC_F_FlashData_A[74] = (unsigned char)(UI_SlowModeStatus_SF & 0xFF); // UI_SlowModeStatus_SF（74）

	UC_F_FlashData_A[75] = (unsigned char)(UI_IsPessureSwitch_SF & 0xFF); // UI_IsPessureSwitch_SF（75） //压力 开关

	UC_F_FlashData_A[76] = (unsigned char)((UI_PessureRange_DF >> 8) & 0xFF); // UI_PessureRange_DF（76-77）
	UC_F_FlashData_A[77] = (unsigned char)(UI_PessureRange_DF & 0xFF);

	UC_F_FlashData_A[78] = (unsigned char)((UI_PessureHigh_DF >> 8) & 0xFF); // UI_PessureHigh_DF（78-79）
	UC_F_FlashData_A[79] = (unsigned char)(UI_PessureHigh_DF & 0xFF);

	UC_F_FlashData_A[80] = (unsigned char)((UI_PessureLow_DF >> 8) & 0xFF); // UI_PessureLow_DF（80-81）
	UC_F_FlashData_A[81] = (unsigned char)(UI_PessureLow_DF & 0xFF);

	UC_F_FlashData_A[82] = (unsigned char)((UC_PowerOffLock_SF >> 8) & 0xFF); // UC_PowerOffLock_SF  （82） 断电停机
	UC_F_FlashData_A[83] = (unsigned char)(UI_IsLevelSwitch_SF & 0xFF);  // UI_IsLevelSwitch_SF（83） 液位开关

	UC_F_FlashData_A[84] = (unsigned char)((UI_LevelHigh_DF >> 8) & 0xFF); // UI_LevelHigh_DF（84-85）
	UC_F_FlashData_A[85] = (unsigned char)(UI_LevelHigh_DF & 0xFF);

	UC_F_FlashData_A[86] = (unsigned char)((UI_LevelLow_DF >> 8) & 0xFF); // UI_LevelLow_DF（86-87）
	UC_F_FlashData_A[87] = (unsigned char)(UI_LevelLow_DF & 0xFF);

	UC_F_FlashData_A[88] = (unsigned char)(UI_RemoteControl_SF & 0xFF); // UI_RemoteControl_SF（88）

	UC_F_FlashData_A[89] = (unsigned char)(UI_RunRelayOutput_SF & 0xFF); // UI_RunRelayOutput_SF（89）

	UC_F_FlashData_A[90] = (unsigned char)(UI_FaultyRelayOutput_SF & 0xFF); // UI_FaultyRelayOutput_SF（90）

	UC_F_FlashData_A[91] = (unsigned char)(UI_ScreenSaver_SF & 0xFF); // UI_ScreenSaver_SF（91）

	UC_F_FlashData_A[92] = (unsigned char)((UI_ScreenSaverTime_DF >> 8) & 0xFF); // UI_ScreenSaverTime_DF（92-93）
	UC_F_FlashData_A[93] = (unsigned char)(UI_ScreenSaverTime_DF & 0xFF);

	UC_F_FlashData_A[94] = (unsigned char)(UI_MachineCode_DF & 0xFF); // UI_MachineCode_DF（94） 机器码

	UC_F_FlashData_A[95] = (unsigned char)(UI_LanguageSet_SF & 0xFF); // UI_LanguageSet_SF（95） 语言

	UC_F_FlashData_A[96] = (unsigned char)((UI_RatedMAXFlow_DF >> 8) & 0xFF); // UI_RatedFlow_DF（96-97） 
	UC_F_FlashData_A[97] = (unsigned char)(UI_RatedMAXFlow_DF & 0xFF);

	UC_F_FlashData_A[98] = (unsigned char)((LI_RatedMAXPulseWidth_DF >> 8) & 0xFF); // LI_RatedMAXPulseWidth_DF（98-99）
	UC_F_FlashData_A[99] = (unsigned char)(LI_RatedMAXPulseWidth_DF & 0xFF);

	// 用户密码（100-107）
	UC_F_FlashData_A[100] = UC_PWDUser_A[0];
	UC_F_FlashData_A[101] = UC_PWDUser_A[1];
	UC_F_FlashData_A[102] = UC_PWDUser_A[2];
	UC_F_FlashData_A[103] = UC_PWDUser_A[3];
	UC_F_FlashData_A[104] = UC_PWDUser_A[4];
	UC_F_FlashData_A[105] = UC_PWDUser_A[5];
	UC_F_FlashData_A[106] = UC_PWDUser_A[6];
	UC_F_FlashData_A[107] = UC_PWDUser_A[7];

	// 工厂密码（108-115）
	UC_F_FlashData_A[108] = UC_PWDFactory_A[0];
	UC_F_FlashData_A[109] = UC_PWDFactory_A[1];
	UC_F_FlashData_A[110] = UC_PWDFactory_A[2];
	UC_F_FlashData_A[111] = UC_PWDFactory_A[3];
	UC_F_FlashData_A[112] = UC_PWDFactory_A[4];
	UC_F_FlashData_A[113] = UC_PWDFactory_A[5];
	UC_F_FlashData_A[114] = UC_PWDFactory_A[6];
	UC_F_FlashData_A[115] = UC_PWDFactory_A[7];

	// 设备ID（116-123）
	UC_F_FlashData_A[116] = UC_DeviceId_A[0];
	UC_F_FlashData_A[117] = UC_DeviceId_A[1];
	UC_F_FlashData_A[118] = UC_DeviceId_A[2];
	UC_F_FlashData_A[119] = UC_DeviceId_A[3];
	UC_F_FlashData_A[120] = UC_DeviceId_A[4];
	UC_F_FlashData_A[121] = UC_DeviceId_A[5];
	UC_F_FlashData_A[122] = UC_DeviceId_A[6];
	UC_F_FlashData_A[123] = UC_DeviceId_A[7];
	
	//剩余批量容积 LI_ResidualFlow_D （124-127）
	UC_F_FlashData_A[124] = (unsigned char)((LI_ResidualFlow_D >> 24) & 0xFF);
	UC_F_FlashData_A[125] = (unsigned char)((LI_ResidualFlow_D >> 16) & 0xFF);
	UC_F_FlashData_A[126] = (unsigned char)((LI_ResidualFlow_D >> 8) & 0xFF);
	UC_F_FlashData_A[127] = (unsigned char)(LI_ResidualFlow_D & 0xFF);
	//批次剩余时间
	UC_F_FlashData_A[128] = (unsigned char)((UI_CountSeconds_D >> 8) & 0xFF); // LI_RatedMAXPulseWidth_DF（98-99）
	UC_F_FlashData_A[129] = (unsigned char)(UI_CountSeconds_D & 0xFF);
	//批次是否自启动
	UC_F_FlashData_A[130] = (unsigned char)(UC_IsPulseLaunch_S & 0xFF);
	//开机状态
	UC_F_FlashData_A[131] = (unsigned char)(UC_C_OnOff_S & 0xFF);
	//循环
	UC_F_FlashData_A[132] = (unsigned char)(UC_IsDelayedLaunch_S & 0xFF);
	UC_F_FlashData_A[133] = (unsigned char)((UC_IsDelayedLaunch_C >> 8) & 0xFF); //循环开始计时
	UC_F_FlashData_A[134] = (unsigned char)(UC_IsDelayedLaunch_C & 0xFF);
	//循环总计时
	UC_F_FlashData_A[135] = (unsigned char)((LI_LoopResidualCycleTime_D >> 24) & 0xFF);
	UC_F_FlashData_A[136] = (unsigned char)((LI_LoopResidualCycleTime_D >> 16) & 0xFF);
	UC_F_FlashData_A[137] = (unsigned char)((LI_LoopResidualCycleTime_D >> 8) & 0xFF);
	UC_F_FlashData_A[138] = (unsigned char)(LI_LoopResidualCycleTime_D & 0xFF);
	//循环运行时间
	UC_F_FlashData_A[139] = (unsigned char)((UC_LaunchTime_C >> 8) & 0xFF);
	UC_F_FlashData_A[140] = (unsigned char)(UC_LaunchTime_C & 0xFF);
	//循环剩余流量
	UC_F_FlashData_A[141] = (unsigned char)((LI_LoopResidualFlow_D >> 24) & 0xFF);
	UC_F_FlashData_A[142] = (unsigned char)((LI_LoopResidualFlow_D >> 16) & 0xFF);
	UC_F_FlashData_A[143] = (unsigned char)((LI_LoopResidualFlow_D >> 8) & 0xFF);
	UC_F_FlashData_A[144] = (unsigned char)(LI_LoopResidualFlow_D & 0xFF);
	//厂家标定
	UC_F_FlashData_A[145] = (unsigned char)((UI_FactoryRatedMAXFlow_DF >> 8) & 0xFF);
	UC_F_FlashData_A[146] = (unsigned char)(UI_FactoryRatedMAXFlow_DF & 0xFF);
	//用户标定
	UC_F_FlashData_A[147] = (unsigned char)((UI_UserRatedMAXFlow_DF >> 8) & 0xFF);
	UC_F_FlashData_A[148] = (unsigned char)(UI_UserRatedMAXFlow_DF & 0xFF);
	//设备运行状态
	UC_F_FlashData_A[149] = (unsigned char)(UC_C_OnOffStatus_S & 0xFF);
	//累计流量立方
	UC_F_FlashData_A[150] = (unsigned char)((UI_FlowInM3_DF >> 8) & 0xFF);
	UC_F_FlashData_A[151] = (unsigned char)(UI_FlowInM3_DF & 0xFF);
	//累计流量L
	UC_F_FlashData_A[152] = (unsigned char)((UI_TotalFlowL_H1_DF >> 8) & 0xFF);
	UC_F_FlashData_A[153] = (unsigned char)(UI_TotalFlowL_H1_DF & 0xFF);

	//保存报警信息---------------------[154-183]************
	save_alarms();
	
	
	ULI_D_FlashTotal=0;  //校验和
	for(UC_D_FlashCount=4;UC_D_FlashCount<184;UC_D_FlashCount++)   //校验和后期需要修改 现在是从4开始的**bug**
	{
		UC_D_FlashTemp=UC_F_FlashData_A[UC_D_FlashCount];
		ULI_D_FlashTotal=ULI_D_FlashTotal+UC_D_FlashTemp;
	}
	UC_F_FlashData_A[184]=(unsigned char)((ULI_D_FlashTotal>>24)&0xFF);
	UC_F_FlashData_A[185]=(unsigned char)((ULI_D_FlashTotal>>16)&0xFF);
	UC_F_FlashData_A[186]=(unsigned char)((ULI_D_FlashTotal>>8)&0xFF);
	UC_F_FlashData_A[187]=(unsigned char)(ULI_D_FlashTotal&0xFF);
}
void FlashGetPar(void)
{	
	if(UC_F_FlashData_A[0]==0xFF&&UC_F_FlashData_A[1]==0XFF)		//Flash异常擦除
	{
		ComSetParInit(); //初始化数据
		ULI_C_PulseCallBack_C=0;
	}
	else
	{
		ULI_D_FlashTotal=0;
		for(UC_D_FlashCount=4;UC_D_FlashCount<184;UC_D_FlashCount++)		//计算flash中的校验和 *bug* 是否是2开始
		{
			UC_D_FlashTemp=UC_F_FlashData_A[UC_D_FlashCount];
			ULI_D_FlashTotal=ULI_D_FlashTotal+UC_D_FlashTemp;
		}
		ULI_D_FlashTotalCRC=UC_F_FlashData_A[184]*0x1000000+UC_F_FlashData_A[185]*0x10000+UC_F_FlashData_A[186]*0x100+UC_F_FlashData_A[187];
	
  		if(ULI_D_FlashTotal==ULI_D_FlashTotalCRC)
		{
			FlashGetValue();//读取flash
			Check_eeproom();//写入 eeprm
		}
		else
		{
			eeprom_read(0x00);
			// 索引关系：UC_C_Read_eeprom_Data[n] = UC_F_FlashData_A[n + 4]

			// ---- 第1组：运行参数 ----
			UC_ModeRun_SF = UC_C_Read_eeprom_Data[0];  // [4]
			UC_ModeRunTemp_SF=UC_ModeRun_SF;//用于切换模式的临时变量
			
			LI_ManualFlow_L_DF = ((UC_C_Read_eeprom_Data[1] & 0xFFFFFFFF) << 24) + 
			                    ((UC_C_Read_eeprom_Data[2] & 0xFFFFFFFF) << 16) + 
			                    ((UC_C_Read_eeprom_Data[3] & 0xFFFF) << 8) + 
			                     UC_C_Read_eeprom_Data[4];  // [5]-[8]

			LI_PulseFlow_ML_DF = ((UC_C_Read_eeprom_Data[5] & 0xFFFFFFFF) << 24) + 
			                    ((UC_C_Read_eeprom_Data[6] & 0xFFFFFFFF) << 16) + 
			                    ((UC_C_Read_eeprom_Data[7] & 0xFFFF) << 8) + 
			                     UC_C_Read_eeprom_Data[8];  // [9]-[12]

			// ---- 第2组：脉冲与4-20mA设置 ----
			//UC_PulseMemory_SF = UC_C_Read_eeprom_Data[9];   // [13] 脉冲记忆不要 改成 批次手动
			UC_BatchManual_SF = UC_C_Read_eeprom_Data[9];   // [13] 批次手动
			UC_04_20MAType_SF = UC_C_Read_eeprom_Data[10];  // [14]

			UC_04_20MA_I1_DF = ((UC_C_Read_eeprom_Data[11] & 0xFFFF) << 8) + 
			                    UC_C_Read_eeprom_Data[12];  // [15]-[16]

			UC_04_20MA_Q1_DF = ((UC_C_Read_eeprom_Data[13] & 0xFFFF) << 8) + 
			                    UC_C_Read_eeprom_Data[14];  // [17]-[18]

			UC_04_20MA_I2_DF = ((UC_C_Read_eeprom_Data[15] & 0xFFFF) << 8) + 
			                    UC_C_Read_eeprom_Data[16];  // [19]-[20]

			UC_04_20MA_Q2_DF = ((UC_C_Read_eeprom_Data[17] & 0xFFFF) << 8) + 
			                    UC_C_Read_eeprom_Data[18];  // [21]-[22]

			// ---- 第3组：批次总量与时间 ----
			LI_LotVolume_DF = ((UC_C_Read_eeprom_Data[19] & 0xFFFFFFFF) << 24) + 
			                 ((UC_C_Read_eeprom_Data[20] & 0xFFFFFFFF) << 16) + 
			                 ((UC_C_Read_eeprom_Data[21] & 0xFFFF) << 8) + 
			                  UC_C_Read_eeprom_Data[22];  // [23]-[26]

			UC_LotVolumeUnit_SF = UC_C_Read_eeprom_Data[23];  // [27]

			UC_AddTimeSec_DF = ((UC_C_Read_eeprom_Data[24] & 0xFFFFFFFF) << 24) + 
			                  ((UC_C_Read_eeprom_Data[25] & 0xFFFFFFFF) << 16) + 
			                  ((UC_C_Read_eeprom_Data[26] & 0xFFFF) << 8) + 
			                   UC_C_Read_eeprom_Data[27];  // [28]-[31]

			// ---- 第4组：循环控制参数 ----
			LI_LoopLotVolume_DF = ((UC_C_Read_eeprom_Data[28] & 0xFFFFFFFF) << 24) + 
			                     ((UC_C_Read_eeprom_Data[29] & 0xFFFFFFFF) << 16) + 
			                     ((UC_C_Read_eeprom_Data[30] & 0xFFFF) << 8) + 
			                      UC_C_Read_eeprom_Data[31];  // [32]-[35]

			UC_LoopLotVolumeUnit_SF = UC_C_Read_eeprom_Data[32];  // [36]

			UC_LoopADDTime_DF = ((UC_C_Read_eeprom_Data[33] & 0xFFFFFFFF) << 24) + 
			                   ((UC_C_Read_eeprom_Data[34] & 0xFFFFFFFF) << 16) + 
			                   ((UC_C_Read_eeprom_Data[35] & 0xFFFF) << 8) + 
			                    UC_C_Read_eeprom_Data[36];  // [37]-[40]

			UC_LoopCycleTime_DF = ((UC_C_Read_eeprom_Data[37] & 0xFFFFFFFF) << 24) + 
			                     ((UC_C_Read_eeprom_Data[38] & 0xFFFFFFFF) << 16) + 
			                     ((UC_C_Read_eeprom_Data[39] & 0xFFFF) << 8) + 
			                      UC_C_Read_eeprom_Data[40];  // [41]-[44]

			UC_LoopStartDelayTime_DF = ((UC_C_Read_eeprom_Data[41] & 0xFFFFFFFF) << 24) + 
			                          ((UC_C_Read_eeprom_Data[42] & 0xFFFFFFFF) << 16) + 
			                          ((UC_C_Read_eeprom_Data[43] & 0xFFFF) << 8) + 
			                           UC_C_Read_eeprom_Data[44];  // [45]-[48]

			// ---- 第5组：标定与通信参数 ----
			UC_IsCalibrated_DF = UC_C_Read_eeprom_Data[45];  // [49]

			UI_CalibrateVolML_H1_DF = ((UC_C_Read_eeprom_Data[46] & 0xFFFFFFFF) << 24) + 
			                         ((UC_C_Read_eeprom_Data[47] & 0xFFFFFFFF) << 16) + 
			                         ((UC_C_Read_eeprom_Data[48] & 0xFFFF) << 8) + 
			                          UC_C_Read_eeprom_Data[49];  // [50]-[53]

			UI_UserCalibrateVolML_H1_DF = ((UC_C_Read_eeprom_Data[50] & 0xFFFFFFFF) << 24) + 
			                             ((UC_C_Read_eeprom_Data[51] & 0xFFFFFFFF) << 16) + 
			                             ((UC_C_Read_eeprom_Data[52] & 0xFFFF) << 8) + 
			                              UC_C_Read_eeprom_Data[53];  // [54]-[57]

			UI_MaxFlowML_H2_DF = ((UC_C_Read_eeprom_Data[54] & 0xFFFFFFFF) << 24) + 
			                    ((UC_C_Read_eeprom_Data[55] & 0xFFFFFFFF) << 16) + 
			                    ((UC_C_Read_eeprom_Data[56] & 0xFFFF) << 8) + 
			                     UC_C_Read_eeprom_Data[57];  // [58]-[61]

			// ---- 第6组：4-20mA标定参数 ----
			UI_C_SetMinAdValue4_DF = ((UC_C_Read_eeprom_Data[58] & 0xFFFF) << 8) + 
			                         UC_C_Read_eeprom_Data[59];  // [62]-[63]

			UI_C_SetMaxAdValue20_DF = ((UC_C_Read_eeprom_Data[60] & 0xFFFF) << 8) + 
			                          UC_C_Read_eeprom_Data[61];  // [64]-[65]

			UI_4_20MA_AD_K_DF = ((UC_C_Read_eeprom_Data[62] & 0xFFFF) << 8) + 
			                    UC_C_Read_eeprom_Data[63];  // [66]-[67]

			UI_4_20MA_AD_B_DF = ((UC_C_Read_eeprom_Data[64] & 0xFFFF) << 8) + 
			                    UC_C_Read_eeprom_Data[65];  // [68]-[69]

			// ---- 第7组：系统设置 ----
			UI_AddSub_B_DF = UC_C_Read_eeprom_Data[66];          // [70]
			UI_BaudRateStatus_SF = UC_C_Read_eeprom_Data[67];    // [71]

			UI_CalibParam_MA_DF = ((UC_C_Read_eeprom_Data[68] & 0xFFFF) << 8) + 
			                      UC_C_Read_eeprom_Data[69];    // [72]-[73]

			UI_SlowModeStatus_SF = UC_C_Read_eeprom_Data[70];   // [74]
			UI_IsPessureSwitch_SF = UC_C_Read_eeprom_Data[71];   // [75] 压力开关

			UI_PessureRange_DF = ((UC_C_Read_eeprom_Data[72] & 0xFFFF) << 8) + 
			                    UC_C_Read_eeprom_Data[73];      // [76]-[77]

			UI_PessureHigh_DF = ((UC_C_Read_eeprom_Data[74] & 0xFFFF) << 8) + 
			                   UC_C_Read_eeprom_Data[75];       // [78]-[79]

			UI_PessureLow_DF = ((UC_C_Read_eeprom_Data[76] & 0xFFFF) << 8) + 
			                  UC_C_Read_eeprom_Data[77];        // [80]-[81]

			UI_IsLevelSwitch_SF = ((UC_C_Read_eeprom_Data[78] & 0xFFFF) << 8) + 
			                  UC_C_Read_eeprom_Data[79];        // [82]-[83] //液位开关

			UI_LevelHigh_DF = ((UC_C_Read_eeprom_Data[80] & 0xFFFF) << 8) + 
			                 UC_C_Read_eeprom_Data[81];         // [84]-[85]

			UI_LevelLow_DF = ((UC_C_Read_eeprom_Data[82] & 0xFFFF) << 8) + 
			                UC_C_Read_eeprom_Data[83];          // [86]-[87]

			// ---- 第8组：继电器与显示 ----
			UI_RemoteControl_SF = UC_C_Read_eeprom_Data[84];     // [88]
			UI_RunRelayOutput_SF = UC_C_Read_eeprom_Data[85];    // [89]
			UI_FaultyRelayOutput_SF = UC_C_Read_eeprom_Data[86]; // [90]
			UI_ScreenSaver_SF = UC_C_Read_eeprom_Data[87];       // [91]

			UI_ScreenSaverTime_DF = ((UC_C_Read_eeprom_Data[88] & 0xFFFF) << 8) + 
			                       UC_C_Read_eeprom_Data[89];    // [92]-[93]

			UI_MachineCode_DF = UC_C_Read_eeprom_Data[90];       // [94]
			UI_LanguageSet_SF = UC_C_Read_eeprom_Data[91];       // [95]

			UI_RatedMAXFlow_DF = ((UC_C_Read_eeprom_Data[92] & 0xFFFF) << 8) + 
			                 UC_C_Read_eeprom_Data[93];          // [96]-[97]

			LI_RatedMAXPulseWidth_DF = ((UC_C_Read_eeprom_Data[94] & 0xFFFF) << 8) + 
			                       UC_C_Read_eeprom_Data[95];    // [98]-[99]
					       
			KS_PWMPULSE=LI_RatedMAXPulseWidth_DF;//快吸慢推

			// ---- 第9组：密码与设备ID ----
			// 用户密码（100-107）
			UC_PWDUser_A[0] = UC_C_Read_eeprom_Data[96];  // [100]
			UC_PWDUser_A[1] = UC_C_Read_eeprom_Data[97];  // [101]
			UC_PWDUser_A[2] = UC_C_Read_eeprom_Data[98];  // [102]
			UC_PWDUser_A[3] = UC_C_Read_eeprom_Data[99];  // [103]
			UC_PWDUser_A[4] = UC_C_Read_eeprom_Data[100]; // [104]
			UC_PWDUser_A[5] = UC_C_Read_eeprom_Data[101]; // [105]
			UC_PWDUser_A[6] = UC_C_Read_eeprom_Data[102]; // [106]
			UC_PWDUser_A[7] = UC_C_Read_eeprom_Data[103]; // [107]

			// 工厂密码（108-115）
			UC_PWDFactory_A[0] = UC_C_Read_eeprom_Data[104]; // [108]
			UC_PWDFactory_A[1] = UC_C_Read_eeprom_Data[105]; // [109]
			UC_PWDFactory_A[2] = UC_C_Read_eeprom_Data[106]; // [110]
			UC_PWDFactory_A[3] = UC_C_Read_eeprom_Data[107]; // [111]
			UC_PWDFactory_A[4] = UC_C_Read_eeprom_Data[108]; // [112]
			UC_PWDFactory_A[5] = UC_C_Read_eeprom_Data[109]; // [113]
			UC_PWDFactory_A[6] = UC_C_Read_eeprom_Data[110]; // [114]
			UC_PWDFactory_A[7] = UC_C_Read_eeprom_Data[111]; // [115]

			// 设备ID（116-123）
			UC_DeviceId_A[0] = UC_C_Read_eeprom_Data[112]; // [116]
			UC_DeviceId_A[1] = UC_C_Read_eeprom_Data[113]; // [117]
			UC_DeviceId_A[2] = UC_C_Read_eeprom_Data[114]; // [118]
			UC_DeviceId_A[3] = UC_C_Read_eeprom_Data[115]; // [119]
			UC_DeviceId_A[4] = UC_C_Read_eeprom_Data[116]; // [120]
			UC_DeviceId_A[5] = UC_C_Read_eeprom_Data[117]; // [121]
			UC_DeviceId_A[6] = UC_C_Read_eeprom_Data[118]; // [122]
			UC_DeviceId_A[7] = UC_C_Read_eeprom_Data[119]; // [123]
			
			// 批次剩余流量（120-123）
			LI_ResidualFlow_D = ((UC_C_Read_eeprom_Data[120] & 0xFFFFFFFF) << 24) + 
			                         ((UC_C_Read_eeprom_Data[121] & 0xFFFFFFFF) << 16) + 
			                         ((UC_C_Read_eeprom_Data[122] & 0xFFFF) << 8) + 
			                          UC_C_Read_eeprom_Data[123];  // [50]-[53]
			
			//批次运行时间
			UI_CountSeconds_D = ((UC_C_Read_eeprom_Data[124] & 0xFFFF) << 8) + 
			                       UC_C_Read_eeprom_Data[125];    
			
			//批次自启动
			UC_IsPulseLaunch_S = UC_C_Read_eeprom_Data[126]; 
			//开机状态
			UC_C_OnOff_S = UC_C_Read_eeprom_Data[127]; 
			//循环运行
			UC_IsDelayedLaunch_S =  UC_C_Read_eeprom_Data[128]; 
			//计时
			UC_IsDelayedLaunch_C = ((UC_C_Read_eeprom_Data[129] & 0xFFFF) << 8) + 
			                       UC_C_Read_eeprom_Data[130];   
			//总时间		       
		 	LI_LoopResidualCycleTime_D = ((UC_C_Read_eeprom_Data[131] & 0xFFFFFFFF) << 24) + 
			                         ((UC_C_Read_eeprom_Data[132] & 0xFFFFFFFF) << 16) + 
			                         ((UC_C_Read_eeprom_Data[133] & 0xFFFF) << 8) + 
			                         UC_C_Read_eeprom_Data[134];
			//运行计时
			UC_LaunchTime_C = ((UC_C_Read_eeprom_Data[135] & 0xFFFF) << 8) + 
			                       UC_C_Read_eeprom_Data[136];   
			//剩余流量
			LI_LoopResidualFlow_D = ((UC_C_Read_eeprom_Data[137] & 0xFFFFFFFF) << 24) + 
			                         ((UC_C_Read_eeprom_Data[138] & 0xFFFFFFFF) << 16) + 
			                         ((UC_C_Read_eeprom_Data[139] & 0xFFFF) << 8) + 
			                         UC_C_Read_eeprom_Data[140];
			
			//厂家额定最大流量
			UI_FactoryRatedMAXFlow_DF = ((UC_C_Read_eeprom_Data[141] & 0xFFFF) << 8) + 
			                       UC_C_Read_eeprom_Data[142]; 
			//用户额定最大流量
			UI_UserRatedMAXFlow_DF = ((UC_C_Read_eeprom_Data[143] & 0xFFFF) << 8) + 
			                       UC_C_Read_eeprom_Data[144]; 
			//设备运行状态
			UC_C_OnOffStatus_S = UC_C_Read_eeprom_Data[145]; 
			
			//累计立方
			UI_FlowInM3_DF = ((UC_C_Read_eeprom_Data[146] & 0xFFFF) << 8) + 
			                       UC_C_Read_eeprom_Data[147]; 
			//累计流量
			UI_TotalFlowL_H1_DF = ((UC_C_Read_eeprom_Data[148] & 0xFFFF) << 8) + 
			                       UC_C_Read_eeprom_Data[149]; 
						  
		}
		
	}
	
}
/*****************************************
* Function Name: ComSetParInit  
* Description  : 数据初始化
* Arguments    : None
* Return Value : None
******************************************/
void FlashGetValue(void)
{
		UC_ModeRun_SF=UC_F_FlashData_A[4];//UC_ModeRun_SF
		UC_ModeRunTemp_SF=UC_ModeRun_SF;//用于切换模式的临时变量
		
		LI_ManualFlow_L_DF=((UC_F_FlashData_A[5]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[6]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[7]&0xFFFF)<<8)+UC_F_FlashData_A[8]; //LI_ManualFlow_L_DF
		LI_PulseFlow_ML_DF=((UC_F_FlashData_A[9]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[10]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[11]&0xFFFF)<<8)+UC_F_FlashData_A[12]; //LI_PulseFlow_ML_DF
		//UC_PulseMemory_SF=UC_F_FlashData_A[13];//UC_PulseMemory_SF
		UC_BatchManual_SF=UC_F_FlashData_A[13];//UC_BatchManual_SF 脉冲记忆不用 改成批次手动
		UC_04_20MAType_SF=UC_F_FlashData_A[14];//UC_04_20MAType_SF
		UC_04_20MA_I1_DF=((UC_F_FlashData_A[15]&0xFFFF)<<8)+UC_F_FlashData_A[16]; //UC_04_20MA_I1_DF
		UC_04_20MA_Q1_DF=((UC_F_FlashData_A[17]&0xFFFF)<<8)+UC_F_FlashData_A[18]; //UC_04_20MA_Q1_DF
		UC_04_20MA_I2_DF=((UC_F_FlashData_A[19]&0xFFFF)<<8)+UC_F_FlashData_A[20]; //UC_04_20MA_I2_DF
		UC_04_20MA_Q2_DF=((UC_F_FlashData_A[21]&0xFFFF)<<8)+UC_F_FlashData_A[22]; //UC_04_20MA_Q2_DF
		//批次数据
		LI_LotVolume_DF=((UC_F_FlashData_A[23]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[24]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[25]&0xFFFF)<<8)+UC_F_FlashData_A[26]; //LI_LotVolume_DF
		UC_LotVolumeUnit_SF=UC_F_FlashData_A[27]; //UC_LotVolumeUnit_SF
		UC_AddTimeSec_DF=((UC_F_FlashData_A[28]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[29]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[30]&0xFFFF)<<8)+UC_F_FlashData_A[31]; //UC_AddTimeSec_DF
		//批次临时数据
		LI_LotVolumeTemp_DF=LI_LotVolume_DF;
		UC_LotVolumeUnitTemp_SF=UC_LotVolumeUnit_SF;
		UC_AddTimeSecTemp_DF=UC_AddTimeSec_DF;
		
		//循环投加数据
		LI_LoopLotVolume_DF=((UC_F_FlashData_A[32]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[33]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[34]&0xFFFF)<<8)+UC_F_FlashData_A[35]; //LI_LoopLotVolume_DF
		UC_LoopLotVolumeUnit_SF=UC_F_FlashData_A[36]; //UC_LoopLotVolumeUnit_SF
		UC_LoopADDTime_DF=((UC_F_FlashData_A[37]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[38]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[39]&0xFFFF)<<8)+UC_F_FlashData_A[40]; //UC_LoopADDTime_DF
		UC_LoopCycleTime_DF=((UC_F_FlashData_A[41]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[42]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[43]&0xFFFF)<<8)+UC_F_FlashData_A[44]; //UC_LoopCycleTime_DF
		UC_LoopStartDelayTime_DF=((UC_F_FlashData_A[45]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[46]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[47]&0xFFFF)<<8)+UC_F_FlashData_A[48]; //UC_LoopCycleTime_DF
		//循环投加临时变量
		LI_LoopLotVolumeTemp_DF=LI_LoopLotVolume_DF;
		UC_LoopLotVolumeUnitTemp_SF=UC_LoopLotVolumeUnit_SF;
		UC_LoopADDTimeTemp_DF=UC_LoopADDTime_DF;
		UC_LoopCycleTimeTemp_DF=UC_LoopCycleTime_DF;
		UC_LoopStartDelayTimeTemp_DF=UC_LoopStartDelayTime_DF;
		
		UC_IsCalibrated_DF=UC_F_FlashData_A[49];//UC_IsCalibrated_DF
		
		UI_CalibrateVolML_H1_DF=((UC_F_FlashData_A[50]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[51]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[52]&0xFFFF)<<8)+UC_F_FlashData_A[53]; // 50-53 (原厂标定流量)

		UI_UserCalibrateVolML_H1_DF=((UC_F_FlashData_A[54]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[55]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[56]&0xFFFF)<<8)+UC_F_FlashData_A[57]; // 54-57 (用户标定流量)

		UI_MaxFlowML_H2_DF=((UC_F_FlashData_A[58]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[59]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[60]&0xFFFF)<<8)+UC_F_FlashData_A[61]; // 58-61 (最大流量)

		UI_C_SetMinAdValue4_DF=((UC_F_FlashData_A[62]&0xFFFF)<<8)+UC_F_FlashData_A[63]; // 62-63 (最小AD值)
		UI_C_SetMaxAdValue20_DF=((UC_F_FlashData_A[64]&0xFFFF)<<8)+UC_F_FlashData_A[65]; // 64-65 (最大AD值)
		UI_4_20MA_AD_K_DF=((UC_F_FlashData_A[66]&0xFFFF)<<8)+UC_F_FlashData_A[67]; // 66-67 (4-20mA斜率)
		UI_4_20MA_AD_B_DF=((UC_F_FlashData_A[68]&0xFFFF)<<8)+UC_F_FlashData_A[69]; // 68-69 (4-20mA截距)

		UI_AddSub_B_DF=UC_F_FlashData_A[70]; // 70 (加减模式)
		UI_BaudRateStatus_SF=UC_F_FlashData_A[71]; // 71 (波特率状态)
		UI_CalibParam_MA_DF=((UC_F_FlashData_A[72]&0xFFFF)<<8)+UC_F_FlashData_A[73]; // 72-73 (校准参数)

		UI_SlowModeStatus_SF=UC_F_FlashData_A[74]; // 74 (慢速模式)
		
		UI_IsPessureSwitch_SF=UC_F_FlashData_A[75]; // 75 (压力等级使能) 压力开关

		UI_PessureRange_DF=((UC_F_FlashData_A[76]&0xFFFF)<<8)+UC_F_FlashData_A[77]; // 76-77 (压力范围)
		UI_PessureHigh_DF=((UC_F_FlashData_A[78]&0xFFFF)<<8)+UC_F_FlashData_A[79]; // 78-79 (压力上限)
		UI_PessureLow_DF=((UC_F_FlashData_A[80]&0xFFFF)<<8)+UC_F_FlashData_A[81]; // 80-81 (压力下限)
		UC_PowerOffLock_SF=UC_F_FlashData_A[82]; //断电保护
		UI_IsLevelSwitch_SF=UC_F_FlashData_A[83]; // 82-83 (液位范围)  液位开关
		UI_LevelHigh_DF=((UC_F_FlashData_A[84]&0xFFFF)<<8)+UC_F_FlashData_A[85]; // 84-85 (液位上限)
		UI_LevelLow_DF=((UC_F_FlashData_A[86]&0xFFFF)<<8)+UC_F_FlashData_A[87]; // 86-87 (液位下限)

		UI_RemoteControl_SF=UC_F_FlashData_A[88]; // 88 (远程控制)
		UI_RunRelayOutput_SF=UC_F_FlashData_A[89]; // 89 (运行继电器)
		UI_FaultyRelayOutput_SF=UC_F_FlashData_A[90]; // 90 (故障继电器)
		UI_ScreenSaver_SF=UC_F_FlashData_A[91]; // 91 (屏保开关)

		UI_ScreenSaverTime_DF=((UC_F_FlashData_A[92]&0xFFFF)<<8)+UC_F_FlashData_A[93]; // 92-93 (屏保时间)
		UI_MachineCode_DF=UC_F_FlashData_A[94]; // 94 (机器码)
		UI_LanguageSet_SF=UC_F_FlashData_A[95]; // 95 (语言设置)

		UI_RatedMAXFlow_DF=((UC_F_FlashData_A[96]&0xFFFF)<<8)+UC_F_FlashData_A[97]; // 96-97 (额定流量)
		LI_RatedMAXPulseWidth_DF=((UC_F_FlashData_A[98]&0xFFFF)<<8)+UC_F_FlashData_A[99]; // 98-99 (额定脉宽)

		KS_PWMPULSE=LI_RatedMAXPulseWidth_DF;//快吸慢推
		// 用户密码（100-107）
		UC_PWDUser_A[0]=UC_F_FlashData_A[100];
		UC_PWDUser_A[1]=UC_F_FlashData_A[101];
		UC_PWDUser_A[2]=UC_F_FlashData_A[102];
		UC_PWDUser_A[3]=UC_F_FlashData_A[103];
		UC_PWDUser_A[4]=UC_F_FlashData_A[104];
		UC_PWDUser_A[5]=UC_F_FlashData_A[105];
		UC_PWDUser_A[6]=UC_F_FlashData_A[106];
		UC_PWDUser_A[7]=UC_F_FlashData_A[107];

		// 工厂密码（108-115）
		UC_PWDFactory_A[0]=UC_F_FlashData_A[108];
		UC_PWDFactory_A[1]=UC_F_FlashData_A[109];
		UC_PWDFactory_A[2]=UC_F_FlashData_A[110];
		UC_PWDFactory_A[3]=UC_F_FlashData_A[111];
		UC_PWDFactory_A[4]=UC_F_FlashData_A[112];
		UC_PWDFactory_A[5]=UC_F_FlashData_A[113];
		UC_PWDFactory_A[6]=UC_F_FlashData_A[114];
		UC_PWDFactory_A[7]=UC_F_FlashData_A[115];

		// 设备ID（116-123）
		UC_DeviceId_A[0]=UC_F_FlashData_A[116];
		UC_DeviceId_A[1]=UC_F_FlashData_A[117];
		UC_DeviceId_A[2]=UC_F_FlashData_A[118];
		UC_DeviceId_A[3]=UC_F_FlashData_A[119];
		UC_DeviceId_A[4]=UC_F_FlashData_A[120];
		UC_DeviceId_A[5]=UC_F_FlashData_A[121];
		UC_DeviceId_A[6]=UC_F_FlashData_A[122];
		UC_DeviceId_A[7]=UC_F_FlashData_A[123];
		
		//批次剩余容积（124-127）
		LI_ResidualFlow_D=((UC_F_FlashData_A[124]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[125]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[126]&0xFFFF)<<8)+UC_F_FlashData_A[127];
		//批次运行时间
		UI_CountSeconds_D =(((UC_F_FlashData_A[128]&0xFFFF)<<8)+UC_F_FlashData_A[129]);
		//批次是否启动
		UC_IsPulseLaunch_S = UC_F_FlashData_A[130];
		//开机状态
		UC_C_OnOff_S=UC_F_FlashData_A[131];
		UC_IsDelayedLaunch_S=UC_F_FlashData_A[132];//循环运行
		UC_IsDelayedLaunch_C =(((UC_F_FlashData_A[133]&0xFFFF)<<8)+UC_F_FlashData_A[134]); //计时
		//总时间
		LI_LoopResidualCycleTime_D=((UC_F_FlashData_A[135]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[136]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[137]&0xFFFF)<<8)+UC_F_FlashData_A[138];
		UC_LaunchTime_C =(((UC_F_FlashData_A[139]&0xFFFF)<<8)+UC_F_FlashData_A[140]); //运行计时
		//剩余流量
		LI_LoopResidualFlow_D=((UC_F_FlashData_A[141]&0xFFFFFFFF)<<24)+((UC_F_FlashData_A[142]&0xFFFFFFFF)<<16)+((UC_F_FlashData_A[143]&0xFFFF)<<8)+UC_F_FlashData_A[144];
		//厂家标定
		UI_FactoryRatedMAXFlow_DF =(((UC_F_FlashData_A[145]&0xFFFF)<<8)+UC_F_FlashData_A[146]); 
		//用户标定
		UI_UserRatedMAXFlow_DF =(((UC_F_FlashData_A[147]&0xFFFF)<<8)+UC_F_FlashData_A[148]); 	
		//设备运行状态
		UC_C_OnOffStatus_S=UC_F_FlashData_A[149];
		//累计流量立方
		UI_FlowInM3_DF =(((UC_F_FlashData_A[150]&0xFFFF)<<8)+UC_F_FlashData_A[151]); 	
		//累计流量L
		UI_TotalFlowL_H1_DF =(((UC_F_FlashData_A[152]&0xFFFF)<<8)+UC_F_FlashData_A[153]); 	
		
		//加载报警消息 ------------[154-183] **********报警信息
		load_alarms();
		
}

/*****************************************
* Function Name: ComSetParInit  
* Description  : set paramater default
* Arguments    : None
* Return Value : None
******************************************/
void ComSetParInit(void)
{
	UC_ModeRun_SF=MANUAL_MODE;//手动模式
	UC_04_20MAType_SF=M_4_20MATYPE; //4-20 模式
	UI_CalibrateVolML_H1_DF=0;//厂家标定容积
	UI_UserCalibrateVolML_H1_DF=0;//用户标定容积
	
	UI_BaudRateStatus_SF=BT_485CLOSE; //485关闭
	UI_CalibParam_MA_DF=1000;//1.000   Default is equal to 1   4-20MA 输出系数
	UI_Suc_Vel_Percent=6000;  //60%
	UI_MachineCode_DF=0x01;//机器码
	
	UI_RatedMAXFlow_DF=RATE_FLOW_200;//额定最大流量2000L/H
	UI_FactoryRatedMAXFlow_DF=RATE_FLOW_200; //厂家额定最大流量
	UI_UserRatedMAXFlow_DF=RATE_FLOW_200; //厂家额定最大流量
	
	LI_RatedMAXPulseWidth_DF=27000;//初始化额定最大脉宽
	KS_PWMPULSE=LI_RatedMAXPulseWidth_DF;//进入快吸慢推的最大脉宽
	
	UI_MaxFlowML_H2_DF=MAXFLOWML_H2; //默认最大流量  250.000.00L  ML_H2
	
	UC_04_20MA_Q2_DF=2000; //0-20mA 正梯度和负梯度 Q2值
	
	LI_ResidualFlow_D=0;//剩余批次容积 ML
	UI_CountSeconds_D=0;//批次运行时间
	UC_IsPulseLaunch_S=0;//批次运行状态
	LI_LoopResidualFlow_D=0; //循环投加剩余流量ML
	
	LI_ManualFlow_L_DF=RATE_FLOW_200;
	LI_ManualFlow_L_DF=LI_ManualFlow_L_DF*1000; ////模式手动模式 200.000.0 L
	UC_LotVolumeUnit_SF=1;//批次容积单位 默认L
	UC_LoopLotVolumeUnit_SF=1; //循环投机容积单位 默认L
	
	
	//用户密码 0000  0x30 ASCII等于0
	UC_PWDUser_A[0]=0x30;UC_PWDUser_A[1]=0x30;UC_PWDUser_A[2]=0x30;UC_PWDUser_A[3]=0x30;
	UC_PWDUser_A[4]=0;UC_PWDUser_A[5]=0;UC_PWDUser_A[6]=0;UC_PWDUser_A[7]=0;
	
	//工厂密码 0000  0x30 ASCII等于0 ---- 赋值0代表空着
	UC_PWDFactory_A[0]=0x30;UC_PWDFactory_A[1]=0x30;UC_PWDFactory_A[2]=0x30;UC_PWDFactory_A[3]=0x30;
	UC_PWDFactory_A[4]=0;UC_PWDFactory_A[5]=0;UC_PWDFactory_A[6]=0;UC_PWDFactory_A[7]=0;
	
	//设备ID 赋值0代表空着
	UC_DeviceId_A[0]=0;UC_DeviceId_A[1]=0;UC_DeviceId_A[2]=0;UC_DeviceId_A[3]=0;
	UC_DeviceId_A[4]=0;UC_DeviceId_A[5]=0;UC_DeviceId_A[6]=0;UC_DeviceId_A[7]=0;
	
	UI_4_20MA_AD_K_DF=2194;//4-20MA 默认斜率K
	UI_4_20MA_AD_B_DF=2628;//4-20MA 默认斜率B
	UI_IsPessureSwitch_SF=FALSE;//压力开关
	UI_IsLevelSwitch_SF=FALSE;//液位开关
	
	UI_RemoteControl_SF=0;//外部开关默认关
	UI_FaultyRelayOutput_SF=0;//报警继电器默认关
	UI_RunRelayOutput_SF=0; //机头运行继电器默认关
	UI_SlowModeStatus_SF=CLOSE; //默认开启快吸慢推 关闭
	
	UI_LanguageSet_SF=1;//系统语言模式中文
	UC_BatchManual_SF=FALSE;//批次手动关
	
	UC_C_WriteFlash_S=TRUE;
}



/*****************************************************************
* Function Name: Send Data For URT1
* Description  : URT1 Send
* Arguments    : None
* Return Value : None
******************************************************************/
void SendData_485URT1(void)
{
	if(SendReady==0xAA)
	{
		switch(SendKind)
		{
			case 0x03:
			case 0x04:
				trans_data_485(UI_URT1_SEND_D,UI_URT1_SEND_D[2]+5);
				UART1_R_Rest();
				break;
				
			case 0x06:
			case 0x10:
				trans_data_485(UI_URT1_SEND_D,8);
				UART1_R_Rest();
				break;
				
			default:
				UART1_R_Rest();
				break;
		}
		SendReady=0x55;
		
		UART1_Stop_T();
		RS485_Direction=RS485_R;
		
		my_delay(1);
		
		UART1_Start_R();
		
	}
}

/*****************************************************************485地址查询******************************************************************/
void readBackData(void)
{
	switch(address)
	{
		case 1000:
			if(UC_C_OnOff_S==TRUE) //总开关
			{
				readSingleData=1;
			}
			else 
			{
				readSingleData=0;
			}
			break;
			
		case 1001:
			if(UC_C_OnOffStatus_S==TRUE) //机器运行状态
			{
				readSingleData=2; //运行
			}
			else if(UC_C_OnOffStatus_S==WAIT)
			{
				readSingleData=1;//暂停
			}
			else 
			{
				readSingleData=0; //关机
			}
			break;
		case 1002:
			if(UC_ModeRun_SF==MANUAL_MODE)
			{
				readSingleData=0;//手动模式
			}
			else if(UC_ModeRun_SF==PULSE_MODE)
			{
				readSingleData=6;//脉冲模式
			}
			else if(UC_ModeRun_SF==MA_0_20_MODE)
			{
				if(UC_04_20MAType_SF==M_0_20MATYPE)
				{
					readSingleData=3; //0-20ma 模式
				}
				else if(UC_04_20MAType_SF==M_4_20MATYPE)
				{
					readSingleData=1; //4-20ma 模式
				}
				else if(UC_04_20MAType_SF==M_0_20MAZTTYPE)
				{
					readSingleData=4; //zt0-20ma 模式
				}
				else if(UC_04_20MAType_SF==M_0_20MAFTTYPE)
				{
					readSingleData=5; //ft0-20ma 模式
				}
			}
			else if(UC_ModeRun_SF==BATCH_MODE)
			{
				readSingleData=2;//批次
			}
			else if(UC_ModeRun_SF==CYCLIC_MODE)
			{
				readSingleData=7;//计时器
			}
			break;
		case 1003:
			readSingleData=UI_RemoteControl_SF; //外部停机
			break;
		case 1004:
			readSingleData=UI_RunRelayOutput_SF; //运行继电器开关
			break;
		case 1005:
			readSingleData=UI_FaultyRelayOutput_SF; //故障继电器开关
			break;
		case 1006:
			UI_TargetFlowL_H1_485_VD=LI_TargetFlowML_H2_DF/10000; //目标流量转化 485显示流量
			readSingleData=UI_TargetFlowL_H1_485_VD; //L_H1 升后1位 用来显示
			break;
		case 1007:
			readSingleData=UI_Percent_H2_DF/10; //运行百分比 小数点后1
			break;
		case 1008:
			readSingleData=UC_RemoteStatusHL_S; //外部停机[485远程停机/节点远程停机]
			break;
		case 1009:
			readSingleData=UI_FlowInM3_DF; //累计流量(M3)
			break;
		case 1010:
			readSingleData=UI_TotalFlowL_H1_DF; //累计流量(L)
			break;
		case 1011:
			readSingleData=UI_SlowModeStatus_SF; //快吸慢推
			break;
		case 1012:
			if(UI_IsLevelSwitch_SF==TRUE) //空桶开关
			{
				readSingleData=1; //低液位开关
			}
			else
			{
				readSingleData=0; //低液位开关
			}
			break;
		case 1013:
			if(UI_IsPessureSwitch_SF==TRUE)//压力开关
			{
				readSingleData=1;
			}
			else 
			{
				readSingleData=0;
			}
			break;
		
		case 1014:
			readSingleData=LI_ManualFlow_L_DF/1000; //手动流量设置 100.000.0 L/H
			break;
		case 1015:
			readSingleData=LI_PulseFlow_ML_DF; //单位“ML/脉冲” 小数点后四位
			break;
		case 1016:
			readSingleData=UI_C_MA_H1_D; //输入电流(模拟)
			break;
		case 1017:
			readSingleData=LI_LotVolume_DF/10; //批次容积 展示小数点后1位
			break;
		case 1018:
			readSingleData=UC_LotVolumeUnit_SF; //批次容积单位 1L 2ml
			break;
		case 1019:
			readSingleData=LI_ResidualFlow_ViewD/10; //剩余批次容积 展示小数点后1位
			break;
		case 1020:
			readSingleData=UC_AddTimeSec_DF/60; //批次投加 分钟
			break;
		case 1021:
			readSingleData=UC_AddTimeSec_DF%60; //批次投加 秒
			break;
		case 1022:
			readSingleData=LI_LoopLotVolume_DF/10; //循环批次容积  展示小数点后1位
			break;
		case 1023:
			readSingleData=UC_LoopLotVolumeUnit_SF; //循环批次容积单位
			break;
		case 1024:
			readSingleData=LI_LoopResidualFlow_ViewD/10; //剩余循环批次容积  展示小数点后1位
			break;
		case 1025:
			readSingleData=UC_LoopADDTime_DF/60; //循环批次投加 分钟
			break;
		case 1026:
			readSingleData=UC_LoopADDTime_DF%60; //循环批次投加 秒
			break;
		case 1027:
			readSingleData=UC_LoopCycleTime_DF/60; //循环批次投加周期  分钟
			break;
		case 1028:
			readSingleData=UC_LoopCycleTime_DF%60; //循环批次投加周期 秒
			break;
		case 1029:
			readSingleData=UC_LoopStartDelayTime_DF/60; //循环批次投加启动  分钟
			break;
		case 1030:
			readSingleData=UC_LoopStartDelayTime_DF%60; //循环批次投加启动 秒
			break;
		case 1031:
			readSingleData=UC_SignalFault_D; //报警状态
			break;
		case 1032:
			readSingleData=UC_ActiveMode_S; //激活状态
			break;
		
		
	}
	UI_BaudRateStatusIN_S=1; //进行读写状态
	UI_BaudRateStatusIN_C=0;
}

/*****************************************************************
485地址赋值
******************************************************************/
void setParameter(unsigned int par_address,unsigned int par_data)
{
	switch(par_address)
	{
		case 2000:
			if(UI_RemoteControl_SF==REMOTE_CLOSE_S) //运行状态  0: 待机 1：运行
			{
				if(par_data==1)
				{
					UC_RemoteStatusHL_S=REMOTE_NON_STOP;
				}
				else if(par_data==0)
				{
					UC_RemoteStatusHL_S=REMOTE_STOP;
				}
				else
				{
					UC_S_Para_Invalid_D=1;
				}
			}
			break;
		
		case 2001:
			if(par_data<=UI_RatedMAXFlow_DF) //手动模式目标流量 单位“L/H”，小数点后一位 要小于额定流量 200.0
			{
				LI_ManualFlow_L_DF=par_data;  //LI_ManualFlow_L_DF 手动流量小数点后四位 200.0*1000 
				LI_ManualFlow_L_DF=LI_ManualFlow_L_DF*1000;
			}
			else
			{
				UC_S_Para_Invalid_D=1;
			}
			break;
		case 2002:
			if(par_data<=(UI_RatedMAXFlow_DF/0.36)) //脉冲流量 2000/0.36   0.5555 脉冲每ml
			{
				LI_PulseFlow_ML_DF=par_data;  //LI_PulseFlow_ML_DF 脉冲小数点后四位 0.5555 最大值
			}
			else
			{
				UC_S_Para_Invalid_D=1;
			}
			break;
		case 2003:
			LI_LotVolume_DF=par_data;  //批次容积
			LI_LotVolume_DF=LI_LotVolume_DF*10;//实际小数点后两位 界面显示文档 小数点后1位
			break;
		case 2004:
			if(par_data==1||par_data==2)  //批次单位
			{
				UC_LotVolumeUnit_SF=par_data;
			}
			else
			{
				UC_S_Para_Invalid_D=1;
			}
			break;
		case 2005:
			UC_AddTimeSec_DF=par_data;
			UC_AddTimeSec_DF=UC_AddTimeSec_DF*60; // UC_AddTimeSec_DF long int 批次 投加时间 分钟 转成s
			break;
		case 2006:
			UC_AddTimeSec_DF=UC_AddTimeSec_DF+par_data; // UC_AddTimeSec_DF long int 批次 投加时间 秒 转成s
			break;
		case 2007:
			LI_LoopLotVolume_DF=par_data;  //循环投加容积
			LI_LotVolume_DF=LI_LoopLotVolume_DF*10;////循环投加容积
			break;
		case 2008:
			if(par_data==1||par_data==2)  //循环投加容积单位
			{
				UC_LoopLotVolumeUnit_SF=par_data;
			}
			else
			{
				UC_S_Para_Invalid_D=1;
			}
			break;
		case 2009:
			UC_LoopADDTime_DF=par_data;
			UC_LoopADDTime_DF=UC_LoopADDTime_DF*60; //循环投加时间 分钟 转成s
			break;
		case 2010:
			UC_LoopADDTime_DF=UC_LoopADDTime_DF+par_data; // UC_LoopADDTime_DF long int循环投加 投加时间 秒 转成s
			break;
		case 2011:
			UC_LoopCycleTime_DF=par_data;
			UC_LoopCycleTime_DF=UC_LoopCycleTime_DF*60; //循环投加周期 分钟 转成s
			break;
		case 2012:
			UC_LoopCycleTime_DF=UC_LoopCycleTime_DF+par_data; // UC_LoopCycleTime_DF long 循环投加周期 投加时间 秒 转成s
			break;
		case 2013:
			UC_LoopStartDelayTime_DF=par_data;
			UC_LoopStartDelayTime_DF=UC_LoopStartDelayTime_DF*60; //循环投加启动延时 分钟 转成s
			break;
		case 2014:
			UC_LoopStartDelayTime_DF=UC_LoopStartDelayTime_DF+par_data; // UC_LoopStartDelayTime_DF long 循环投加启动延时 投加时间 秒 转成s
			break;
	}
	UI_BaudRateStatusIN_S=1; //进行读写状态
	UI_BaudRateStatusIN_C=0;
}

/***********************************时钟芯片的写入和读取************************************************/
/*****************************************
* Function Name: WriteDATA 
* Description  : WriteDATA
* Arguments    : 写入时钟芯片
* Return Value : None
******************************************/
void Write1302(unsigned char addr,unsigned char dat)
{ 
	unsigned char i,temp;
	CE=0; 											
	SLK=0; 										
	CE = 1; 										
	for ( i=8; i>0; i-- ) 
	{
		SLK = 0;
		temp = addr;
		DIO = temp&0x01; 
		addr >>= 1; 
		SLK = 1;
	}
	for ( i=8; i>0; i-- )
	{
		SLK = 0;
		temp = dat;
		DIO = temp&0x01;
		dat >>= 1;
		SLK = 1;
	}
	CE = 0;
}

/*****************************************
* Function Name: Read1302 
* Description  : Read1302
* Arguments    : 读取时钟芯片
* Return Value : None
******************************************/
unsigned char Read1302 (unsigned char addr )
{
   	unsigned char i,temp,dat1,shiwei,gewei;
	CE=0;
	SLK=0;
	CE = 1;
	
	for ( i=8; i>0; i-- ) 
	{
		SLK = 0;
		temp = addr;
		DIO =temp&0x01;
		addr >>= 1; 
		SLK = 1;
	}
	R_PORT_P61_IN();
	for ( i=8; i>0; i-- )
	{
		ACC_Temp=DIO;
		if(DIO==0)
		{
			ACC_7=0x00;
		}
		else 
		{
			ACC_7=0x80;
		}
		ACC=ACC|ACC_7;
		SLK = 1;
		ACC>>=1;
		SLK = 0;
	}
	CE=0;
	dat1=ACC;
	
	shiwei=dat1>>4;
	gewei=dat1&0x0f;
	
	dat1=gewei+shiwei*10;
	
	R_PORT_P61_OUT();
	return (dat1);
}

/*****************************************
* Function Name: Initial 
* Description  : Initial
* Arguments    : 初始化时钟芯片
* Return Value : None
******************************************/
void Initial(void)
{
	Write1302(WRITE_PROTECT,0x00); 
	Write1302(WRITE_SECOND,0x00);
	Write1302(WRITE_PROTECT,0x80); 
}

void R_PORT_P61_IN(void)
{
   PM6 = _00_PMn0_MODE_OUTPUT | _02_PMn1_MODE_INPUT | _00_PMn2_MODE_OUTPUT | _08_PMn3_NOT_USE;
}

void R_PORT_P61_OUT(void)
{
   PM6 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _08_PMn3_NOT_USE;
}

/***********************************SET 输出模拟mA信息**************************************/
void Write_Date(unsigned int dat)
{
	unsigned char i,temp;
	
	nSYNC=0;
	
	for(i=0; i<8; i++)
	{
		SCLK=1;
		Data_IN_OUT=0x00;
		SCLK=0;
	}
	
	for(i=0; i<16; i++)
	{
		SCLK=1;
		temp = dat>>15;
		Data_IN_OUT=temp&0x01;
		dat<<=1;
		SCLK=0;
	}
		
	nSYNC=1;
}


/*********************************************I2C 存储************************************************************/
void Check_eeproom(void)				//写入eeproom，最多校验两次
{
	unsigned char check_count=0;
	while(check_count<3)
	{
		R_WP=0;
		Write_EEprom();
		delay_us(5000);
		eeprom_read(0x00);
		
		ULI_D_FlashTotal=0;
		for(UC_D_FlashCount=0;UC_D_FlashCount<150;UC_D_FlashCount++)		//计算读取24C02中的校验和 106要求要修改**bug**
		{
			UC_D_FlashTemp=UC_C_Read_eeprom_Data[UC_D_FlashCount];
			ULI_D_FlashTotal=ULI_D_FlashTotal+UC_D_FlashTemp;
		}
		
		// flash 校验和
		ULI_D_FlashTotalCRC=0;  //校验和
		for(UC_D_FlashCount=4;UC_D_FlashCount<154;UC_D_FlashCount++)   //校验和后期需要修改 现在是从4开始的
		{
			UC_D_FlashTemp=UC_F_FlashData_A[UC_D_FlashCount];
			ULI_D_FlashTotalCRC=ULI_D_FlashTotalCRC+UC_D_FlashTemp;
		}
		
		if(ULI_D_FlashTotal==ULI_D_FlashTotalCRC) //对比I2C校验和 flash 校验和
		{
			R_WP=1;
			return;
		}
		else
		{
			check_count++;
		}
	}
}

//写入eeproom，多页
void Write_EEprom(void)
{
	eeprom_write(0x00);
	eeprom_write(0x01);
	eeprom_write(0x02);
	eeprom_write(0x03);
	eeprom_write(0x04);
	eeprom_write(0x05);
	eeprom_write(0x06);
	eeprom_write(0x07);
	eeprom_write(0x08);
	eeprom_write(0x09);
	eeprom_write(0x0A);
	eeprom_write(0x0B);
	eeprom_write(0x0C);
	eeprom_write(0x0D);
	eeprom_write(0x0E);
	eeprom_write(0x0F);
	eeprom_write(0x10);
	eeprom_write(0x11);
	eeprom_write(0x12);
}

void eeprom_write(unsigned char page)  //分页写入
{
	i2c_start();
    	i2c_write(0xa0);    // 写入器件地址
	switch(page)
	{
		case 0:
		    i2c_write(0x00);    // 写入存储地址
		    i2c_write(UC_ModeRun_SF);                   // UC_F_FlashData_A[4]
		    i2c_write((unsigned char)((LI_ManualFlow_L_DF >>24)&0xFF));  // [5]
		    i2c_write((unsigned char)((LI_ManualFlow_L_DF >>16)&0xFF));  // [6]
		    i2c_write((unsigned char)((LI_ManualFlow_L_DF >>8)&0xFF));   // [7]
		    i2c_write((unsigned char)(LI_ManualFlow_L_DF&0xFF));         // [8]
		    i2c_write((unsigned char)((LI_PulseFlow_ML_DF >>24)&0xFF));   // [9]
		    i2c_write((unsigned char)((LI_PulseFlow_ML_DF >>16)&0xFF));   // [10]
		    i2c_write((unsigned char)((LI_PulseFlow_ML_DF >>8)&0xFF));    // [11]
		break;

		case 1:
		    i2c_write(0x08);    // 写入存储地址
		    i2c_write((unsigned char)(LI_PulseFlow_ML_DF&0xFF));         // [12]
		    //i2c_write(UC_PulseMemory_SF);                                // [13]
		    i2c_write(UC_BatchManual_SF);     
		    i2c_write(UC_04_20MAType_SF);                               // [14]
		    i2c_write((unsigned char)((UC_04_20MA_I1_DF >>8)&0xFF));     // [15]
		    i2c_write((unsigned char)(UC_04_20MA_I1_DF&0xFF));           // [16]
		    i2c_write((unsigned char)((UC_04_20MA_Q1_DF >>8)&0xFF));     // [17]
		    i2c_write((unsigned char)(UC_04_20MA_Q1_DF&0xFF));           // [18]
		    i2c_write((unsigned char)((UC_04_20MA_I2_DF >>8)&0xFF));    // [19]
		break;

		case 2:
		    i2c_write(0x10);    // 写入存储地址
		    i2c_write((unsigned char)(UC_04_20MA_I2_DF&0xFF));           // [20]
		    i2c_write((unsigned char)((UC_04_20MA_Q2_DF >>8)&0xFF));     // [21]
		    i2c_write((unsigned char)(UC_04_20MA_Q2_DF&0xFF));           // [22]
		    i2c_write((unsigned char)((LI_LotVolume_DF >>24)&0xFF));     // [23]
		    i2c_write((unsigned char)((LI_LotVolume_DF >>16)&0xFF));     // [24]
		    i2c_write((unsigned char)((LI_LotVolume_DF >>8)&0xFF));      // [25]
		    i2c_write((unsigned char)(LI_LotVolume_DF&0xFF));             // [26]
		    i2c_write(UC_LotVolumeUnit_SF);                              // [27]
		break;

		case 3:
		    i2c_write(0x18);    // 写入存储地址
		    i2c_write((unsigned char)((UC_AddTimeSec_DF >>24)&0xFF));    // [28]
		    i2c_write((unsigned char)((UC_AddTimeSec_DF >>16)&0xFF));    // [29]
		    i2c_write((unsigned char)((UC_AddTimeSec_DF >>8)&0xFF));     // [30]
		    i2c_write((unsigned char)(UC_AddTimeSec_DF&0xFF));            // [31]
		    i2c_write((unsigned char)((LI_LoopLotVolume_DF >>24)&0xFF)); // [32]
		    i2c_write((unsigned char)((LI_LoopLotVolume_DF >>16)&0xFF)); // [33]
		    i2c_write((unsigned char)((LI_LoopLotVolume_DF >>8)&0xFF));  // [34]
		    i2c_write((unsigned char)(LI_LoopLotVolume_DF&0xFF));         // [35]
		break;

		case 4:
		    i2c_write(0x20);    // 写入存储地址
		    i2c_write(UC_LoopLotVolumeUnit_SF);                          // [36]
		    i2c_write((unsigned char)((UC_LoopADDTime_DF >>24)&0xFF));   // [37]
		    i2c_write((unsigned char)((UC_LoopADDTime_DF >>16)&0xFF));   // [38]
		    i2c_write((unsigned char)((UC_LoopADDTime_DF >>8)&0xFF));    // [39]
		    i2c_write((unsigned char)(UC_LoopADDTime_DF&0xFF));          // [40]
		    i2c_write((unsigned char)((UC_LoopCycleTime_DF >>24)&0xFF)); // [41]
		    i2c_write((unsigned char)((UC_LoopCycleTime_DF >>16)&0xFF));// [42]
		    i2c_write((unsigned char)((UC_LoopCycleTime_DF >>8)&0xFF)); // [43]
		break;

		case 5:
		    i2c_write(0x28);    // 写入存储地址
		    i2c_write((unsigned char)(UC_LoopCycleTime_DF&0xFF));        // [44]
		    i2c_write((unsigned char)((UC_LoopStartDelayTime_DF >>24)&0xFF)); // [45]
		    i2c_write((unsigned char)((UC_LoopStartDelayTime_DF >>16)&0xFF));// [46]
		    i2c_write((unsigned char)((UC_LoopStartDelayTime_DF >>8)&0xFF)); // [47]
		    i2c_write((unsigned char)(UC_LoopStartDelayTime_DF&0xFF));       // [48]
		    i2c_write(UC_IsCalibrated_DF);                              // [49]
		    i2c_write((unsigned char)((UI_CalibrateVolML_H1_DF >>24)&0xFF));// [50]
		    i2c_write((unsigned char)((UI_CalibrateVolML_H1_DF >>16)&0xFF));// [51]
		break;

		case 6:
		    i2c_write(0x30);    // 写入存储地址
		    i2c_write((unsigned char)((UI_CalibrateVolML_H1_DF >>8)&0xFF)); // [52]
		    i2c_write((unsigned char)(UI_CalibrateVolML_H1_DF&0xFF));      // [53]
		    i2c_write((unsigned char)((UI_UserCalibrateVolML_H1_DF >>24)&0xFF)); // [54]
		    i2c_write((unsigned char)((UI_UserCalibrateVolML_H1_DF >>16)&0xFF));// [55]
		    i2c_write((unsigned char)((UI_UserCalibrateVolML_H1_DF >>8)&0xFF)); // [56]
		    i2c_write((unsigned char)(UI_UserCalibrateVolML_H1_DF&0xFF));       // [57]
		    i2c_write((unsigned char)((UI_MaxFlowML_H2_DF >>24)&0xFF));    // [58]
		    i2c_write((unsigned char)((UI_MaxFlowML_H2_DF >>16)&0xFF));    // [59]
		break;

		case 7:
		    i2c_write(0x38);    // 写入存储地址
		    i2c_write((unsigned char)((UI_MaxFlowML_H2_DF >>8)&0xFF));    // [60]
		    i2c_write((unsigned char)(UI_MaxFlowML_H2_DF&0xFF));          // [61]
		    i2c_write((unsigned char)((UI_C_SetMinAdValue4_DF >>8)&0xFF));// [62]
		    i2c_write((unsigned char)(UI_C_SetMinAdValue4_DF&0xFF));      // [63]
		    i2c_write((unsigned char)((UI_C_SetMaxAdValue20_DF >>8)&0xFF));// [64]
		    i2c_write((unsigned char)(UI_C_SetMaxAdValue20_DF&0xFF));     // [65]
		    i2c_write((unsigned char)((UI_4_20MA_AD_K_DF >>8)&0xFF));     // [66]
		    i2c_write((unsigned char)(UI_4_20MA_AD_K_DF&0xFF));          // [67]
		break;

		case 8:
		    i2c_write(0x40);    // 写入存储地址
		    i2c_write((unsigned char)((UI_4_20MA_AD_B_DF >>8)&0xFF));     // [68]
		    i2c_write((unsigned char)(UI_4_20MA_AD_B_DF&0xFF));          // [69]
		    i2c_write(UI_AddSub_B_DF);                                  // [70]
		    i2c_write(UI_BaudRateStatus_SF);                            // [71]
		    i2c_write((unsigned char)((UI_CalibParam_MA_DF >>8)&0xFF)); // [72]
		    i2c_write((unsigned char)(UI_CalibParam_MA_DF&0xFF));       // [73]
		    i2c_write(UI_SlowModeStatus_SF);                            // [74]
		    i2c_write(UI_IsPessureSwitch_SF);                            // [75] 压力快关
		break;

		case 9:
		    i2c_write(0x48);    // 写入存储地址
		    i2c_write((unsigned char)((UI_PessureRange_DF >>8)&0xFF));  // [76]
		    i2c_write((unsigned char)(UI_PessureRange_DF&0xFF));         // [77]
		    i2c_write((unsigned char)((UI_PessureHigh_DF >>8)&0xFF));    // [78]
		    i2c_write((unsigned char)(UI_PessureHigh_DF&0xFF));          // [79]
		    i2c_write((unsigned char)((UI_PessureLow_DF >>8)&0xFF));     // [80]
		    i2c_write((unsigned char)(UI_PessureLow_DF&0xFF));           // [81]
		    i2c_write(UC_PowerOffLock_SF);    // [82]//断电停机
		    i2c_write((unsigned char)(UI_IsLevelSwitch_SF&0xFF));           // [83] //液位报警开关
		break;

		case 10:
		    i2c_write(0x50);    // 写入存储地址
		    i2c_write((unsigned char)((UI_LevelHigh_DF >>8)&0xFF));      // [84]
		    i2c_write((unsigned char)(UI_LevelHigh_DF&0xFF));            // [85]
		    i2c_write((unsigned char)((UI_LevelLow_DF >>8)&0xFF));       // [86]
		    i2c_write((unsigned char)(UI_LevelLow_DF&0xFF));             // [87]
		    i2c_write(UI_RemoteControl_SF);                             // [88]
		    i2c_write(UI_RunRelayOutput_SF);                            // [89]
		    i2c_write(UI_FaultyRelayOutput_SF);                         // [90]
		    i2c_write(UI_ScreenSaver_SF);                               // [91]
		break;

		case 11:
		    i2c_write(0x58);    // 写入存储地址
		    i2c_write((unsigned char)((UI_ScreenSaverTime_DF >>8)&0xFF)); // [92]
		    i2c_write((unsigned char)(UI_ScreenSaverTime_DF&0xFF));      // [93]
		    i2c_write(UI_MachineCode_DF);                               // [94]
		    i2c_write(UI_LanguageSet_SF);                               // [95]
		    i2c_write((unsigned char)((UI_RatedMAXFlow_DF >>8)&0xFF));     // [96]
		    i2c_write((unsigned char)(UI_RatedMAXFlow_DF&0xFF));            // [97]
		    i2c_write((unsigned char)((LI_RatedMAXPulseWidth_DF >>8)&0xFF));// [98]
		    i2c_write((unsigned char)(LI_RatedMAXPulseWidth_DF&0xFF));      // [99]
		break;

		case 12:
		    i2c_write(0x60);    // 写入存储地址
		    i2c_write(UC_PWDUser_A[0]);   // [100]
		    i2c_write(UC_PWDUser_A[1]);   // [101]
		    i2c_write(UC_PWDUser_A[2]);   // [102]
		    i2c_write(UC_PWDUser_A[3]);   // [103]
		    i2c_write(UC_PWDUser_A[4]);   // [104]
		    i2c_write(UC_PWDUser_A[5]);   // [105]
		    i2c_write(UC_PWDUser_A[6]);   // [106]
		    i2c_write(UC_PWDUser_A[7]);   // [107]
		break;

		case 13:
		    i2c_write(0x68);    // 写入存储地址
		    i2c_write(UC_PWDFactory_A[0]); // [108]
		    i2c_write(UC_PWDFactory_A[1]); // [109]
		    i2c_write(UC_PWDFactory_A[2]); // [110]
		    i2c_write(UC_PWDFactory_A[3]); // [111]
		    i2c_write(UC_PWDFactory_A[4]); // [112]
		    i2c_write(UC_PWDFactory_A[5]); // [113]
		    i2c_write(UC_PWDFactory_A[6]); // [114]
		    i2c_write(UC_PWDFactory_A[7]); // [115]
		break;
		case 14:
		    i2c_write(0x70);    // 写入存储地址 0x70（对应数据索引 116-123）
		    i2c_write(UC_DeviceId_A[0]);   // [116]
		    i2c_write(UC_DeviceId_A[1]);   // [117]
		    i2c_write(UC_DeviceId_A[2]);   // [118]
		    i2c_write(UC_DeviceId_A[3]);   // [119]
		    i2c_write(UC_DeviceId_A[4]);   // [120]
		    i2c_write(UC_DeviceId_A[5]);   // [121]
		    i2c_write(UC_DeviceId_A[6]);   // [122]
		    i2c_write(UC_DeviceId_A[7]);   // [123]
		 break;
		case 15:
		    i2c_write(0x78);    
		    i2c_write((unsigned char)((LI_ResidualFlow_D >>24)&0xFF));
		    i2c_write((unsigned char)((LI_ResidualFlow_D >>16)&0xFF));
		    i2c_write((unsigned char)((LI_ResidualFlow_D >>8)&0xFF));
		    i2c_write((unsigned char)(LI_ResidualFlow_D&0xFF));  
		    i2c_write((unsigned char)((UI_CountSeconds_D>>8)&0xFF));   
		    i2c_write((unsigned char)(UI_CountSeconds_D&0xFF));  
		    i2c_write(UC_IsPulseLaunch_S); 
		    i2c_write(UC_C_OnOff_S); //开关状态
		 break;
		case 16:
		    i2c_write(0x80);    
		    i2c_write((unsigned char)(UC_IsDelayedLaunch_S)); //循环运行
		    i2c_write((unsigned char)((UC_IsDelayedLaunch_C >>8)&0xFF)); //计时
		    i2c_write((unsigned char)((UC_IsDelayedLaunch_C )&0xFF)); //计时
		    i2c_write((unsigned char)((LI_LoopResidualCycleTime_D >>24)&0xFF));
		    i2c_write((unsigned char)((LI_LoopResidualCycleTime_D >>16)&0xFF));
		    i2c_write((unsigned char)((LI_LoopResidualCycleTime_D >>8)&0xFF));
		    i2c_write((unsigned char)(LI_LoopResidualCycleTime_D&0xFF));  
		    i2c_write((unsigned char)((UC_LaunchTime_C >>8)&0xFF));
		 break;
		case 17:
		    i2c_write(0x88);   
		    i2c_write((unsigned char)(UC_LaunchTime_C&0xFF));
		    i2c_write((unsigned char)((LI_LoopResidualFlow_D >>24)&0xFF));
		    i2c_write((unsigned char)((LI_LoopResidualFlow_D >>16)&0xFF));
		    i2c_write((unsigned char)((LI_LoopResidualFlow_D >>8)&0xFF));
		    i2c_write((unsigned char)(LI_LoopResidualFlow_D&0xFF));  
		    i2c_write((unsigned char)((UI_FactoryRatedMAXFlow_DF >>8)&0xFF));
		    i2c_write((unsigned char)(UI_FactoryRatedMAXFlow_DF&0xFF)); 
		    i2c_write((unsigned char)((UI_UserRatedMAXFlow_DF >>8)&0xFF));
		 break;
		case 18:
		    i2c_write(0x90);   
		    i2c_write((unsigned char)(UI_UserRatedMAXFlow_DF&0xFF));
		    i2c_write(UC_C_OnOffStatus_S); //开关机状态
		    i2c_write((unsigned char)((UI_FlowInM3_DF>>8)&0xFF));
		    i2c_write((unsigned char)(UI_FlowInM3_DF&0xFF)); //累计立方
		    i2c_write((unsigned char)((UI_TotalFlowL_H1_DF>>8)&0xFF));
		    i2c_write((unsigned char)(UI_TotalFlowL_H1_DF&0xFF)); //累计流量
		    i2c_write(0);
		    i2c_write(0);
		 break;
	}
	
	i2c_stop();
     	delay_us(5000);
}


/*****************************************
* Function Name: FunctionalWait  
* Description  : 模式的功能待机 --比如脉冲模式 无脉冲输入  4-20Ma 电流小于4ma 
* Arguments    : None  
* Return Value : None  
******************************************/
void FunctionalWait(void)
{
	if(UC_C_OnOff_S==TRUE)
	{
		UC_C_OnOffStatus_S=WAIT;//功能暂停
	}
}

/*****************************************
* Function Name: RuningOrRemote  
* Description  : 正常运行和外部停机的逻辑处理
* Arguments    : None  
* Return Value : None  
******************************************/
void RuningOrRemote(void)
{
	if(UC_C_OnOffStatus_S==TRUE&&UC_RemoteStatusHL_S==REMOTE_STOP)//返回外部停机(远程)状态  1 暂时停机 0 不停机
	{
		UC_C_OnOffStatus_S=WAIT; //外部暂停
	}
	else if(UC_C_OnOffStatus_S==WAIT&&UC_RemoteStatusHL_S==REMOTE_NON_STOP) //远程不停机
	{
		//报警状态 设备部启动
		if(UC_LevelKTStatusHL_S!=KT_ALARM&&UC_PressureLevelAlarm_S!=OVER_Voltage&&UC_C_PumpAlarm_S!=PUMP_ERROR&&UC_4_20MAAlarmS_S!=MA4_20_ERROR&&UC_DiaphragmAlarm_S!=DIA_ERROR)
		{
			UC_C_OnOffStatus_S=TRUE;//外部启动
		}
	}
}
//清除一些异常的状态和数据
void ClearStatusAndData(void)
{
	ULI_C_PulseCallBack_C=0; //电机反馈霍尔
}
/* End user code. Do not edit comment generated here */
