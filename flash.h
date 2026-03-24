#ifndef _FLASH_H
#define _FLASH_H

void ERROR_Handler(void);
void PFDL_Init(void);
void PFDL_Write(void);
void PFDL_Read(void);
void PFDL_Erase(void);
void F_Close(void);
//unsigned char PFDL_BlankCheck(unsigned int Flash_index,unsigned int Length);   
//unsigned char PFDL_Iverify(unsigned int Flash_index,unsigned int Length);

#endif