#ifndef _PWM_H_
#define _PWM_H_


unsigned int Initialise_PWM(unsigned int PWM_Freq);
void PWM_Enable(char Channel ,char Duty_Cycle,unsigned int Periode_Reg_Value);
void PWM_Disable(char channel);
void PWM_Enable_int(char Channel ,char Duty_Cycle,unsigned int Periode_Reg_Value);
void PWM_Disable_int(char channel);
unsigned int Initialise_PWM1(unsigned int PWM_Freq);
void PWM1_Enable(char Channel ,char Duty_Cycle,unsigned int Periode_Reg_Value);
void PWM1_Disable(char channel);
void PWM1_Enable_int(char Channel ,char Duty_Cycle,unsigned int Periode_Reg_Value);
void PWM1_Disable_int(char channel);
int map(int x, int in_min, int in_max, int out_min, int out_max);
long map_1(long x, long in_min, long in_max, long out_min, long out_max);

#endif