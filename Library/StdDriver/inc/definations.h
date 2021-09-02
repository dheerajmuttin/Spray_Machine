#include "MS51_32K.h"

#define CYCLE_ON 0
#define CYCLE_COMP 1
#define CYCLE_STOP 2
#define FORWORD 0
#define REVERSE 1
#define ON 1
#define OFF 0
#define RELESE 1
#define PRESS 0

#define Forword_Dir clr_P1_3;
#define Reverse_Dir set_P1_3;
#define MTR_SPEED_CTL 9
#define PUMP_SPEED_CTL 12
#define MTR_PWM	1 // channel 0
#define PUMP_PWM 0 // channel 1
#define SYS_FREQ 24000000L
#define Time_Constant 0.000000166


//sbit Forword_Switch = P2^5;
//sbit Reverse_Swith = P1^5;

enum HCSR04_STAGE {
NOP,
TRIGGERED,
CAPTURED,
COMPLETE,
TIMER_OVERFLOW,
ERROR
};

typedef struct Travel_Status{
 unsigned char Start;
 unsigned char Direction;
 unsigned char Start_Sensor;
 unsigned char End_Sensor;
 unsigned char Pump_Status;
}TRAVEL;

void Initilisation();
void Reset_Travel_Staus(TRAVEL *Cart);
void trigger_2();
void trigger_1();
void UART0_Timer3(unsigned long u32Baudrate);
void Update_Display(unsigned char channel,unsigned char value);
void Get_Pump_Speed();
void Get_Motor_Speed();
void Reset_Travel_Staus(TRAVEL *Cart);