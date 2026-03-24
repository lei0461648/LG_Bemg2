#include "r_cg_macrodriver.h"
#include "r_cg_wdt.h"
#include "get_screen.h"
#include "var_define.h"
#include "UART_SET.h"
#include "r_cg_serial.h"
#include "b_basic.h"
#include "r_cg_userdefine.h"


unsigned long int UI_SingleStrokeML_H3_DF;
unsigned long int UI_MaxFlowML_H2_DF;
unsigned int UI_Percent_H2_DF;

unsigned int TDR07_Percent_C;
unsigned int UI_PulseWidth_D;//设置脉宽
unsigned long int TDR07_Percent_Temp;

extern unsigned int KS_PWMPULSE; //进入快吸慢推的脉宽
unsigned int AtPresentPluse;//当前脉冲
unsigned int NewPluse;//新脉冲
unsigned char PluseChange_S;//是否改变脉宽
extern unsigned char UC_C_OnOffStatus_S; //开机状态


/************extern PARAM*************/
extern unsigned int UI_RatedMAXFlow_DF; //额定最大流量
extern unsigned long int LI_RatedMAXPulseWidth_DF; //额定脉宽根据额定流量计算得出
extern unsigned char UC_ChangKSModel_S;//改变快吸慢推模式
extern unsigned int UI_SlowModeStatus_SF;//设置慢速模式

/************Temp PARAM*************/
unsigned long int UI_Percent_H2_Temp;
unsigned long long int targetFlowML_H2_Long_Temp;

//每秒多少流量
unsigned int UI_SecondlyFlowML_H1_D;
unsigned int UI_strokeCount_D;

/************************************************
* Function Name: RatedFlow  
* Description  : Nominal volume divided by the number of strokes
                 STTROKE_COUNT 200  
* Arguments    : calibVol  for example  120.0ml ,  STTROKE_COUNT default 200  calibVol/STTROKE_COUNT=0.601
* Return Value : None
*************************************************/
void RatedFlow(unsigned long int calibVol)
{
	unsigned long int tempResult;
	tempResult = calibVol;
	tempResult = tempResult * 100;  //120.0 ml *100  = 120.000
    	UI_SingleStrokeML_H3_DF = tempResult / CALI_STTROKE_COUNT;
	Get_MaximumFlowML();
}

/************************************************
* Function Name: Get_MaximumFlow  获取最大流量
* Description  : get max mum flow TO ml
* Arguments    : None    for example  STTROKE_COUNT_HOUR (12000*6010)/10000=7212 ml/h = 72.12l/h
* Return Value : None
*************************************************/
void Get_MaximumFlowML(void)
{
	UI_MaxFlowML_H2_DF=UI_SingleStrokeML_H3_DF;
	
	if (UI_SlowModeStatus_SF == 2) {
	    UI_strokeCount_D = STTROKE_COUNT_HOUR_25;
	} else if (UI_SlowModeStatus_SF == 1) {
	    UI_strokeCount_D = STTROKE_COUNT_HOUR_50;
	} else {
	    UI_strokeCount_D = STTROKE_COUNT_HOUR;
	}
	
	UI_MaxFlowML_H2_DF = (UI_MaxFlowML_H2_DF * UI_strokeCount_D) / 10;
}


/************************************************
* Function Name: get_Percent
* Description  : get  Percent
* Arguments    : None
* Return Value : None
*************************************************/
void get_Percent(unsigned long int targetFlowML_H2)
{	
	targetFlowML_H2_Long_Temp=targetFlowML_H2; //180.000.00 L
	//targetFlowML_H2_Long_Temp=targetFlowML_H2_Long_Temp/10; //180.000.0L   因为额定流量扩大10
	UI_SecondlyFlowML_H1_D=targetFlowML_H2_Long_Temp/3600;//计算出每10秒多少流量 ML_H1   180.000.0L/360=500.0ML
	UI_Percent_H2_Temp=targetFlowML_H2_Long_Temp/UI_RatedMAXFlow_DF; ////180.000.00/2000 目标流量 除以 额定最大流量  =9000= 90.00%
	
	UI_Percent_H2_DF=UI_Percent_H2_Temp;//% 00.00 获取百分比
}

/************************************************
* Function Name: get_Percent
* Description  : get  Percent
* Arguments    : None
* Return Value : None
*************************************************/
void get_PercentMA(unsigned int PercentMA)
{
	UI_Percent_H2_DF=PercentMA;
}

/************************************************
* Function Name: getPWMForPercent
* Description  : get  Percent
* Arguments    : None
* Return Value : None
*************************************************/
void getPWMForPercent(unsigned int Percent)
{
	
	if(Percent<=10000)
	{
		TDR07_Percent_Temp=Percent;
		TDR07_Percent_Temp=TDR07_Percent_Temp*LI_RatedMAXPulseWidth_DF;
		TDR07_Percent_Temp=TDR07_Percent_Temp/10000;
		TDR07_Percent_C=TDR07_Percent_Temp;
	}
	else
	{
		TDR07=40000;
	}
	
	ChangeSlowMode();//判断是否进入或退出快吸慢推模式
	
	/**********************减速要平滑****当前是运输模式才改变直接改变脉宽********************************/
	if(UC_ChangKSModel_S==0)
	{
		if(AtPresentPluse>TDR07_Percent_C&&UC_C_OnOffStatus_S==TRUE) //当前脉宽大于设置新脉宽 在机器运行的时候做动态减速才有意义
		{
			PluseChange_S=1;
			
		}
		else //加速过程不会卡死直接运行
		{
			UI_PulseWidth_D=TDR07_Percent_C;
			setPWMPulseWidth();//设置脉宽宽度
			PluseChange_S=0;
		}
	}
}

/************************************************
* Function Name: setPWMPulseWidth
* Description  : get  Percent
* Arguments    : None 
* Return Value : None
*************************************************/
void setPWMPulseWidth(void)
{
	if(UI_PulseWidth_D<40000)
	{
		TDR07=UI_PulseWidth_D;
		AtPresentPluse=UI_PulseWidth_D;//当前脉宽
	}
	else
	{
		TDR07=40000;
	}
}

/************************************************
* Function Name: setSuckFastAndPushSlow
* Description  : get  Percent
* Arguments    : None
* Return Value : None
*************************************************/
void setSuckFastAndPushSlow(void)
{
	TDR07=KS_PWMPULSE;//设置固定快吸速度
}

