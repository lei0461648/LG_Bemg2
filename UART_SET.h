#ifndef _UART_SET_H
#define _UART_SET_H


void UART1_CREATE(unsigned char b_type);
void UART1_Start_R(void);
void UART1_Start_T(void);
void UART1_Stop_R(void);
void UART1_Stop_T(void);
void INIT_UART1(void);
void INIT_UART2(void);
unsigned int crc16(unsigned char *puchMsg, int usDataLen);

void UART2_Stop_R(void);
void UART2_Stop_T(void);
void UART2_Start_T(void);
void UART2_Start_R(void);

#endif