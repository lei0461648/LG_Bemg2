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
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G13 V2.03.04.01 [11 Nov 2016]
* Device(s)    : R5F100FE
* Tool-Chain   : CCRL
* Description  : This file implements device driver for TAU module.
* Creation Date: 2025-04-02
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "var_define.h"	
#include "m_lot_4.h"	
#include "m_loop_5.h"	
#include "UART_SET.h"
#include "get_screen.h"
#include "set_other.h"
#include "b_basic.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_tau0_channel0_interrupt(vect=INTTM00)
#pragma interrupt r_tau0_channel2_interrupt(vect=INTTM02)
#pragma interrupt r_tau0_channel3_interrupt(vect=INTTM03)
#pragma interrupt r_tau0_channel5_interrupt(vect=INTTM05)
#pragma interrupt r_tau0_channel6_interrupt(vect=INTTM06)
/* Start user code for pragma. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* For TAU0_ch3 pulse measurement */
volatile uint32_t g_tau0_ch3_width = 0UL;
/* Start user code for global. Do not edit comment generated here */

//时间
extern unsigned char Time_Second;
extern unsigned char Time_Minute;
extern unsigned char Time_Hour;
extern unsigned char Time_Day;
extern unsigned char Time_Month;
extern unsigned char Time_Year;

//4-20ma 延时
extern unsigned int UI_AD_4_20MA_Delay_C;

extern unsigned char UC_C_WriteFlash_S;

extern unsigned char getadv_flag;
extern unsigned int UI_C_Time_Delay;
extern unsigned char UC_RuningCal_Delay_C;
unsigned char UC_C_OutPutMA_Count;

unsigned char UC_GetPessureDelay_C; //获取延时压力
/********************PULSE************************/
unsigned char UC_PulseTrigger_S;

/********************temp param************************/
unsigned char UC_RemoteHL_CT;
unsigned char UC_LevelKTHL_CT;

/********************extern param************************/
extern unsigned char UI_RemoteControl_SF;
unsigned char UC_RemoteStatusHL_S;
unsigned char UC_LevelKTStatusHL_S;
extern unsigned char  UC_DelayWriteFlash_S;
extern unsigned char UC_ModeRun_SF; //工作模式
extern unsigned int UI_IsPessureSwitch_SF; //压力 开关0x55 0xAA 
extern unsigned int UI_IsLevelSwitch_SF; //液位开关量 开关量 0x55 关 0xAA开 
extern unsigned int UI_BaudRateStatus_SF; //485 开关
extern unsigned char UC_DiaphragmDelay_S;//隔膜报警运行
extern unsigned char UC_DiaphragmDelay_C;//隔膜报警计数

/********************time ************************/
unsigned char DELAY_COUNT_M; //time delay
unsigned char UC_Time5_S; //定时器5


/**********************防抖******************************/
extern unsigned char UC_IntiHigh_S;//进入高中断
extern unsigned char UC_IntiLow_S;//进入低中断
unsigned int UI_Shaking_C;//抖动计数
unsigned int UI_ShakingERROR_S;//抖动通过
unsigned int UI_Shaking_D;//抖动计数
extern unsigned int UI_PulseWidth_D;//设置脉宽

/**********************累计流量******************************/
unsigned char UI_TotalFlowAdd_S; //累计流量开始添加状态
unsigned char UI_TotalFlowAdd_C; //累计流量计数
extern unsigned char UC_C_OnOffStatus_S;
unsigned char UC_UpdateSecondly_C=0;//10s 更新一次流量
extern unsigned char UC_SignalFault_D; //返回的报警参数

/**********************上电延时******************************/
unsigned char UC_ProwOnDelay_D=0; //上电延时
unsigned char UC_ProwOnDelay_S=0; //上电延时状态
extern unsigned char UC_ServerDelay_C; //开启服务延时

/**********************485通信状态******************************/
extern unsigned char UI_BaudRateStatusIN_S;//进行485读写
extern unsigned char UI_BaudRateStatusIN_C;//进行485读写计数清除

/**********************平滑减速******************************/
extern unsigned char PluseChange_S;//是否改变脉宽
extern unsigned int AtPresentPluse;//当前脉冲
extern unsigned int TDR07_Percent_C;
unsigned int UI_PulseWidth_Temp;//脉冲临时

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    
   //上电初始化 R_MAIN_UserInit 延时
   if(UI_C_Time_Delay>0)
    {
   	 UI_C_Time_Delay--;
    }

    // 4-20MA 采样 延时  当前为4-20ma 工作模式
    if((UC_ModeRun_SF==MA_0_20_MODE)&&(UI_AD_4_20MA_Delay_C<ADC_4_20MA_PERIOD))			//Get a 4-20ma delay
    {
    	UI_AD_4_20MA_Delay_C++;
    }
    
    //PWM 脉宽变成 延时
    if(UC_RuningCal_Delay_C<RUNINGCAL_PERIOD)			//Run mode delay
    {
    	UC_RuningCal_Delay_C++;
    }
    
    //输出 4-20 MA 计数延时
    if(UC_C_OutPutMA_Count<=OUTPUT_MA_PERIOD)		//The analog traffic display is updated every 2 seconds
    {
	UC_C_OutPutMA_Count++;
    }
    
    //4-20MA 压力AD 延时 1s 当前开启压力开关
    if((UI_IsPessureSwitch_SF==TRUE)&&(UC_GetPessureDelay_C<=ADC_4_20MA_YL))		//The analog traffic display is updated every 2 seconds
    {
	UC_GetPessureDelay_C++;
    }
    
    //读取时钟芯片的延时
    if(DELAY_COUNT_M<25)
    {
    	DELAY_COUNT_M++;
    }
    
    //隔膜报警AD采样延时
    if(UC_DiaphragmDelay_S==0)
    {
	    if(++UC_DiaphragmDelay_C>150)//延时600ms
	    {
		    UC_DiaphragmDelay_C=0;
		    UC_DiaphragmDelay_S=1;    
	    }
    }
    
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    
    /**********************外部开关防抖处理*******************************/
     if(UI_RemoteControl_SF==REMOTE_OPEN_S)
     {
	if(REMOTE_SWITCH_PIN==HI&&UC_RemoteStatusHL_S==REMOTE_STOP)  //控制开关打开，线路闭合 高电平，开机
	{
		if(++UC_RemoteHL_CT>=5)
		{
			UC_RemoteHL_CT=0;
			UC_RemoteStatusHL_S=REMOTE_NON_STOP; //返回外部停机(远程)状态  0 不停机
		}
	}
	else if(REMOTE_SWITCH_PIN==LO)  //控制开关打开，线路断开 低电平，关机
	{
		if(++UC_RemoteHL_CT>=5)
		{
			UC_RemoteHL_CT=0;
			UC_RemoteStatusHL_S=REMOTE_STOP;
		}
	}
     }
     else 
     {
	     /*远程停机的 优先级"外部开关停机"大于"485开关机"，
     		*/
	     if(UI_BaudRateStatus_SF==0)
	     {
		     UC_RemoteStatusHL_S=REMOTE_NON_STOP;
	     }
     	
     }
     
     /************************空桶低液位防抖处理*****************************/
     if(UI_IsLevelSwitch_SF==LEVEL_OPEN_S)
     {
	if(KT_PIN==HI&&UC_LevelKTStatusHL_S==KT_NON_ALARM)
	{
		if(++UC_LevelKTHL_CT>=5)
		{
			UC_LevelKTHL_CT=0;
			UC_LevelKTStatusHL_S=KT_ALARM; //KT 液位空桶报警
		}
	}
	else if(KT_PIN==LO)
	{
		if(++UC_LevelKTHL_CT>=5)
		{
			UC_LevelKTHL_CT=0;
			UC_LevelKTStatusHL_S=KT_NON_ALARM; //KT 不报警
		}
	}
     }
     else 
     {
     	UC_LevelKTStatusHL_S=KT_NON_ALARM;//KT 不报警
     }
     /************************处理掉电记忆***********bug 仿真执行了 先关闭******************/
     PowerOffMemory();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel3_interrupt
* Description  : This function is INTTM03 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel3_interrupt(void)
{
    if ((TSR03 & _0001_TAU_OVERFLOW_OCCURS) == 1U)    /* overflow occurs */
    {            
        g_tau0_ch3_width = (uint32_t)(TDR03 + 1UL) + 0x10000UL;
    }
    else
    {
        g_tau0_ch3_width = (uint32_t)(TDR03 + 1UL);
    }

    /* Start user code. Do not edit comment generated here */
    UC_PulseTrigger_S=1;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel5_interrupt
* Description  : This function is INTTM05 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel5_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    UC_Time5_S=1;
    
    //上电延时
    if(UC_ProwOnDelay_S==0)
    {
	    //--------------------bug 测试 2s 正常32
	    if(++UC_ProwOnDelay_D>=32)
	    //if(++UC_ProwOnDelay_D>=2)
	    {
		    UC_ProwOnDelay_S=0xAA;
	    } 
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel6_interrupt
* Description  : This function is INTTM06 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel6_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
   getadv_flag=1;
   
   //防抖计算
   if(UC_IntiHigh_S==1&&UC_IntiLow_S==0)
   {
	   UI_Shaking_C++;
   }
   else if(UC_IntiHigh_S==0&&UC_IntiLow_S==1)
   {
	   UI_Shaking_C++;
   }
   else if(UC_IntiHigh_S==1&&UC_IntiLow_S==1)
   {
	   if(UI_PulseWidth_D>1000)
	   {
		   UI_Shaking_D=50;
	   }
	   else if(UI_PulseWidth_D<=1000)
	   {
		   UI_Shaking_D=100;
	   }
	   
	   if(UI_Shaking_C<UI_Shaking_D)
	   {
		   UI_ShakingERROR_S=1;
	   }
	   else 
	   {
		   UI_ShakingERROR_S=2;
	   }
	   // 清零计数，准备下次检测
	   UI_Shaking_C = 0;
	   UC_IntiHigh_S = 0;
	   UC_IntiLow_S = 0;
   }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/**************************************定时器5 1000ms方法**********************************************/
void T_Channel5_Function(void)
{
    //延时保存flash
    if(UC_DelayWriteFlash_S==1)
    {
	  DelayWriteFlash();
    }
    
    //mode-4****************bug 现在远程开关跟暂停状态冲突先注释
    if(UC_ModeRun_SF==BATCH_MODE&&UC_SignalFault_D==0x00)//无报警状态
    {
	   get_ResidualFlow(); 
    }
    
    //mode-5 
    if(UC_ModeRun_SF==CYCLIC_MODE&&UC_SignalFault_D==0x00)//无报警状态
    {
    	get_LoopRun();   
    }
    
    
    //定时清除霍尔反馈信息--用来判断电机停机故障
    ClearHRPulse();
    
    //检测电机是否异常停机无反馈
    CheckPumpFault();
    
    //返回报警状态
    EnableRelayOnAlarm();

    //激活功能
    ActiveMode();    
    
    //光电异常检测
    GDCheck();
    
    //开机状态累计流量
    if(UC_C_OnOffStatus_S==TRUE)
    {
	    if(++UI_TotalFlowAdd_C>=TOTALFLOW_S)//30s 增加一次累计流量
	    {
		    UI_TotalFlowAdd_C=0;
		    UI_TotalFlowAdd_S=1;
	    }
	    // 累加每10秒的0.1毫升流量
 	    if(++UC_UpdateSecondly_C>=10)//10s 增加一次累计流量
	    {
		   update_secondly_flow();
		   UC_UpdateSecondly_C=0;
	    }
    }
    else
    {
	    UI_TotalFlowAdd_C=0;
	    UI_TotalFlowAdd_S=0;
    }
    
    
    //延时服务
    if(UC_ServerDelay_C>0)
    {
	    UC_ServerDelay_C--;
    }
    
     //485 通信状态
     if(UI_BaudRateStatusIN_S==1)
     {
	     if(++UI_BaudRateStatusIN_C>5)
	     {
		     UI_BaudRateStatusIN_C=0;
		     UI_BaudRateStatusIN_S=0;
	     }
     }
    else
    {
	     UI_BaudRateStatusIN_C=0;
    }
    
    /**********************减速要平滑***********************************/
    if(PluseChange_S==1)
    {
	    if(AtPresentPluse>1000) //当前脉宽小于1000 每次减的系数
	    {
		    UI_PulseWidth_Temp=AtPresentPluse-1000;
		    
		    if(UI_PulseWidth_Temp>TDR07_Percent_C)//当前脉宽大于新脉宽
		    {
			   UI_PulseWidth_D=AtPresentPluse-1000;
			   setPWMPulseWidth();//设置脉宽宽度
		    }
		    else
		    {
			    /***********最终数据**************/
			    UI_PulseWidth_D=TDR07_Percent_C;
			    setPWMPulseWidth();//设置脉宽宽度
			    PluseChange_S=0;
		    }
	    }
	    else
	    {
		    /***********最终数据**************/
		    UI_PulseWidth_D=TDR07_Percent_C;
		    setPWMPulseWidth();//设置脉宽宽度
		    PluseChange_S=0;
	    }
    }
}

/* End user code. Do not edit comment generated here */
