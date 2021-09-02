#include "MS51_32K.h"
#include "definations.h"
#include "PWM.h"
// must define the periode register value in globle variable Periode_Reg_Value

unsigned int Initialise_PWM(unsigned int PWM_Freq)
{	
	long temp;
	long PWM_Freq_KHZ;
	unsigned int Period_Reg;
	ALL_PWM_CLOCK_FSYS;
	PWM0_EDGE_TYPE;
	PWM0_IMDEPENDENT_MODE;
	PWM0_CLOCK_DIV_1;
	set_PWM0CON0_CLRPWM;
	PWM_Freq_KHZ = 1000*((long)PWM_Freq);
	temp = (SYS_FREQ/PWM_Freq_KHZ);
	Period_Reg = (unsigned int)temp;
	SFRS = 0;
	PWM0PH = HIBYTE(Period_Reg);
	PWM0PL = LOBYTE(Period_Reg);
	//set_PWMRUN;
	set_PWM0CON0_LOAD;
	//while(LOAD);
	return Period_Reg;
}

unsigned int Initialise_PWM1(unsigned int PWM_Freq)
{
  long temp;
	long PWM_Freq_KHZ;
	unsigned int Period_Reg;
	PWM1_EDGE_TYPE;
	PWM1_IMDEPENDENT_MODE;
	PWM1_CLOCK_DIV_1;
	set_PWM1CON0_CLRPWM;
	PWM_Freq_KHZ = 1000*((long)PWM_Freq);
	temp = (SYS_FREQ/PWM_Freq_KHZ);
	Period_Reg = (unsigned int)temp;
	SFRS = 2;
  PWM0PH = HIBYTE(Period_Reg);
	PWM0PL = LOBYTE(Period_Reg);
	set_PWM1CON0_LOAD;
	return Period_Reg;
}

void PWM1_Enable(char Channel,char Duty_Cycle,unsigned int Periode_Reg_Value)
{
	unsigned int Set_Duty_Cycle = 0;
	float Temp;
	DISABLE_GLOBAL_INTERRUPT;
	Temp = ((float)Periode_Reg_Value * ((float)Duty_Cycle/100));
	Set_Duty_Cycle = ((unsigned int)Temp);
	switch(Channel)
	{
		case 1:
		P23_PUSHPULL_MODE;
		ENABLE_PWM1_CH0_P23_OUTPUT;
		SFRS = 2;
		PWM1C0H = HIBYTE(Set_Duty_Cycle);			
		PWM1C0L = LOBYTE(Set_Duty_Cycle);
		case 2:
		P22_PUSHPULL_MODE;
	  ENABLE_PWM1_CH1_P22_OUTPUT;
		SFRS = 2;
		PWM1C0H = HIBYTE(Set_Duty_Cycle);			
		PWM1C0L = LOBYTE(Set_Duty_Cycle);
	}
	
	set_PWM1CON0_LOAD ;
	set_PWM1CON0_PWMRUN;
	SFRS=0;
	set_PWM0CON0_PWM1RUN;
	ENABLE_GLOBAL_INTERRUPT;
}

void PWM1_Enable_int(char Channel,char Duty_Cycle,unsigned int Periode_Reg_Value)
{
	unsigned int Set_Duty_Cycle = 0;
	float Temp;
	DISABLE_GLOBAL_INTERRUPT;
	Temp = ((float)Periode_Reg_Value * ((float)Duty_Cycle/100));
	Set_Duty_Cycle = ((unsigned int)Temp);
	switch(Channel)
	{
		case 1:
		P23_PUSHPULL_MODE;
		ENABLE_PWM1_CH0_P23_OUTPUT;
		SFRS = 2;
		PWM1C0H = HIBYTE(Set_Duty_Cycle);			
		PWM1C0L = LOBYTE(Set_Duty_Cycle);
		case 2:
		P22_PUSHPULL_MODE;
	  ENABLE_PWM1_CH1_P22_OUTPUT;
		SFRS = 2;
		PWM1C0H = HIBYTE(Set_Duty_Cycle);			
		PWM1C0L = LOBYTE(Set_Duty_Cycle);
	}
	
	set_PWM1CON0_LOAD ;
	set_PWM1CON0_PWMRUN;
	SFRS=0;
	set_PWM0CON0_PWM1RUN;
	ENABLE_GLOBAL_INTERRUPT;
}


void PWM_Enable(char Channel ,char Duty_Cycle,unsigned int Periode_Reg_Value)
{
	unsigned int Set_Duty_Cycle = 0;
	float Temp;
	DISABLE_GLOBAL_INTERRUPT;
	Temp = ((float)Periode_Reg_Value * ((float)Duty_Cycle/100));
	Set_Duty_Cycle = ((unsigned int)Temp);
	SFRS = 0;
	switch(Channel)
	{
		case 1 :
			P12_PUSHPULL_MODE;
			clr_P1_2;
			ENABLE_PWM0_CH0_P12_OUTPUT;
			SFRS = 0;
			PWM0C0H = HIBYTE(Set_Duty_Cycle);			
			PWM0C0L = LOBYTE(Set_Duty_Cycle);
			break;
		case 2 :
			P14_PUSHPULL_MODE;
		  clr_P1_1;
			ENABLE_PWM0_CH1_P14_OUTPUT;
			//ENABLE_PWM0_CH1_P11_OUTPUT;
			SFRS = 0;
			PWM0C1H = HIBYTE(Set_Duty_Cycle);			
			PWM0C1L = LOBYTE(Set_Duty_Cycle);
			break;
	/*	case 3 :
			P10_PUSHPULL_MODE;
			clr_P1_0;
			ENABLE_PWM0_CH2_P10_OUTPUT;
			PWM0C2H = HIBYTE(Set_Duty_Cycle);			
			PWM0C2L = LOBYTE(Set_Duty_Cycle);
			break;
		case 4 :
			P00_PUSHPULL_MODE;
			clr_P0_0;
			ENABLE_PWM0_CH3_P00_OUTPUT;
			PWM0C3H = HIBYTE(Set_Duty_Cycle);			
			PWM0C3L = LOBYTE(Set_Duty_Cycle);
			break;
		case 5 :
			P01_PUSHPULL_MODE;
			clr_P0_1;
			ENABLE_PWM0_CH4_P01_OUTPUT;
			PWM0C4H = HIBYTE(Set_Duty_Cycle);			
			PWM0C4L = LOBYTE(Set_Duty_Cycle);
			break;
		case 6 :
			P03_PUSHPULL_MODE;
			clr_P0_3;
			ENABLE_PWM0_CH5_P03_OUTPUT;
			PWM0C5H = HIBYTE(Set_Duty_Cycle);			
			PWM0C5L = LOBYTE(Set_Duty_Cycle);
		  break;
	/*	default:
			ALL_PWM_OUTPUT_ENABLE;
			PWM0H = HIBYTE(Set_Duty_Cycle);			
			PWM0L = LOBYTE(Set_Duty_Cycle);
			PWM1H = HIBYTE(Set_Duty_Cycle);			
			PWM1L = LOBYTE(Set_Duty_Cycle);
			PWM2H = HIBYTE(Set_Duty_Cycle);			
			PWM2L = LOBYTE(Set_Duty_Cycle);
			PWM3H = HIBYTE(Set_Duty_Cycle);			
			PWM3L = LOBYTE(Set_Duty_Cycle);
			PWM4H = HIBYTE(Set_Duty_Cycle);			
			PWM4L = LOBYTE(Set_Duty_Cycle);
			PWM5H = HIBYTE(Set_Duty_Cycle);			
			PWM5L = LOBYTE(Set_Duty_Cycle);
	*/
	}
	set_PWM0CON0_LOAD;
	set_PWM0CON0_PWM0RUN;
	ENABLE_GLOBAL_INTERRUPT;
	//while(LOAD);
}

void PWM_Enable_int(char Channel ,char Duty_Cycle,unsigned int Periode_Reg_Value)
{
	unsigned int Set_Duty_Cycle = 0;
	float Temp;
	Temp = ((float)Periode_Reg_Value * ((float)Duty_Cycle/100));
	Set_Duty_Cycle = ((unsigned int)Temp);
	SFRS = 0;
	switch(Channel)
	{
		case 1 :
			P12_PUSHPULL_MODE;
			clr_P1_2;
			ENABLE_PWM0_CH0_P12_OUTPUT;
			SFRS = 0;
			PWM0C0H = HIBYTE(Set_Duty_Cycle);			
			PWM0C0L = LOBYTE(Set_Duty_Cycle);
			break;
		case 2 :
			P11_PUSHPULL_MODE;
		  clr_P1_1;
			ENABLE_PWM0_CH1_P11_OUTPUT;
			SFRS = 0;
			PWM0C1H = HIBYTE(Set_Duty_Cycle);			
			PWM0C1L = LOBYTE(Set_Duty_Cycle);
			break;
	/*	case 3 :
			P10_PUSHPULL_MODE;
			clr_P1_0;
			ENABLE_PWM0_CH2_P10_OUTPUT;
			PWM0C2H = HIBYTE(Set_Duty_Cycle);			
			PWM0C2L = LOBYTE(Set_Duty_Cycle);
			break;
		case 4 :
			P00_PUSHPULL_MODE;
			clr_P0_0;
			ENABLE_PWM0_CH3_P00_OUTPUT;
			PWM0C3H = HIBYTE(Set_Duty_Cycle);			
			PWM0C3L = LOBYTE(Set_Duty_Cycle);
			break;
		case 5 :
			P01_PUSHPULL_MODE;
			clr_P0_1;
			ENABLE_PWM0_CH4_P01_OUTPUT;
			PWM0C4H = HIBYTE(Set_Duty_Cycle);			
			PWM0C4L = LOBYTE(Set_Duty_Cycle);
			break;
		case 6 :
			P03_PUSHPULL_MODE;
			clr_P0_3;
			ENABLE_PWM0_CH5_P03_OUTPUT;
			PWM0C5H = HIBYTE(Set_Duty_Cycle);			
			PWM0C5L = LOBYTE(Set_Duty_Cycle);
		  break;
	/*	default:
			ALL_PWM_OUTPUT_ENABLE;
			PWM0H = HIBYTE(Set_Duty_Cycle);			
			PWM0L = LOBYTE(Set_Duty_Cycle);
			PWM1H = HIBYTE(Set_Duty_Cycle);			
			PWM1L = LOBYTE(Set_Duty_Cycle);
			PWM2H = HIBYTE(Set_Duty_Cycle);			
			PWM2L = LOBYTE(Set_Duty_Cycle);
			PWM3H = HIBYTE(Set_Duty_Cycle);			
			PWM3L = LOBYTE(Set_Duty_Cycle);
			PWM4H = HIBYTE(Set_Duty_Cycle);			
			PWM4L = LOBYTE(Set_Duty_Cycle);
			PWM5H = HIBYTE(Set_Duty_Cycle);			
			PWM5L = LOBYTE(Set_Duty_Cycle);
	*/
	}
	set_PWM0CON0_LOAD;
	set_PWM0CON0_PWM0RUN;
	//while(LOAD);
}

void PWM_Disable(char channel)
{
	DISABLE_GLOBAL_INTERRUPT;
  switch(channel)
	{
		case 1 :
			DISABLE_PWM0_CH0_P12_OUTPUT;
			clr_P1_2;
			break;
		case 2 :
			DISABLE_PWM0_CH1_P11_OUTPUT;
			clr_P1_1;
			break;
		case 3 :
			DISABLE_PWM0_CH2_P10_OUTPUT;
			clr_P1_0;
			break;
		case 4 :
			DISABLE_PWM0_CH3_P00_OUTPUT;
		  clr_P0_0;
			break;
		case 5 :
			DISABLE_PWM0_CH4_P01_OUTPUT;
		  clr_P0_1;
			break;
		case 6 :
			DISABLE_PWM0_CH5_P03_OUTPUT;
			clr_P0_3;
			break;
		default:
			DISABLE_ALL_PWM0_OUTPUT;
	}
	ENABLE_GLOBAL_INTERRUPT;
}


void PWM_Disable_int(char channel)
{
  switch(channel)
	{
		case 1 :
			DISABLE_PWM0_CH0_P12_OUTPUT;
			clr_P1_2;
			break;
		case 2 :
			DISABLE_PWM0_CH1_P11_OUTPUT;
			clr_P1_1;
			break;
		case 3 :
			DISABLE_PWM0_CH2_P10_OUTPUT;
			clr_P1_0;
			break;
		case 4 :
			DISABLE_PWM0_CH3_P00_OUTPUT;
		  clr_P0_0;
			break;
		case 5 :
			DISABLE_PWM0_CH4_P01_OUTPUT;
		  clr_P0_1;
			break;
		case 6 :
			DISABLE_PWM0_CH5_P03_OUTPUT;
			clr_P0_3;
			break;
		default:
			DISABLE_ALL_PWM0_OUTPUT;
	}
}

void PWM1_Disable(char channel)
{
	switch(channel)
	{
		case 1:
		DISABLE_PWM1_CH0_P23_OUTPUT;
		clr_P2_3;
		break;
		case 2:
		DISABLE_PWM1_CH1_P22_OUTPUT;
		clr_P2_2;
		break;
	}
}


void PWM1_Disable_int(char channel)
{
	switch(channel)
	{
		case 1:
		DISABLE_PWM1_CH0_P23_OUTPUT;
		clr_P2_3;
		break;
		case 2:
		DISABLE_PWM1_CH1_P22_OUTPUT;
		clr_P2_2;
		break;
	}
}

long map_1(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int map(int x, int in_min, int in_max, int out_min, int out_max) 
{
	int new_range;
	int old_range;
	float new_value;
	
	new_range = (out_max - out_min);
	old_range = (in_max - in_min);
  new_value = ((((float)x -in_min) * (float)new_range)/(float)old_range) + out_min;
	return (int)new_value;
}