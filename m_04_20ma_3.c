#include "r_cg_macrodriver.h"
#include "r_cg_wdt.h"
#include "m_04_20ma_3.h"
#include "get_screen.h"
#include "b_basic.h"
#include "var_define.h"
#include "r_cg_userdefine.h"

extern unsigned char UC_04_20MAType_SF;
unsigned int UI_PercentMA_D;
unsigned long int UI_0_20MAZT_K_DF;
unsigned long int UI_0_20MAZT_B_DF;
unsigned long int UI_0_20MAFT_K_DF;
unsigned long int UI_0_20MAFT_B_DF;
unsigned long int LI_Q_D;



/************extern PARAM*************/
extern unsigned long int LI_TargetFlowML_H2_DF;
extern unsigned long long int targetFlowML_H2_Long_Temp;
extern unsigned int UI_C_MA_H1_D;
extern unsigned int UI_RatedMAXFlow_DF; //额定最大流量
extern unsigned int UI_Percent_H2_DF; //百分比

extern unsigned int  UC_04_20MA_I1_DF;
extern unsigned int  UC_04_20MA_Q1_DF;
extern unsigned int  UC_04_20MA_I2_DF;
extern unsigned int  UC_04_20MA_Q2_DF;
extern unsigned int UI_SecondlyFlowML_H1_D;//每10秒多少流量

/************Temp PARAM*************/
unsigned long int LI_MA_Temp;
unsigned long int LI_Q1Q2_Temp;
unsigned long int LI_Q1B_Temp;
unsigned long int LI_KI1_Temp;

unsigned int I1I2_Temp;


/************************************************
* Function Name: m_RunManual  
* Description  : Run Manual Mode 
* Arguments    : None
* Return Value : None
*************************************************/
void m_Run04_20MA(void)
{
	if(UC_04_20MAType_SF==M_0_20MATYPE)
	{
		CurrToPct0_20MA(UI_C_MA_H1_D);
	}
	else if(UC_04_20MAType_SF==M_4_20MATYPE)
	{
		CurrToPct4_20MA(UI_C_MA_H1_D);
	}
	else if(UC_04_20MAType_SF==M_0_20MAZTTYPE)
	{
		Get0_20MAZT(UC_04_20MA_I1_DF,UC_04_20MA_Q1_DF,UC_04_20MA_I2_DF,UC_04_20MA_Q2_DF,UI_C_MA_H1_D);
	}
	else if(UC_04_20MAType_SF==M_0_20MAFTTYPE)
	{
		Get0_20MAFT(UC_04_20MA_I1_DF,UC_04_20MA_Q1_DF,UC_04_20MA_I2_DF,UC_04_20MA_Q2_DF,UI_C_MA_H1_D);
	}
}

/************************************************
* Function Name: CurrToPct0_20MA
* Description  : Converts a 0-20mA current value to a percentage.
* Arguments    : curr - curr 0.1 【Acc】 15.4mA curr 154
* Return Value : The corresponding percentage (0% to 100%).
*************************************************/
void CurrToPct0_20MA(unsigned int curr)
{
	if (curr>=200){curr=200;}
	
	LI_MA_Temp=curr;
	UI_PercentMA_D=LI_MA_Temp*50; //5000 = 50.00%
	get_PercentMA(UI_PercentMA_D);
	
	//通过4-20ma 输入得到百分比 百分比获取目标流量
	Get_TargetFlowForPerMA(UI_PercentMA_D);
}

/************************************************
* Function Name: CurrToPct0_20MA
* Description  : Converts a 0-20mA current value to a percentage.
* Arguments    : curr - curr 0.1 【Acc】 15.4mA curr 154
* Return Value : The corresponding percentage (0% to 100%).
*************************************************/
void CurrToPct4_20MA(unsigned int curr)
{
	unsigned long int UI_PercentMA_D_Temp=0;
	if (curr<40)
	{
		curr=40; //电流小于4ma 需要暂停
	}   
	if (curr>200)  curr=200;

	//y = 62.5x - 2500
	LI_MA_Temp=curr;
	UI_PercentMA_D_Temp=(LI_MA_Temp-40)*625;
	UI_PercentMA_D=UI_PercentMA_D_Temp/10;
	get_PercentMA(UI_PercentMA_D);
	
	//通过4-20ma 输入得到百分比 百分比获取目标流量
	Get_TargetFlowForPerMA(UI_PercentMA_D);
}


/************************************************
* Function Name: Get4_20MAZT
* Description  : Converts a 0-20mA current value to a percentage.
* Arguments    : curr - curr 0.1 【Acc】 15.4mA curr 154
* Return Value : The corresponding percentage (0% to 100%).
*************************************************/
void Get0_20MAZT(unsigned int I1,unsigned int Q1,unsigned int I2,unsigned int Q2,unsigned int curr)
{
	//异常处理
	if (I2 <= I1 || Q2 < Q1) 
	{
        	LI_TargetFlowML_H2_DF=0;
		get_Percent(0);
		FunctionalWait();//暂停
    	}
	else if ((curr < I1) || (curr == 0 && I1 == 0 && Q1 == 0)) 
	{
		LI_TargetFlowML_H2_DF=0;
		get_Percent(0);
		
		FunctionalWait();//暂停
	}else if (curr >= I2) 
	{
		LI_TargetFlowML_H2_DF=Q2; //Q2 150.0 =150.0L/H
		LI_TargetFlowML_H2_DF=LI_TargetFlowML_H2_DF*10000; //150.0L/H = 150.000.00 ML后两位
		get_Percent(LI_TargetFlowML_H2_DF);
		//--------------------------------------------控制泵开关逻辑----远程-----------------------------------------------------------------
		RuningOrRemote();
	}
	else
	{
		
		 // 核心计算（全整数运算）  Q1 Q2  L_H1 下面计算又增加了10倍 计算得出 L_H2  于目标流量 ML_H2 后面在扩大1000倍
		const uint16_t delta_I = curr - I1;             // 电流差
		const uint32_t delta_Q = (uint16_t)(Q2 - Q1) * 10; // 流量差扩大10倍
		const uint32_t numerator = delta_I * delta_Q;   // 分子
		const uint16_t denominator = I2 - I1;          // 分母
		
		 // 四舍五入：(numerator + denominator/2) / denominator
   		const uint32_t increment = (numerator + denominator / 2) / denominator;

   		 // 返回Q1扩大10倍后的值 + 增量
    		//return (Q1 * 10) + (uint32_t)increment;
		
		//I1I2_Temp=Q1 * 10;
		LI_TargetFlowML_H2_DF= (Q1 * 10) + (uint32_t)increment;
		LI_TargetFlowML_H2_DF=LI_TargetFlowML_H2_DF*1000;
		get_Percent(LI_TargetFlowML_H2_DF);
		getPWMForPercent(UI_Percent_H2_DF);
		//--------------------------------------------控制泵开关逻辑----远程-----------------------------------------------------------------
		RuningOrRemote();
	}
}


/************************************************
* Function Name: Get4_20MAZT
* Description  : Converts a 0-20mA current value to a percentage.
* Arguments    : curr - curr 0.1 【Acc】 15.4mA curr 154
* Return Value : The corresponding percentage (0% to 100%).
*************************************************/
void Get0_20MAFT(unsigned int I1,unsigned int Q1,unsigned int I2,unsigned int Q2,unsigned int curr)
{
	//异常处理
	if (I2 <= I1 || Q1 < Q2) 
	{
        	LI_TargetFlowML_H2_DF=0;
		get_Percent(0);
    	}
	else if (curr < I1) 
	{
		LI_TargetFlowML_H2_DF=0;
		get_Percent(0);
		
		FunctionalWait();//暂停
	}else if (curr >= I2) 
	{
		LI_TargetFlowML_H2_DF=Q2; //Q2 150.0 =150.0L/H
		LI_TargetFlowML_H2_DF=LI_TargetFlowML_H2_DF*10000; //150.0L/H = 150.000.00 ML后两位
		get_Percent(LI_TargetFlowML_H2_DF);
		//--------------------------------------------控制泵开关逻辑----远程-----------------------------------------------------------------
		RuningOrRemote();
	}
	else
	{
		
		 // 核心计算（全整数运算）  Q1 Q2  L_H1 下面计算又增加了10倍 计算得出 L_H2  于目标流量 ML_H2 后面在扩大1000倍
		const uint16_t delta_I = curr - I1;             // 电流差
		const uint32_t delta_Q = (uint16_t)(Q1 - Q2) * 10; // 流量差扩大10倍
		const uint32_t numerator = delta_I * delta_Q;   // 分子
		const uint16_t denominator = I2 - I1;          // 分母
		
		 // 四舍五入：(numerator + denominator/2) / denominator
   		const uint32_t increment = (numerator + denominator / 2) / denominator;

   		 // 返回Q1扩大10倍后的值 + 增量
    		//return (Q1 * 10) + (uint32_t)increment;
		
		//I1I2_Temp=Q1 * 10;
		LI_TargetFlowML_H2_DF= (Q1 * 10) - (uint32_t)increment;
		LI_TargetFlowML_H2_DF=LI_TargetFlowML_H2_DF*1000;
		get_Percent(LI_TargetFlowML_H2_DF);
		getPWMForPercent(UI_Percent_H2_DF);
		//--------------------------------------------控制泵开关逻辑----远程-----------------------------------------------------------------
		RuningOrRemote();
	}
	if(UI_Percent_H2_DF==0) //百分比等于0 机器暂停
	{
		FunctionalWait();
	}	
}

/************************************************
* Function Name: CurrToPct0_20MA
* Description  : Converts a 0-20mA current value to a percentage.
* Arguments    : curr - curr 0.1 【Acc】 15.4mA curr 154
* Return Value : The corresponding percentage (0% to 100%).
*************************************************/
void Get_TargetFlowForPerMA(unsigned int PercentMA)
{
	getPWMForPercent(PercentMA); //通过百分比获取当前流量的脉宽
	
	targetFlowML_H2_Long_Temp=PercentMA;  //JD 00.00%  50%- PercentMA=5000 
	targetFlowML_H2_Long_Temp=targetFlowML_H2_Long_Temp*UI_RatedMAXFlow_DF;  //50.00%*2000=100.000.00=ML_H2 通过当前百分比计算目标流量
	LI_TargetFlowML_H2_DF=targetFlowML_H2_Long_Temp;
	
	UI_SecondlyFlowML_H1_D=LI_TargetFlowML_H2_DF/3600;//计算出每10秒多少流量 ML_H1
	
	//泵暂停工作的逻辑处理（包含远程和百分比为0）
	if(PercentMA==0)
	{
		FunctionalWait();//百分比为0泵头暂停工作
	}
	else 
	{
		RuningOrRemote();//运行过程中远程开关逻辑
	}
}
