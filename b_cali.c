#include "r_cg_macrodriver.h"
#include "var_define.h"
#include "b_cali.h"
#include "b_basic.h"

/************PARAM*************/
unsigned char UC_SwitchCali_S=FALSE;
unsigned int UC_StrokesNumber_C;
unsigned char UC_StartZeroing_S=FALSE;

/************extern PARAM*************/
extern unsigned int UI_Percent_H2_DF;
extern unsigned int UI_PulseWidth_D;//设置脉宽
extern unsigned int TDR07_Percent_C; //当前脉宽

/************************************************
* Function Name: Get_CurrentStrokeCount  
* Description  : Gets the current number of strokes
                 
* Arguments    : None
* Return Value : None
*************************************************/
void Get_CurrentStrokeCount(void)
{
	if(UC_StrokesNumber_C>=CALI_STTROKE_COUNT)
	{
		UC_SwitchCali_S=TRUE;
	}
}

/************************************************
* Function Name: Open_Stroke  
* Description  : Open Stroke
                 
* Arguments    : None
* Return Value : None
*************************************************/
void Switch_Cali(unsigned int PulseWidth)
{
	if(UC_SwitchCali_S==TRUE)
	{
		TDR07_Percent_C=PulseWidth;//标定也是快吸慢推
		UI_PulseWidth_D=PulseWidth; //27000us 34000*0.8
		setPWMPulseWidth();
	}	
}


/************************************************
* Function Name: StartZero  
* Description  : Start Zeroing
                 
* Arguments    : None
* Return Value : None
*************************************************/
void StartZero(void)
{
	UC_StartZeroing_S=TRUE;
	
	UI_PulseWidth_D=11000; //11000ms 30%
	setPWMPulseWidth();
}

