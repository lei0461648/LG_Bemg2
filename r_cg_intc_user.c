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
* File Name    : r_cg_intc_user.c
* Version      : CodeGenerator for RL78/G13 V2.03.04.01 [11 Nov 2016]
* Device(s)    : R5F100FE
* Tool-Chain   : CCRL
* Description  : This file implements device driver for INTC module.
* Creation Date: 2025-04-02
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_intc.h"
/* Start user code for include. Do not edit comment generated here */
#include "var_define.h"	
#include "set_other.h"
#include "m_manual_1.h"
#include "b_cali.h"
#include "b_basic.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_intc1_interrupt(vect=INTP1)
#pragma interrupt r_intc2_interrupt(vect=INTP2)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
unsigned int UI_ZeroCali_C; //??????

/************extern PARAM*************/
extern unsigned char UC_StartZeroing_S;
extern unsigned char UC_SwitchCali_S;
extern unsigned char UI_SlowModeStatus_SF;
extern unsigned int UI_Percent_H2_DF;
extern unsigned long int ULI_C_PulseCallBack_C;//??????
extern unsigned int TDR07_Percent_C; //????

unsigned char UC_IntiHigh_S;//?????
unsigned char UC_IntiLow_S;//?????
extern unsigned int UI_ShakingERROR_S;//????
extern unsigned int UC_FastPull_D;//??????
extern unsigned long int TDR07_Percent_Temp; //?????
extern unsigned  long int UC_SetZhuanSu_D;//??????
extern unsigned char UC_C_OnOff_S; //????
unsigned int UC_GDEorro_S;//????
extern  unsigned char UC_C_Gaosu_Count; //????

unsigned char UC_ChangKSModel_S;//????????
extern unsigned int KS_PWMPULSE; //?????????
extern unsigned char PluseChange_S;//??????
extern unsigned int UI_PulseWidth_D;//???????

unsigned int UI_GDError_D;//???????
unsigned int UI_GDError_C;//???????
extern unsigned char UI_C_GDAlarm_S; //光电报警状态
extern unsigned int AtPresentPluse;//当前脉冲

//extern unsigned long int aaa;
//extern unsigned long int bbb;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_intc1_interrupt
* Description  : This function is INTP1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_intc1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    // ??????10000 ??0 ??????-- > ??????  2 ?????? 3 ?????
    //if(UI_PulseWidth_D<=KS_PWMPULSE&&UC_C_OnOff_S==TRUE&&UC_SwitchCali_S==FALSE&&UC_StartZeroing_S==FALSE)
    
    //??????????=?? - ?????????
    if((UC_C_OnOff_S==TRUE||UC_SwitchCali_S==TRUE)&&UC_ChangKSModel_S==1)
    {	    
	    if(AtPresentPluse>TDR07_Percent_C)//??
	    {
		    PluseChange_S=1;//???
	    }
	    else 
	    {
		   if(INPT_1==1)					//0-1?y?
		    {
			UI_PulseWidth_D=TDR07_Percent_C;//???????????
			setPWMPulseWidth();
			PUMP_Direction=1;
		    }
		    else
		    {
			SuckFast();
			PUMP_Direction=0;
		    }      
	    }  
    }
    
    //???????????
    if(INPT_1==1)
    {
	    if(UI_PulseWidth_D>=3000||UI_ShakingERROR_S==2)
	    {
		    UC_IntiHigh_S=0;
		    UC_IntiLow_S=0;
	    }
	    UC_IntiHigh_S=1;
	    
	    //??? ???
	    if(UC_StartZeroing_S==TRUE)
	    {
			UC_StartZeroing_S=FALSE;
			//ComControl();
			Com_OFF();//????
	    }
	   
	    
	    //?????????
	    if(UC_SwitchCali_S==TRUE)
	    {
	    	UI_ZeroCali_C++;
		if(UI_ZeroCali_C>=CALI_STTROKE_COUNT)
		{
			UC_SwitchCali_S=FALSE;
			ComControl();
		}
		else if(UI_ZeroCali_C==(CALI_STTROKE_COUNT-2))
		{
			Switch_Cali(UC_SetZhuanSu_D*0.7);//--------bug ????????
		}
		else if(UI_ZeroCali_C==(CALI_STTROKE_COUNT-1))
		{
			Switch_Cali(UC_SetZhuanSu_D*0.49);
		}
		
	    }
	    PUMP_Direction=1;//???????
    }
    else 
    {
	    UC_IntiLow_S=1;
	    
	    PUMP_Direction=0;//???????
    }
    
    //????????-???? ????????
    UC_C_Gaosu_Count=0;
    UI_GDError_D=0;//光电异常检测
    UI_GDError_C=0;
    UI_C_GDAlarm_S=GD_NORMAL; //光电报警取消
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc2_interrupt
* Description  : This function is INTP2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_intc2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    ULI_C_PulseCallBack_C++;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
