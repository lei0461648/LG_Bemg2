#ifndef _GET_ADC_H
#define _GET_ADC_H

void ADGet4_20MA(void);
void ADGet0_5V(void);
void ADGet_Pressure(void);

void t_max(void);
void t_min(void);
void get_adv_1(char index);
unsigned int get_adv_36(char index);
void Get_OutPut(void);
void PID_Init(void);
void PID_Calc(float speed);
void CheckDiaphragmAlarm(void);
#endif