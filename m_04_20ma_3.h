#ifndef _M_04_20MA_3_H
#define _M_04_20MA_3_H


#define M_0_20MATYPE		0x01
#define M_4_20MATYPE		0x02
#define M_0_20MAZTTYPE		0x03
#define M_0_20MAFTTYPE		0x04


void m_Run04_20MA(void);
void get_04_20MAPercent(void);
void CurrToPct0_20MA(unsigned int curr);
void CurrToPct4_20MA(unsigned int curr);
void Get_TargetFlowForPerMA(unsigned int PercentMA);
void Get0_20MAZT(unsigned int I1,unsigned int Q1,unsigned int I2,unsigned int Q2,unsigned int curr);
void Get0_20MAFT(unsigned int I1,unsigned int Q1,unsigned int I2,unsigned int Q2,unsigned int curr);

#endif
