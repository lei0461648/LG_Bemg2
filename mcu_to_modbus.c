#include "r_cg_macrodriver.h"
#include "r_cg_wdt.h"
#include "get_screen.h"
#include "var_define.h"
#include "UART_SET.h"
#include "r_cg_serial.h"

extern  uint8_t UI_URT2_REC_D[];
unsigned char UC_URART2_TxDataArray_A[100];
extern unsigned char UC_C_FunctionCode_D;
extern unsigned int crc_r;
extern unsigned char crc_H;	
extern unsigned char crc_L;
extern unsigned char *puch;

/************************************************
* Function Name: ADGetEC  
* Description  : Get electric current
* Arguments    : None
* Return Value : None
*************************************************/
void SendDataToBoard(void)
{
	UC_C_FunctionCode_D=UI_URT2_REC_D[3];
	switch(UC_C_FunctionCode_D)
	{
		case InitializeParam://0x00
		
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x06;//leagth
			UC_URART2_TxDataArray_A[4]=0x04;//mode
			UC_URART2_TxDataArray_A[5]=TRUE;//ON/OFF
			UC_URART2_TxDataArray_A[6]=0x00;//
			
			CreateCRC_TX(7);
			break;
		case HandModeParam://0x01
		
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x0D;//leagth
			
			UC_URART2_TxDataArray_A[4]=0x01;//Target traffic
			UC_URART2_TxDataArray_A[5]=0x45;//
			
			UC_URART2_TxDataArray_A[6]=0x01;//Real-time traffic
			UC_URART2_TxDataArray_A[7]=0x5E;//
			
			UC_URART2_TxDataArray_A[8]=0x03;//percent
			UC_URART2_TxDataArray_A[9]=0xB6;//
			
			UC_URART2_TxDataArray_A[10]=0x00;//Alarm message
			UC_URART2_TxDataArray_A[11]=0x00;//RunStatus
			
			UC_URART2_TxDataArray_A[12]=0x00;//activation feature
			UC_URART2_TxDataArray_A[13]=0x01;//1 l/h 2 ml/h
			
			CreateCRC_TX(14);
			break;
		case PulseModeParam://0x02
		
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x0E;//leagth
			
			UC_URART2_TxDataArray_A[4]=0x00;//pulse flow
			UC_URART2_TxDataArray_A[5]=0x45;
			UC_URART2_TxDataArray_A[6]=0x01;//1£ºl  2£º ml  Pulse flow units
			UC_URART2_TxDataArray_A[7]=0x03;//percent
			UC_URART2_TxDataArray_A[8]=0xB6;
			UC_URART2_TxDataArray_A[9]=0x00;//RunStatus
			UC_URART2_TxDataArray_A[10]=0x01;//Target traffic
			UC_URART2_TxDataArray_A[11]=0x45;
			UC_URART2_TxDataArray_A[12]=0x01;//1 l/h 2 ml/h
			UC_URART2_TxDataArray_A[13]=0x00;//Alarm message
			UC_URART2_TxDataArray_A[14]=0x00;//activation feature
			
			CreateCRC_TX(15);
			break;
			
		case Mode_4_20MAParam://0x03
		
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x0F;//leagth
			
			UC_URART2_TxDataArray_A[4]=0x00;//Target traffic
			UC_URART2_TxDataArray_A[5]=0x50;
			UC_URART2_TxDataArray_A[6]=0x01;//1 l/h 2 ml/h
			UC_URART2_TxDataArray_A[7]=0x03;//percent
			UC_URART2_TxDataArray_A[8]=0xB6;
			UC_URART2_TxDataArray_A[9]=0x00;//RunStatus
			UC_URART2_TxDataArray_A[10]=0x0A;//mA1
			UC_URART2_TxDataArray_A[11]=0x14;//mA2
			UC_URART2_TxDataArray_A[12]=0x05;//Input Current
			UC_URART2_TxDataArray_A[13]=0xF0;
			UC_URART2_TxDataArray_A[14]=0x00;//Alarm message
			UC_URART2_TxDataArray_A[15]=0x00;//activation feature
			
			CreateCRC_TX(16);
			break;
		case BatchPulseParam://0x04
		
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x0C;//leagth
			
			UC_URART2_TxDataArray_A[4]=0x00;//Batch volume
			UC_URART2_TxDataArray_A[5]=0xE0;
			UC_URART2_TxDataArray_A[6]=0x01;//1 l 2 ml
			UC_URART2_TxDataArray_A[7]=0x00;//Remaining batch volume
			UC_URART2_TxDataArray_A[8]=0x50;
			UC_URART2_TxDataArray_A[9]=0x01;//1 l 2 ml
			UC_URART2_TxDataArray_A[10]=0x00;//RunStatus
			UC_URART2_TxDataArray_A[11]=0x00;//Alarm message
			UC_URART2_TxDataArray_A[12]=0x00;//activation feature
			
			CreateCRC_TX(13);
			break;
		case CyclicDosingParam://0x05
		
		 	TxDataHeader();//Head
			UC_URART2_TxDataArray_A[2]=0x0E;//leagth
			
			UC_URART2_TxDataArray_A[4]=0x00;//Batch volume
			UC_URART2_TxDataArray_A[5]=0xE0;
			UC_URART2_TxDataArray_A[6]=0x01;//1 l 2 ml
			UC_URART2_TxDataArray_A[7]=0x00;//Remaining batch volume
			UC_URART2_TxDataArray_A[8]=0x50;
			UC_URART2_TxDataArray_A[9]=0x01;//1 l 2 ml
			UC_URART2_TxDataArray_A[10]=0x00;//RunStatus
			UC_URART2_TxDataArray_A[11]=0x01;//Time
			UC_URART2_TxDataArray_A[12]=0x0A;
			UC_URART2_TxDataArray_A[13]=0x00;//Alarm message
			UC_URART2_TxDataArray_A[14]=0x00;//activation feature
			
			CreateCRC_TX(15);
			break;
	}
}


/************************************************
* Function Name: TxDataHeader  
* Description  : 
* Arguments    : None
* Return Value : None
*************************************************/
void TxDataHeader(void)
{
	UC_URART2_TxDataArray_A[0]=UI_URT2_REC_D[0];
	UC_URART2_TxDataArray_A[1]=UI_URT2_REC_D[1];
	UC_URART2_TxDataArray_A[3]=UI_URT2_REC_D[3];
}


/************************************************
* Function Name: TxDataHeader  
* Description  : 
* Arguments    : None
* Return Value : None
*************************************************/
void CreateCRC_TX(unsigned int arraySize)
{
	puch=&(UC_URART2_TxDataArray_A[0]);
	crc_r=crc16(puch,arraySize);
	crc_H=(crc_r>>8)&0xff;
	crc_L=crc_r&0xff;
	
	UC_URART2_TxDataArray_A[arraySize]=crc_L;
	UC_URART2_TxDataArray_A[arraySize+1]=crc_H;

	trans_data(UC_URART2_TxDataArray_A,(arraySize+2));
	
	//memset(UC_URART3_RevDataArray_A,0x00,256);//
	//clearArray
	/*
	for (i = 0; i < 100; i++) 
	{
    		UC_URART2_TxDataArray_A[i] = 0;
	}*/

}

