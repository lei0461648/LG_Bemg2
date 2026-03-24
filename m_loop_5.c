#include "r_cg_macrodriver.h"
#include "r_cg_wdt.h"
#include "m_lot_4.h"
#include "m_loop_5.h"
#include "get_screen.h"
#include "b_basic.h"
#include "var_define.h"
#include "r_cg_userdefine.h"

extern unsigned long int LI_LoopLotVolume_DF;
extern unsigned char UC_LoopLotVolumeUnit_SF;
extern unsigned long int UC_LoopADDTime_DF;
extern unsigned long int UC_LoopCycleTime_DF;
extern unsigned long int UC_LoopStartDelayTime_DF;

/************Temp extern*************/
extern unsigned long int LI_TimeINFlowML_H2_D;
extern unsigned long long int LLI_TimeINFlowML_H2_Temp;
extern unsigned char UC_ModeRun_SF;
extern unsigned char UC_C_OnOff_S;
extern unsigned char UC_C_OnOffStatus_S;

/************Comm*************/
unsigned char UC_IsDelayedLaunch_S=0;
unsigned int UC_IsDelayedLaunch_C=0;
unsigned int UC_LaunchTime_C=0;
unsigned long int LI_LoopResidualFlow_D;
unsigned long int LI_LoopResidualCycleTime_D;

extern unsigned char UC_RemoteStatusHL_S;
extern unsigned int UI_Percent_H2_DF; //当前百分比
extern unsigned long int LI_TargetFlowML_H2_DF; //目标流量


/************************************************
* Function Name: void m_RunLot(void)  
* Description  : Run Lot Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void m_RunLoop(void)
{
	get_LoopTargetFlow();
}



/************************************************
* Function Name: void get_LotTargetFlow(void)  
* Description  : Run Lot Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void get_LoopTargetFlow(void)
{
	if(UC_LoopLotVolumeUnit_SF==U_L)
	{
		LI_TimeINFlowML_H2_D=LI_LoopLotVolume_DF*1000;
	}
	else if(UC_LoopLotVolumeUnit_SF==U_ML)
	{
		LI_TimeINFlowML_H2_D=LI_LoopLotVolume_DF;
	}
	//LI_TimeINFlowML_H2_D=(LI_TimeINFlowML_H2_D*3600)/UC_LoopADDTime_DF;
	get_LoopPercent();
}

/************************************************
* Function Name: void get_SecForAddTimeSec(void)  
* Description  : Run Lot Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void get_LoopPercent(void)
{
	LLI_TimeINFlowML_H2_Temp=LI_TimeINFlowML_H2_D;
	LLI_TimeINFlowML_H2_Temp=(LLI_TimeINFlowML_H2_Temp*3600)/UC_LoopADDTime_DF;
	LI_TargetFlowML_H2_DF=LLI_TimeINFlowML_H2_Temp; //赋值目标流量
	get_Percent(LLI_TimeINFlowML_H2_Temp);
	getPWMForPercent(UI_Percent_H2_DF);
}

/************************************************
* Function Name: void LoopRun(void)  
* Description  : Run Lot Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void get_LoopRun(void)  //1s Timer 
{
	unsigned long long int LLI_ML_H2_Temp;
	
	if(UC_ModeRun_SF==CYCLIC_MODE&&UC_C_OnOff_S==TRUE&&UC_RemoteStatusHL_S==REMOTE_NON_STOP)
	{
		if(UC_IsDelayedLaunch_S==0)//NO Launch 处于等待还是运行
		{
			//--------------------------------------------控制泵开关逻辑-----等待-----------------------------------------------------------------
			FunctionalWait();
			//运行多久延时时间
			if(++UC_IsDelayedLaunch_C>=UC_LoopStartDelayTime_DF)
			{
				UC_IsDelayedLaunch_S=1;
				UC_IsDelayedLaunch_C=0;
				//--------------------------------------------控制泵开关逻辑---运行-远程-----------------------------------------------------------------
				RuningOrRemote();
			}
			//Minus start-up time   总周期倒计时
			if(LI_LoopResidualCycleTime_D>0)
			{
				LI_LoopResidualCycleTime_D=LI_LoopResidualCycleTime_D-1;	
			}
		}
		else if(UC_IsDelayedLaunch_S==1) //Launch  循环投加计时
		{
			//运行时间小于等于投加时间
			if(++UC_LaunchTime_C<UC_LoopADDTime_DF)
			{
				if(UC_C_OnOffStatus_S==WAIT)
				{
					UC_C_OnOffStatus_S=TRUE;
				}
				LLI_ML_H2_Temp=LLI_TimeINFlowML_H2_Temp/3600*UC_LaunchTime_C;
				if(LI_TimeINFlowML_H2_D>LLI_ML_H2_Temp)
				{
					LI_LoopResidualFlow_D=LI_TimeINFlowML_H2_D-LLI_ML_H2_Temp;
				}
				else 
				{
					LI_LoopResidualFlow_D=0;
				}
				
				//--------------------------------------------控制泵开关逻辑---运行-远程-----------------------------------------------------------------
				RuningOrRemote();
			}
			else 
			{
				LI_LoopResidualFlow_D=0;
				
				//Reassign the remaining traffic
				if(UC_LoopLotVolumeUnit_SF==U_L)
				{
					LI_LoopResidualFlow_D=LI_LoopLotVolume_DF*1000;
				}
				else if(UC_LoopLotVolumeUnit_SF==U_ML)
				{
					LI_LoopResidualFlow_D=LI_LoopLotVolume_DF;
				}
				
				//--------------------------------------------控制泵开关逻辑-----一次批次运行完毕 开始等待-----------------------------------------------------------------
				FunctionalWait();
			}
			
			if(UC_LaunchTime_C>=UC_LoopCycleTime_DF)
			{
				UC_LaunchTime_C=0;
			}
			
			//Cycle time minus running time equals remaining time
			//循环时间减去运行时间等于剩余时间
			if(UC_LoopCycleTime_DF>=UC_LaunchTime_C)
			{
				LI_LoopResidualCycleTime_D=UC_LoopCycleTime_DF-UC_LaunchTime_C;
			}
		}
	}
	else
	{
		//---控制泵开关逻辑---运行-远程------------------
		RuningOrRemote();
	}
}


