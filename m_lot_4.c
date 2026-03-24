#include "r_cg_macrodriver.h"
#include "r_cg_wdt.h"
#include "m_lot_4.h"
#include "get_screen.h"
#include "b_basic.h"
#include "var_define.h"
#include "r_cg_userdefine.h"

extern unsigned long int LI_LotVolume_DF;//_H2
extern unsigned char UC_LotVolumeUnit_SF;
extern unsigned long int UC_AddTimeSec_DF;

unsigned long int LI_TimeINFlowML_H2_D;
unsigned long int LI_SEC_D;

unsigned long int LI_ResidualFlow_D; //剩余批量容积
unsigned int UI_CountSeconds_D;
unsigned char UC_IsPulseLaunch_S=0;

/************Temp PARAM*************/
unsigned long long int LLI_TimeINFlowML_H2_Temp;

/************Temp extern*************/
extern unsigned char UC_ModeRun_SF;
extern unsigned char UC_C_OnOff_S;
extern unsigned char UC_C_OnOffStatus_S;
extern unsigned char UC_PulseTrigger_S;
extern volatile uint32_t g_tau0_ch3_width;
extern unsigned int UI_Percent_H2_DF; //当前百分比
extern unsigned long int LI_TargetFlowML_H2_DF; //目标流量

extern unsigned long int LI_LotVolumeTemp_DF; //批次容积的临时变量
extern unsigned char UC_LotVolumeUnitTemp_SF; //批次容积的临时变量单位
extern unsigned char UC_C_OnOffStatus_S; //机器状态
extern unsigned long int LI_Width_Temp;
extern unsigned int UI_Pulse_HZ_DF;
extern unsigned char  UC_BatchManual_SF;    //批次手动
extern unsigned char  UC_BatchManualOver_S=0;    //批次手动本局结束

/************************************************
* Function Name: void m_RunLot(void)  
* Description  : Run Lot Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void m_RunLot(void)
{
	get_LotTargetFlow();
}


/************************************************
* Function Name: void get_LotTargetFlow(void)  
* Description  : Run Lot Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void get_LotTargetFlow(void)
{
	//获取一个剩余流量的总数
	if(UC_LotVolumeUnit_SF==U_L)
	{
		LI_TimeINFlowML_H2_D=LI_LotVolume_DF*1000;
	}
	else if(UC_LotVolumeUnit_SF==U_ML)
	{
		LI_TimeINFlowML_H2_D=LI_LotVolume_DF;
	}
	
	get_LotPercent();
}

/************************************************
* Function Name: void get_SecForAddTimeSec(void)  
* Description  : Run Lot Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void get_LotPercent(void)
{	
	LLI_TimeINFlowML_H2_Temp=LI_TimeINFlowML_H2_D;
	LLI_TimeINFlowML_H2_Temp=(LLI_TimeINFlowML_H2_Temp*3600)/UC_AddTimeSec_DF;
	LI_TargetFlowML_H2_DF=LLI_TimeINFlowML_H2_Temp;//赋值目标流量
	get_Percent(LLI_TimeINFlowML_H2_Temp);
	getPWMForPercent(UI_Percent_H2_DF);
	
	//get_ResidualFlow();
	/*
	if(UC_AddTimeSec_DF>3600)
	{
		
	}
	else if(UC_AddTimeSec_DF<3600)
	{
		LI_TimeINFlowML_H2_D=(LI_TimeINFlowML_H2_D*3600)/UC_AddTimeSec_DF;
		get_Percent(LI_TimeINFlowML_H2_D);// 1 h 
	}
	else 
	{
		get_Percent(LI_TimeINFlowML_H2_D);// 1 h 
	}*/
	
}

/************************************************
* Function Name: void get_ResidualFlow(void)  
* Description  : Run Lot Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void get_ResidualFlow(void)
{
	unsigned long long int LLI_ML_H2_4Temp;
	
	// 1. 先判断是否在批次模式
	if(UC_ModeRun_SF==BATCH_MODE)
	{
		if(UC_C_OnOff_S==TRUE) // 2. 判断屏幕上的开机状态
		{
			if(UC_IsPulseLaunch_S==0) //Waiting pulse 开机等待脉冲
			{
				FunctionalWait();//等待脉冲
				if(UC_BatchManual_SF==FALSE)
				{
					if(UC_PulseTrigger_S==1)//Starting pulse One
					{
						
						CalculatedPulse();//计算脉冲
						g_tau0_ch3_width=0;
						UC_PulseTrigger_S=0;
						
						//HZ 1-100HZ 之间设备启动
						if(UI_Pulse_HZ_DF>0&&UI_Pulse_HZ_DF<=100)
						{
							UC_IsPulseLaunch_S=1;
							RuningOrRemote();//--控制泵开关逻辑---运行-远程-
						}
					}
				}
				else
				{
					// 新增判断：只有当前批次未结束时，才允许启动
					if(UC_BatchManualOver_S == 0) 
					{
						UC_IsPulseLaunch_S=1;
						RuningOrRemote();//--控制泵开关逻辑---运行-远程-
					}
				}
				
			}
			else if(UC_IsPulseLaunch_S==1&&UC_C_OnOffStatus_S==TRUE) //接收脉冲开始启动 并且泵为开机状态
			{
				if(++UI_CountSeconds_D<UC_AddTimeSec_DF)
				{
					LLI_ML_H2_4Temp=(LLI_TimeINFlowML_H2_Temp/3600)*UI_CountSeconds_D;
					if(LI_TimeINFlowML_H2_D>LLI_ML_H2_4Temp)
					{
						LI_ResidualFlow_D=LI_TimeINFlowML_H2_D-LLI_ML_H2_4Temp;
					}
					else 
					{
						LI_ResidualFlow_D=0;
					}
					
					//--------------------------------------------控制泵开关逻辑---运行-远程-----------------------------------------------------------------
					RuningOrRemote();
				}
				else 
				{
					LI_ResidualFlow_D=0;
					UC_IsPulseLaunch_S=0;
					UC_PulseTrigger_S=0;
					UI_CountSeconds_D=0;
					
					// 【关键修改】：标记手动批次本局已结束，防止立刻重新进入上面的启动分支
					if(UC_BatchManual_SF == TRUE)
					{
						UC_BatchManualOver_S = 1; 
					}
					
					//Reassign the remaining traffic
					if(UC_LotVolumeUnit_SF==U_L)
					{
						LI_ResidualFlow_D=LI_LotVolume_DF*1000;
					}
					else if(UC_LotVolumeUnit_SF==U_ML)
					{
						LI_ResidualFlow_D=LI_LotVolume_DF;
					}
					
					//--------------------------------------------控制泵开关逻辑-----一次批次运行完毕 开始等待-----------------------------------------------------------------
					FunctionalWait();
				}
			}
			else 
			{
				//--------------------------------------------控制泵开关逻辑---运行-远程-----------------------------------------------------------------
				RuningOrRemote();//解除远程报警
			}
		}
		else 
		{
			// === 客户在界面上点击了“关机” ===
			// 此时设备停止，我们把“本局结束”标志复位
			// 这样客户下次再点“开机”时，UC_BatchManualOver_S == 0，就能重新启动了
			UC_BatchManualOver_S = 0; 
			
			// 安全起见，关机时顺便把运行标志也清零
			UC_IsPulseLaunch_S = 0; 
			UI_CountSeconds_D = 0; 
		}
		
	}
}

//计算脉冲是否在 1-100HZ
void CalculatedPulse(void)
{
	if(g_tau0_ch3_width<70000&&g_tau0_ch3_width>500)
	{
		LI_Width_Temp=624000/g_tau0_ch3_width; //扩大10倍 用于下面的四舍五入
		
		//四舍五入得到HZ
		if((LI_Width_Temp%10)>=7)
		{
			UI_Pulse_HZ_DF=(unsigned int)(LI_Width_Temp/10+1);
		}
		else
		{
			UI_Pulse_HZ_DF=(unsigned int)(LI_Width_Temp/10);
		}
	}
}