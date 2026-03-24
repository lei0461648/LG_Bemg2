#include "r_cg_macrodriver.h"
#include "r_cg_wdt.h"
#include "m_manual_1.h"
#include "get_screen.h"
#include "b_basic.h"
#include "var_define.h"
#include "UART_SET.h"
#include "r_cg_serial.h"
#include "r_cg_userdefine.h"

unsigned long int LI_ManualFlow_L_Temp;

/************extern PARAM*************/
extern unsigned long int LI_ManualFlow_L_DF;
extern unsigned char UC_ManualFlowUnit_DF;
extern unsigned long int UI_MaxFlowML_H2_DF;
extern unsigned long int LI_TargetFlowML_H2_DF;
extern unsigned int UI_Percent_H2_DF;

/************************************************
* Function Name: m_RunManual  
* Description  : Run Manual Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void m_RunManual(void)
{
	get_ManualPercent();
}



/************************************************
* Function Name: get_ManualPercent  
* Description  : get  Percent
* Arguments    : None
* Return Value : None
*************************************************/
void get_ManualPercent(void)
{
	//------控制泵开关逻辑----远程-----
	//RuningOrRemote();
	//手动流量转化目标流量
	LI_TargetFlowML_H2_DF=LI_ManualFlow_L_DF*10; //LI_ManualFlow_L_DF  Manual L  0.0025L min, 2.5ML  LI_ManualFlowML_H1_DF 
	
	get_Percent(LI_TargetFlowML_H2_DF); //获取当前流量百分比
	
	getPWMForPercent(UI_Percent_H2_DF); //通过百分比获取当前流量的脉宽
	
	//??????????(?????????0)
	if(UI_Percent_H2_DF==0)
	{
		FunctionalWait();//????0??????
	}
	else 
	{
		RuningOrRemote();//???????????
	}
}
