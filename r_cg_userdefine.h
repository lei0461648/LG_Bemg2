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
* File Name    : r_cg_userdefine.h
* Version      : CodeGenerator for RL78/G13 V2.03.04.01 [11 Nov 2016]
* Device(s)    : R5F100FE
* Tool-Chain   : CCRL
* Description  : This file includes user definition.
* Creation Date: 2025-04-02
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
void FlashSetPar(void);
void FlashGetPar(void);

void INIT_UART2(void);
void Initial(void);
void Write1302(unsigned char addr,unsigned char dat);
unsigned char Read1302 ( unsigned char addr );
void InitPoweringOn(void);

void R_MAIN_UserInit(void);
void Change_SystemTime(unsigned char Time_Minute,unsigned char Time_Hour,unsigned char Time_Day,unsigned char Time_Month,unsigned char Time_Year);
void R_PORT_P61_IN(void);
void R_PORT_P61_OUT(void);
void Com_Run(void);
void Com_WAIT(void);
void Com_OFF(void);

//-------------------------------------------------------------------------------------------------------------------------------
void ComControl(void);
void RunCal(void);
void ISendDataToBoard(void);
void ComSetParInit(void);
void Write_Date(unsigned int dat);
void Get_Data(void);
void SendData_485URT1(void);

void Check_eeproom(void);
void Write_EEprom(void);
void eeprom_write(unsigned char page);
void RuningOrRemote(void);
void FunctionalWait(void);
void FlashGetValue(void);
void ChangeSlowMode(void);
void ClearStatus(void);
void readBackData(void);
void ClearStatusAndData(void);
void setParameter(unsigned int par_address,unsigned int par_data);
void CLoseClearAlarmS(void);

/* End user code. Do not edit comment generated here */
#endif
