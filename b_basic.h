#ifndef _B_BASIC_H
#define _B_BASIC_H

void RatedFlow(unsigned long int calibVol);
void Get_MaximumFlowML(void);
void get_Percent(unsigned long int targetFlowML_H2);
void get_PercentMA(unsigned int PercentMA);
void getPWMForPercent(unsigned int Percent);

void setPWMPulseWidth(void);
void setSuckFastAndPushSlow(void);

#endif
