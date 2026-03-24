#include "flash.h"
#include "r_cg_wdt.h"
#include "iodefine.h"
#include "pfdl.h"

pfdl_descriptor_t my_pfdl_descriptor_t;         
pfdl_status_t my_pfdl_status_t;           
pfdl_request_t my_pfdl_request_t;  
unsigned char Execute_status; 

unsigned char UC_F_FlashData_A[188];
unsigned char UC_F_FlashDataSize_D=188;

void ERROR_Handler(void)
{
	//WDTE = 0xAC; 
	__nop();
}

void PFDL_Init(void)
{
	my_pfdl_descriptor_t.fx_MHz_u08 = 24;   				//1MHz, must be integer     
	my_pfdl_descriptor_t.wide_voltage_mode_u08 = 0x01;  			//00: full-speed mode, 01 wide voltage mode   
	my_pfdl_status_t = PFDL_Open((__near pfdl_descriptor_t*) &my_pfdl_descriptor_t);  
	if (my_pfdl_status_t != PFDL_OK) ERROR_Handler();   
}

void PFDL_Erase(void)  
{
	my_pfdl_request_t.index_u16 = 0x0000;       				//Erase地址      
	my_pfdl_request_t.command_enu = PFDL_CMD_ERASE_BLOCK;   		//Erase命令
	my_pfdl_status_t = PFDL_Execute(&my_pfdl_request_t);    		//Erase执行函数    	   	
	do 
	{
		my_pfdl_status_t = PFDL_Handler();
		R_WDT_Restart();
	}
	while(my_pfdl_status_t == PFDL_BUSY);     				//等待	Erase完毕
	if (my_pfdl_status_t != PFDL_OK) 
	{
		ERROR_Handler();      						//如果Erase失败、调用错处理函数（复位）	
	}
}

void PFDL_Write(void)
{
	my_pfdl_request_t.index_u16 = 0x0050;					//Write地址
	my_pfdl_request_t.data_pu08 = UC_F_FlashData_A;     			//Write缓存首地址
	my_pfdl_request_t.bytecount_u16 = UC_F_FlashDataSize_D;   		//Write数据长度
	my_pfdl_request_t.command_enu = PFDL_CMD_WRITE_BYTES;  			//Write命令
	my_pfdl_status_t = PFDL_Execute(&my_pfdl_request_t);      		//Write执行函数 
	do 
	{
		my_pfdl_status_t = PFDL_Handler();
		R_WDT_Restart();
	}
	while(my_pfdl_status_t == PFDL_BUSY);     				//等待Write完毕
	if (my_pfdl_status_t != PFDL_OK)  ERROR_Handler();     			//如果Write失败、调用错处理函数（复位）
}


void PFDL_Read(void)		
{
	my_pfdl_request_t.index_u16 = 0x0050;					//Read地址
	my_pfdl_request_t.data_pu08 = UC_F_FlashData_A;     			//Read缓存首地址
	my_pfdl_request_t.bytecount_u16 = UC_F_FlashDataSize_D;   		//Read数据长度
	my_pfdl_request_t.command_enu = PFDL_CMD_READ_BYTES;  			//Read命令
	my_pfdl_status_t = PFDL_Execute(&my_pfdl_request_t);      		//Read执行函数
	do 
	{
		my_pfdl_status_t = PFDL_Handler();
		R_WDT_Restart();
	}
	while(my_pfdl_status_t == PFDL_BUSY);     				//等待Read完毕
	if (my_pfdl_status_t != PFDL_OK)  ERROR_Handler();     			//如果Read失败、调用错处理函数（复位）
}

void F_Close(void)
{
	PFDL_Close();
}
/*
unsigned char PFDL_Iverify(unsigned int Flash_index,unsigned int Length)//Iverify函数
{
	my_pfdl_request_t.index_u16 = Flash_index;        			//Iverify地址
	my_pfdl_request_t.bytecount_u16 = Length; 				//Iverify数据长度
	my_pfdl_request_t.command_enu = PFDL_CMD_IVERIFY_BYTES;  		//Iverify命令
	my_pfdl_status_t = PFDL_Execute(&my_pfdl_request_t);      		//Iverify执行函数 
	do 
	{
		my_pfdl_status_t = PFDL_Handler();
	}
	while(my_pfdl_status_t == PFDL_BUSY);     				//等待Iverify完毕
	return (my_pfdl_status_t);  
}
*/


/*
unsigned char PFDL_BlankCheck(unsigned int Flash_index,unsigned int Length)
{
	my_pfdl_request_t.index_u16 = Flash_index;        
	my_pfdl_request_t.bytecount_u16 = Length;   
	my_pfdl_request_t.command_enu = PFDL_CMD_BLANKCHECK_BYTES;  
	my_pfdl_status_t = PFDL_Execute(&my_pfdl_request_t);      	
	do 
	{
		my_pfdl_status_t = PFDL_Handler();
	}
	while(my_pfdl_status_t == PFDL_BUSY);     	
	return (my_pfdl_status_t);   
}
*/
