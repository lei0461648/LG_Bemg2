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
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/G13 V2.03.04.01 [11 Nov 2016]
* Device(s)    : R5F100FE
* Tool-Chain   : CCRL
* Description  : This file implements device driver for Serial module.
* Creation Date: 2025-04-02
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "var_define.h"	
#include "r_cg_wdt.h"
#include "UART_SET.h"
#include "string.h"
#include "r_cg_timer.h"
#include "set_other.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_uart1_interrupt_send(vect=INTST1)
#pragma interrupt r_uart1_interrupt_receive(vect=INTSR1)
#pragma interrupt r_uart2_interrupt_send(vect=INTST2)
#pragma interrupt r_uart2_interrupt_receive(vect=INTSR2)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart1_tx_address;         /* uart1 send buffer address */
extern volatile uint16_t  g_uart1_tx_count;            /* uart1 send data number */
extern volatile uint8_t * gp_uart1_rx_address;         /* uart1 receive buffer address */
extern volatile uint16_t  g_uart1_rx_count;            /* uart1 receive data number */
extern volatile uint16_t  g_uart1_rx_length;           /* uart1 receive data length */
extern volatile uint8_t * gp_uart2_tx_address;         /* uart2 send buffer address */
extern volatile uint16_t  g_uart2_tx_count;            /* uart2 send data number */
extern volatile uint8_t * gp_uart2_rx_address;         /* uart2 receive buffer address */
extern volatile uint16_t  g_uart2_rx_count;            /* uart2 receive data number */
extern volatile uint16_t  g_uart2_rx_length;           /* uart2 receive data length */
/* Start user code for global. Do not edit comment generated here */

unsigned int UI_URT2_REC_C=0;
unsigned int UI_URT1_REC_C=0;
unsigned char UC_S_FunNum;    

/********************Flag parameter******************/

unsigned int crc_r;
unsigned char crc_H=0;	
unsigned char crc_L=0;
unsigned char *puch;
unsigned char *puch1;
unsigned int address_H;
unsigned char address_L;
unsigned int address;
unsigned char i_for;
unsigned char i_count;
unsigned char i_count2;
unsigned char UC_S_ERR_CODE;
unsigned int UI_S_TempData_D;
unsigned char UC_S_Para_Invalid_D;
unsigned char temp_index_count=3;
unsigned int byte_count;
unsigned char Control_485;
unsigned int UC_D_SetCurFlow_Panel;
extern unsigned int  Pressure_Range;
extern  uint8_t UI_URT2_REC_D[];
extern  uint8_t UI_URT2_SEND_D[];
extern  uint8_t UI_URT1_REC_D[];
extern  uint8_t UI_URT1_SEND_D[];
extern unsigned char UC_C_WriteFlash_S;

extern unsigned char Time_Second;
extern unsigned char Time_Minute;
extern unsigned char Time_Hour;
extern unsigned char Time_Day;
extern unsigned char Time_Month;
extern unsigned char Time_Year;
/******************AD_VALUE**********************************/
unsigned int readSingleData; //接受数据

unsigned char UC_C_Headcode_S;
unsigned char UC_C_Headcode2_S;
unsigned char UC_C_RECLengths_D;
unsigned char UC_C_FunctionCode_D;

unsigned char UC_URART2_RevFlag_S;
/************extern PARAM*************/
extern unsigned char UI_BaudRateStatus_SF;
extern unsigned char UI_MachineCode_DF;
extern unsigned char SendReady;
extern unsigned char SendKind;
extern unsigned char SendReady;
extern unsigned long int LI_ManualFlow_L_DF;

/************temp PARAM*************/
unsigned int UI_ManualFlow_H_Temp;
unsigned int UI_ManualFlow_L_Temp;

// 增加通信状态监控变量
unsigned char uart1_busy_flag = FALSE;


//extern unsigned long int aaa;
//extern unsigned long int bbb;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart1_interrupt_receive
* Description  : This function is INTSR1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart1_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR03 & 0x0007U);
    SIR03 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart1_callback_error(err_type);
    }
    
    rx_data = RXD1;

    if (g_uart1_rx_length > g_uart1_rx_count)
    {
        *gp_uart1_rx_address = rx_data;
        gp_uart1_rx_address++;
        g_uart1_rx_count++;

        if (g_uart1_rx_length == g_uart1_rx_count)
        {
            r_uart1_callback_receiveend();
        }
    }
    else
    {
        r_uart1_callback_softwareoverrun(rx_data);
    }
}

/***********************************************************************************************************************
* Function Name: r_uart1_interrupt_send
* Description  : This function is INTST1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart1_interrupt_send(void)
{
    if (g_uart1_tx_count > 0U)
    {
        TXD1 = *gp_uart1_tx_address;
        gp_uart1_tx_address++;
        g_uart1_tx_count--;
    }
}

/***********************************************************************************************************************
* Function Name: r_uart1_callback_receiveend
* Description  : This function is a callback function when UART1 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    
    UI_URT1_REC_C++;
    g_uart1_rx_count = 0;
    
    if(UI_BaudRateStatus_SF==BT_485CLOSE)
    {
	UART1_R_Rest();
	return;
    }
   
    if(UI_URT1_REC_C>=1)
    {
	    if(UI_URT1_REC_D[0]!=UI_MachineCode_DF)
	    {
		    UART1_R_Rest();
		    return;
	    }
    }
  
    // 设置忙标志，防止重复处理
    if(uart1_busy_flag==TRUE)
    {
        UART1_R_Rest();
        return;
    }
    uart1_busy_flag = TRUE;
    
    if(UI_URT1_REC_C>=2)
    {
	    UC_S_FunNum=UI_URT1_REC_D[1];
	    switch(UC_S_FunNum)
	    {
		//1:机器码 2:功能码 3:首地址H 4:首地址L 5:输入数量H 6:输入数量L 7:CRC_H 8:CRC_L
	    	case 0x04:
		case 0x03:
			if(UI_URT1_REC_C>7)
			{
				if(UI_URT1_REC_D[0]==UI_MachineCode_DF)
				{
					puch1=&(UI_URT1_REC_D[0]);
					crc_r=crc16(puch1,6);
					crc_H=(crc_r>>8)&0xff;
					crc_L=crc_r&0xff;
					if((UI_URT1_REC_D[6]==crc_L)&&(UI_URT1_REC_D[7]==crc_H))
					{
						address_H=UI_URT1_REC_D[2]<<8;
						address_L=UI_URT1_REC_D[3];
						address=address_H+address_L;
						
						if((address<1000)||(address>1032))
						{
							UART1_R_Rest();
							uart1_busy_flag = FALSE;
		    					return;
						}
						
						if(UI_URT1_REC_D[5]>33) //返回数据 16 最大长度
						{
							UART1_R_Rest();
							uart1_busy_flag = FALSE;
		    					return;
						}
						
						//call back 返回头码
						UI_URT1_SEND_D[0]=UI_URT1_REC_D[0];		//address num
						UI_URT1_SEND_D[1]=UI_URT1_REC_D[1];		//function num
						UI_URT1_SEND_D[2]=UI_URT1_REC_D[5]*2;		//byte sum
						
						for(i_for=0;i_for<UI_URT1_REC_D[5];i_for++)
						{
							readBackData();
							UI_URT1_SEND_D[i_for*2+3]=(readSingleData>>8)&0xFF;
							UI_URT1_SEND_D[i_for*2+4]=readSingleData&0xFF;
							address++;
						}
						puch1=&(UI_URT1_SEND_D[0]);
						crc_r=crc16(puch1,UI_URT1_SEND_D[2]+3);
						UI_URT1_SEND_D[UI_URT1_SEND_D[2]+3]=crc_r&0xff;
						UI_URT1_SEND_D[UI_URT1_SEND_D[2]+4]=(crc_r>>8)&0xff;					
						SendKind=0x04;
						SendReady=0xAA;
					}
					else
					{
						//CRC ERROR
						UC_S_ERR_CODE=0x01;
					}
				}
				UART1_R_Rest();
			}
			break;
		//1:机器码 2:功能码 3:首地址H 4:首地址L 5:数据H 6:数据L 7:CRC_H 8:CRC_L
		case 0x06:
			if(UI_URT1_REC_C>7)
			{
				if(UI_URT1_REC_D[0]==UI_MachineCode_DF)
				{
					puch1=&(UI_URT1_REC_D[0]);                                                   
					crc_r=crc16(puch1,6);
					crc_H=(crc_r>>8)&0xff;
					crc_L=crc_r&0xff;
					if((UI_URT1_REC_D[6]==crc_L)&&(UI_URT1_REC_D[7]==crc_H))
					{
						address_H=UI_URT1_REC_D[2]<<8;
						address_L=UI_URT1_REC_D[3];
						address=address_H+address_L;
						
						
						if((address<2000)||(address>2014))
						{
							UART1_R_Rest();
							uart1_busy_flag = FALSE;
		    					return;
						}
						
						UI_S_TempData_D=(UI_URT1_REC_D[4]<<8)+UI_URT1_REC_D[5];
						
						setParameter(address,UI_S_TempData_D);//通过地址修改对应的参数
						if(UC_S_Para_Invalid_D==1)
						{
							//Para_Invalid ERR
							UC_S_ERR_CODE=0x02;
							//trans_data_err(UC_S_FunNum);
							UC_S_Para_Invalid_D=0;
						}
						else
						{
							//call_back
							UI_URT1_SEND_D[0]=UI_URT1_REC_D[0];
							UI_URT1_SEND_D[1]=UI_URT1_REC_D[1];
							UI_URT1_SEND_D[2]=UI_URT1_REC_D[2];
							UI_URT1_SEND_D[3]=UI_URT1_REC_D[3];
							UI_URT1_SEND_D[4]=UI_URT1_REC_D[4];
							UI_URT1_SEND_D[5]=UI_URT1_REC_D[5];
							UI_URT1_SEND_D[6]=UI_URT1_REC_D[6];
							UI_URT1_SEND_D[7]=UI_URT1_REC_D[7];
							
							SendKind=0x06;
							
							SendReady=0xAA;
						}
					}
					else 
					{
						UC_S_ERR_CODE=0x01;
					}
				}
				UART1_R_Rest();
			}
			break;
		case 0x10:
			//1:机器码 2:功能码 (3+4):首地址 (5+6):寄存器数量H+L 7:字节数 (8...N):寄存器值 (N+1):CRC_H (N+2):CRC_L
			if(UI_URT1_REC_C>6&&temp_index_count<4)
			{
				temp_index_count=temp_index_count+UI_URT1_REC_D[6]+6;
			}
			if(temp_index_count>4)
			{
				if(UI_URT1_REC_C==temp_index_count)
				{
					if(UI_URT1_REC_D[0]==UI_MachineCode_DF)
					{
						puch1=&(UI_URT1_REC_D[0]);
						crc_r=crc16(puch1,UI_URT1_REC_C-2);
						crc_H=(crc_r>>8)&0xff;
						crc_L=crc_r&0xff;
						if((UI_URT1_REC_D[UI_URT1_REC_C-2]==crc_L)&&(UI_URT1_REC_D[UI_URT1_REC_C-1]==crc_H))
						{
							UART1_Stop_R();
							address_H=UI_URT1_REC_D[2]<<8;
							address_L=UI_URT1_REC_D[3];
							address=address_H+address_L;
							
							if((address<2000)||(address>2014))
							{
								UART1_R_Rest();
								uart1_busy_flag = FALSE;
			    					return;
							}
							
							byte_count=(UI_URT1_REC_D[4]<<8)+UI_URT1_REC_D[5];
							for(i_for=0;i_for<byte_count;i_for++)
							{
								UI_S_TempData_D=(UI_URT1_REC_D[2*i_for+7]<<8)+UI_URT1_REC_D[2*i_for+8];
								setParameter(address,UI_S_TempData_D);
								if(UC_S_Para_Invalid_D==1)
								{
									//Para_Invalid ERR
									UC_S_Para_Invalid_D=0;
								}	
								address=address+1;	
							}
							
								//call_back
							UI_URT1_SEND_D[0]=UI_URT1_REC_D[0];
							UI_URT1_SEND_D[1]=UI_URT1_REC_D[1];
							UI_URT1_SEND_D[2]=UI_URT1_REC_D[2];
							UI_URT1_SEND_D[3]=UI_URT1_REC_D[3];
							UI_URT1_SEND_D[4]=UI_URT1_REC_D[4];
							UI_URT1_SEND_D[5]=UI_URT1_REC_D[5];
							
							puch1=&(UI_URT1_REC_D[0]);
							crc_r=crc16(puch1,6);
							crc_H=(crc_r>>8)&0xff;
							crc_L=crc_r&0xff;
							UI_URT1_SEND_D[6]=crc_L;
							UI_URT1_SEND_D[7]=crc_H;
							
							SendKind=0x10;
							
							SendReady=0xAA;
						}
					}
					UART1_R_Rest();
				}
			}
			break;
		default:
			UART1_R_Rest();
			break;
	    }
    }
    
    // 清除忙标志
    uart1_busy_flag = FALSE;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart1_callback_softwareoverrun
* Description  : This function is a callback function when UART1 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    UART1_R_Rest();
    uart1_busy_flag = FALSE;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart1_callback_error
* Description  : This function is a callback function when UART1 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart1_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
     // 重置接收状态
    UART1_R_Rest();
    uart1_busy_flag = FALSE;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart2_interrupt_receive
* Description  : This function is INTSR2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart2_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR11 & 0x0007U);
    SIR11 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart2_callback_error(err_type);
    }
    
    rx_data = RXD2;

    if (g_uart2_rx_length > g_uart2_rx_count)
    {
        *gp_uart2_rx_address = rx_data;
        gp_uart2_rx_address++;
        g_uart2_rx_count++;

        if (g_uart2_rx_length == g_uart2_rx_count)
        {
            r_uart2_callback_receiveend();
        }
    }
    else
    {
        r_uart2_callback_softwareoverrun(rx_data);
    }
}

/***********************************************************************************************************************
* Function Name: r_uart2_interrupt_send
* Description  : This function is INTST2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart2_interrupt_send(void)
{
    if (g_uart2_tx_count > 0U)
    {
        TXD2 = *gp_uart2_tx_address;
        gp_uart2_tx_address++;
        g_uart2_tx_count--;
    }
    else
    {
        r_uart2_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart2_callback_receiveend
* Description  : This function is a callback function when UART2 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart2_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    UI_URT2_REC_C++;
    g_uart2_rx_count = 0;
    if(UI_URT2_REC_C>=2)
    {
	UC_C_Headcode_S=UI_URT2_REC_D[0];
	UC_C_Headcode2_S=UI_URT2_REC_D[1];
	if((UC_C_Headcode_S==0xC3)&&(UC_C_Headcode2_S==0x3C))
	{	
		if(UI_URT2_REC_C>=3)
    		{
			UC_C_RECLengths_D=UI_URT2_REC_D[2];
		
			if(UI_URT2_REC_C>=(UC_C_RECLengths_D+3))
			{
				puch=&(UI_URT2_REC_D[0]);
				
				crc_r=crc16(puch,UI_URT2_REC_C-2);
				crc_H=(crc_r>>8)&0xff;
				crc_L=crc_r&0xff;
				if(UI_URT2_REC_D[UI_URT2_REC_C-2]==crc_L&&UI_URT2_REC_D[UI_URT2_REC_C-1]==crc_H)
				{
					UC_URART2_RevFlag_S=TRUE;
				}
				UART2_R_Rest();
			}
		}
		
	}
	else 
	{
		UART2_R_Rest();	
	}
	
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart2_callback_softwareoverrun
* Description  : This function is a callback function when UART2 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart2_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart2_callback_sendend
* Description  : This function is a callback function when UART2 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart2_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    UART2_Stop_T();
    UART2_Start_R();
    UART2_R_Rest();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart2_callback_error
* Description  : This function is a callback function when UART2 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart2_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */


void UART1_T_Delay(void)
{
	UART1_Stop_R();
	UART1_R_Rest();
}

void UART1_R_Rest(void)
{
	temp_index_count=3;
	UI_URT1_REC_C=0;
	g_uart1_rx_count = 0;
	gp_uart1_rx_address = UI_URT1_REC_D;
}


void my_delay(unsigned int count)
{
	unsigned int temp = 0;
	while(count--){
		while((++temp)<2000);
		R_WDT_Restart();
		temp = 0;
	}
}

void trans_data_err(unsigned char kind)
{
	switch(kind)
	{
		case 0x03:	
			UI_URT1_SEND_D[0]=UI_URT1_REC_D[0];
			UI_URT1_SEND_D[1]=0x83;
			UI_URT1_SEND_D[2]=UC_S_ERR_CODE;
			break;
			
		case 0x04:	
			UI_URT1_SEND_D[0]=UI_URT1_REC_D[0];
			UI_URT1_SEND_D[1]=0x84;
			UI_URT1_SEND_D[2]=UC_S_ERR_CODE;
			break;
			
		case 0x06:	
			UI_URT1_SEND_D[0]=UI_URT1_REC_D[0];
			UI_URT1_SEND_D[1]=0x86;
			UI_URT1_SEND_D[2]=UC_S_ERR_CODE;
			break;
			
		case 0x10:	
			UI_URT1_SEND_D[0]=UI_URT1_REC_D[0];
			UI_URT1_SEND_D[1]=0x90;
			UI_URT1_SEND_D[2]=UC_S_ERR_CODE;
			break;
	}
	
	puch1=&(UI_URT1_SEND_D[0]);
	crc_r=crc16(puch,3);
	UI_URT1_SEND_D[3]=crc_r&0xff;
	UI_URT1_SEND_D[4]=(crc_r>>8)&0xff;
	
	UC_S_ERR_CODE=0;
	trans_data_485(UI_URT1_SEND_D,5);
}

/*****************************************************************
* Function Name: trans_data
* Description  : trans_data
* Arguments    : None
* Return Value : None
******************************************************************/
void trans_data(unsigned char *data,unsigned int len)
{
	/*
	unsigned int i;
        uint8_t *fp;
	fp = data;
	*/
	
	UART2_Stop_R();
	UART2_Start_T();
	
	R_UART2_Send(data,len);
	
	/*
	for(i=0;i<len;i++){
		R_UART2_Send(fp++,1);
		my_delay(3);
	}*/
}


void trans_data_485(unsigned char *data,unsigned int len)
{
	unsigned int i;
	uint8_t *fp;
	fp = data;
	
	UART1_Stop_R();
	
	RS485_Direction=RS485_T;
	
	my_delay(1);
	
	UART1_Start_T();
	
	my_delay(10);
	
	for(i=0;i<len;i++){
		R_UART1_Send(fp++,1);
		my_delay(2);
	}
	
	SendReady=0x00;
}

void UART2_R_Rest(void)
{
	UI_URT2_REC_C=0;
	gp_uart2_rx_address = UI_URT2_REC_D;
}


/* End user code. Do not edit comment generated here */
