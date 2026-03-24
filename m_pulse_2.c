#include "r_cg_macrodriver.h"
#include "r_cg_wdt.h"
#include "m_pulse_2.h"
#include "get_screen.h"
#include "b_basic.h"
#include "var_define.h"
#include "UART_SET.h"
#include "r_cg_serial.h"
#include "r_cg_userdefine.h"

extern unsigned long int LI_PulseFlow_ML_DF;
unsigned int UI_Pulse_HZ_DF;

/************extern PARAM*************/
extern unsigned long int LI_TargetFlowML_H2_DF;
extern volatile uint32_t g_tau0_ch3_width;
extern unsigned char UC_C_OnOffStatus_S;
extern unsigned char UC_PulseTrigger_S;
extern unsigned char UC_RemoteStatusHL_S;
extern unsigned int UI_Percent_H2_DF;

/************Temp PARAM*************/
unsigned long int LI_Width_Temp;

unsigned char UC_IsHZero_C;

/************************************************
* Function Name: m_RunManual  
* Description  : Run Manual Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void m_RunPulse(void)
{
	//计算宽度 //1HZ 对应的 62400 g_tau0_ch3_width  100HZ 对应的 624 
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
	else
	{
		LI_Width_Temp=0;
	}
	
	//Pulseless input  Puls 0  没有脉冲输入或是HZ非常快判定为异常状态  做设备暂停处理
	if(g_tau0_ch3_width==0||UI_Pulse_HZ_DF==0)
	{
		if(++UC_IsHZero_C>=5)//低脉宽有波动
		{
			UI_Pulse_HZ_DF=0;
			//--------------------------------------------控制泵开关逻辑-----异常-----------------------------------------------------------------
			FunctionalWait();
		}
	}
	else
	{
		//输入的脉冲范围就在 200和10之间
		if(UI_Pulse_HZ_DF>=100)
		{
			UI_Pulse_HZ_DF=100;
		}
		UC_IsHZero_C=0;//清空计数
		//--------------------------------------------控制泵开关逻辑----远程-----------------------------------------------------------------
		RuningOrRemote();
	}
	
	get_PulsePercent();
	
	getPWMForPercent(UI_Percent_H2_DF);
	
	//Clear pulse period 清除脉冲HZ数
	if(UC_PulseTrigger_S==1)
	{
		g_tau0_ch3_width=0;
	}
	UC_PulseTrigger_S=0;	//清除反转状态
}



/************************************************
* Function Name: get_PercentPWM  
* Description  : get  Percent
* Arguments    : None
* Return Value : None
*************************************************/
void get_PulsePercent(void)
{
	//UI_Pulse_HZ_DF   Min10Hz   Max200Hz
	LI_TargetFlowML_H2_DF=LI_PulseFlow_ML_DF*UI_Pulse_HZ_DF*3600/100; //Pulse ml  0.0015ml min, LI_PulseFlow_ML_H4_DF
	get_Percent(LI_TargetFlowML_H2_DF);
}
