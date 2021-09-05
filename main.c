#include "MS51_32K.h"
#include "definations.h"
#include "PWM.h"

sbit MT_Forword_Switch  = P2^5;
sbit MT_Reverse_Switch = P1^5;

enum HCSR04_STAGE Sensor_1 = NOP;
enum HCSR04_STAGE Sensor_2 = NOP;

TRAVEL Cart;
char chart_status = CYCLE_COMP;
unsigned int PWM0_Periode;
unsigned int PWM1_Periode;
unsigned int MOTOR_SPEED=0;
unsigned int PUMP_SPEED=0;
unsigned char Pump_status = OFF;
unsigned char Forword_Switch_Status = RELESE;
unsigned char Reverse_Swith_Status = RELESE;

unsigned char Water_Level;
unsigned int Distance;
unsigned char display_vlaue[3]={0x00,0x00,0x00};
unsigned char High_Byte,Low_Byte;
unsigned int Word;
float Echo_Time;

char Pre_FWD_Status = RELESE;
char Pre_RVS_Status = RELESE;


void main()
{
	MODIFY_HIRC(HIRC_24);
  Reset_Travel_Staus(&Cart);
	PWM0_Periode = Initialise_PWM(1);
	PWM1_Periode = Initialise_PWM1(1);
	UART0_Timer3(115200);
	Initilisation();
	
	while(1)
	{
		Get_Motor_Speed();
		Get_Pump_Speed();
		
/*		if (Cart.Start != CYCLE_ON)
		{
			if(Reverse_Swith == 0 && Reverse_Swith_Status == RELESE)
			{
				Reverse_Dir;
				PWM_Enable(2,MOTOR_SPEED,PWM0_Periode);
				Reverse_Swith_Status = PRESS;
			}
			else if(Reverse_Swith == 1 && Reverse_Swith_Status == PRESS)
			{
				PWM_Disable(2);
				Reverse_Swith_Status = RELESE;
			}
			else if(Forword_Switch == 0 && Forword_Switch_Status == RELESE)
			{
				Forword_Dir;
				PWM_Enable(2,MOTOR_SPEED,PWM0_Periode);
				Forword_Switch_Status = PRESS;
			}
		
			else if(Forword_Switch == 0 && Forword_Switch_Status == PRESS)
			{
				PWM_Disable(2);
				Forword_Switch_Status = RELESE;
			}
		}
		*/
		
		if (chart_status/*Cart.Start*/ == CYCLE_ON)
		{
			if(Cart.Direction == FORWORD && Cart.End_Sensor == 1)
			{
				PWM_Disable(2);
				Timer0_Delay(SYS_FREQ,1000,1000);
				Cart.Direction = REVERSE;
				Reverse_Dir;
				PWM_Enable(2,MOTOR_SPEED,PWM0_Periode);
			}
			if(Cart.Direction == REVERSE && Cart.Start_Sensor == 1)
			{
				PWM_Disable(2);
				PWM1_Disable(1);
				Cart.Direction = FORWORD;
				Cart.Start_Sensor = 0;
				Cart.End_Sensor = 0;
				//Cart.Start = CYCLE_COMP;
				chart_status = CYCLE_COMP;
				
			}
		}
		
		if (Sensor_1 == COMPLETE)
		{
			// calculate time distance and hight of water using paramters 
			Sensor_1 = NOP;
		}
		
			if (Sensor_2 == COMPLETE)
		{
			// calculate time distance and hight of water using paramters 
			SFRS = 0;
			Low_Byte = C1L;
			High_Byte = C1H;
			Word = MAKEWORD(High_Byte,Low_Byte);
			Echo_Time = (float)Word * Time_Constant;
			Distance = Echo_Time/58;
			Sensor_2 = NOP;
			Update_Display(1,Water_Level);
		}
		
		if(MT_Forword_Switch == PRESS && Pre_FWD_Status == RELESE && MT_Reverse_Switch == RELESE)
		{
			Pre_FWD_Status = PRESS;
			Forword_Dir;
			PWM_Enable(2,MOTOR_SPEED,PWM0_Periode);
		}
		if (MT_Forword_Switch == RELESE && Pre_FWD_Status == PRESS && MT_Reverse_Switch == RELESE)
		{
			Pre_FWD_Status = RELESE;
			PWM_Disable(2);
		}
			if(MT_Forword_Switch == RELESE && Pre_RVS_Status == RELESE && MT_Reverse_Switch == PRESS)
		{
			Pre_RVS_Status = PRESS;
			Forword_Dir;
			PWM_Enable(2,MOTOR_SPEED,PWM0_Periode);
		}
		if (MT_Forword_Switch == RELESE && Pre_RVS_Status == PRESS && MT_Reverse_Switch == RELESE)
		{
			Pre_RVS_Status = RELESE;
			Reverse_Dir;
			PWM_Disable(2);
		}
		
		if (MT_Forword_Switch == RELESE && MT_Reverse_Switch == RELESE)
		{
			PWM_Disable(2);
		}
	}//while end
}// main end 

void Initilisation()
{
	P13_PUSHPULL_MODE; // dir switch
	clr_P1_3;
	P03_INPUT_MODE; // proxy 1
	P04_INPUT_MODE; // proxy 2
	P30_INPUT_MODE; // switch 1 INT0  external intrrupt emergency stop switch
	P05_INPUT_MODE; //proxy 3
	P01_INPUT_MODE; //proxy 4
	P15_INPUT_MODE; //switch 5 pin change interrupt
	P25_INPUT_MODE; // swtich 4 pin change interrupt
	P36_INPUT_MODE; // swtich 2 pin change interrupt
	P37_INPUT_MODE; // swtich 3 pin change interrupt
	P32_INPUT_MODE;// swtich 6 pin change interrupt
	P13_PUSHPULL_MODE; // buzzer outpute
	clr_P1_3; // clear the buzzer outpute
	P11_PUSHPULL_MODE; // trigger 1
	P10_PUSHPULL_MODE; // trigger 2
	clr_P1_1; // clear the trigger 1
	clr_P0_0; // clear the trigger 2
	
	INT0_FALLING_EDGE_TRIG;
	ENABLE_INT0_INTERRUPT; // emergency stop button on interrupt external switch 1
	//ENABLE_TIMER2_INTERRUPT; // timer2 overflow detection for sensor checking
	//ENABLE_CAPTURE_INTERRUPT; // capture interrupt detection
	//ENABLE_WKT_INTERRUPT; // wakeup timer interrupt for the time counting
	ENABLE_PIT0_P03_FALLINGEDGE; // enable the pin interrupt 0 on the P03 pin; proxy 1
	ENABLE_PIT1_P04_FALLINGEDGE;// enable the pin interrupt 1 on the P04 pin; proxy 2
	ENABLE_PIT2_P36_FALLINGEDGE;// enable the pin interrupt 2 on the P36 pin; switch 2;
	ENABLE_PIT3_P37_FALLINGEDGE;//enable the pin interrupt 3 on the P37 pin; switch 3;
	ENABLE_PIN_INTERRUPT; //  pinchange interrupt for button pressed 
	
	// timer 2 initialization divide by 4 timer
	clr_T2CON_CMRL2;
	TIMER2_DIV_4; // 24/4=6MHZ frequency
	clr_T2MOD_LDEN;
	IC0_P12_CAP0_BOTHEDGE_CAPTURE;//echo 1
	IC3_P00_CAP1_BOTHEDGE_CAPTURE; // echo 2 both edge capture
	set_T2MOD_CAPCR;
	ENABLE_GLOBAL_INTERRUPT; // globle interrupt enable pin
}



void trigger_1()
{
	// 10us high pin trigger 
	set_P1_1;
	Timer0_Delay(SYS_FREQ,1,100);
  Sensor_1 = TRIGGERED;
	Sensor_2 = NOP;
	// and set the status byte enum data type 
	//set reset timer to apropriate ccp module in t2mod last bits
}

void trigger_2()
{
	// 10us high pin trigger 
	set_P1_0;
	Timer0_Delay(SYS_FREQ,1,100);
	clr_P1_0;
	Sensor_2 = TRIGGERED;
	Sensor_1 = NOP;
	// and set the status byte enum data type 
	//set reset timer to apropriate ccp module in t2mod last bits
}
void Reset_Travel_Staus(TRAVEL *Cart)
{
	Cart->Start = CYCLE_COMP;
	Cart->Direction = FORWORD;
	Cart->Start_Sensor = 0;
	Cart->End_Sensor = 0;
}

void Get_Motor_Speed()
	
{
	unsigned int Current_speed = 0;
	unsigned int ADC_reding = 0;
	ENABLE_ADC_CH12;
	clr_ADCCON0_ADCF;
  set_ADCCON0_ADCS;                  // ADC start trig signal
  while(ADCF == 0);
  ADC_reding =(ADCRH<<4)|(ADCRL&0x0F);
	//Current_speed = (int)map_1(ADC_reding,0,4095,0,100);
	Current_speed = map(ADC_reding,0,4095,0,100);
	
	if (Current_speed != MOTOR_SPEED )
	{
		MOTOR_SPEED = Current_speed;
		Update_Display(2,MOTOR_SPEED);
	}
}

void Get_Pump_Speed()
{
	unsigned int Current_speed1 = 0;
	unsigned int ADC_reding = 0;
	ENABLE_ADC_CH9;
	clr_ADCCON0_ADCF;
  set_ADCCON0_ADCS;                  // ADC start trig signal
  while(ADCF == 0);
  ADC_reding =(ADCRH<<4)|(ADCRL&0x0F);
	//Current_speed1 = (int)map_1(ADC_reding,0,4095,0,100);
	
	Current_speed1 = (int)map(ADC_reding,0,4095,0,100);
	if (Current_speed1 != PUMP_SPEED)
	{
		MOTOR_SPEED = Current_speed1;
		if (Pump_status == ON)
		{
			PWM1_Enable(1,PUMP_SPEED,PWM1_Periode);
			Update_Display(3,PUMP_SPEED);
		}
	}
}


void Update_Display(unsigned char channel,unsigned char value)
{
	char i;
	switch (channel)
	{
		case 1:
		display_vlaue[0] = value;
		break;
		case 2:
		display_vlaue[1] = value;
		break;
		case 3:
		display_vlaue[2] = value;
	}
	
	for(i=0;i<3;i++) //  sending to uart remote diplay for showing the status
	{
					TI=0;
          SBUF = display_vlaue[i];
          while(!TI);
	}
	
}

void UART0_Timer3(unsigned long u32Baudrate)
{
						SCON = 0x50;          //UART0 Mode1,REN=1,TI=1
            set_PCON_SMOD;        //UART0 Double Rate Enable
            T3CON &= 0xF8;        //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
            set_T3CON_BRCK;        //UART0 baud rate clock source = Timer3
            RH3   = HIBYTE(65536 - (SYS_FREQ / 16 / u32Baudrate));
            RL3   = LOBYTE(65536 - (SYS_FREQ / 16 / u32Baudrate));
            set_T3CON_TR3;         //Trigger Timer3
            clr_IE_ES;
}


void INT0_ISR(void) interrupt 0          // Vector @  0x03
{
	DISABLE_GLOBAL_INTERRUPT;
    _push_(SFRS);
  //emergency stop button function written
		PWM_Disable_int(2);
		PWM1_Disable_int(1);
		//Cart.Start = CYCLE_STOP;
	    chart_status = CYCLE_STOP;
	
	// set all the satus regarding direction 
	// and emergency stop is detected then start cycle from previous memory status
		clr_TCON_IE0;
    _pop_(SFRS);
	ENABLE_GLOBAL_INTERRUPT;
}

void Pin_INT_ISR(void) interrupt 7       // Vector @  0x3B
{
	unsigned char temp;
	DISABLE_GLOBAL_INTERRUPT;
  _push_(SFRS);
	temp = PIF;
	if(temp&SET_BIT0)
	{
		// proxy 1 Sensor Detection
		//if(Cart.Start == CYCLE_ON)
			if(chart_status == CYCLE_ON)
		{
		Cart.Start_Sensor = 1;
		}
	}
	
	temp = PIF;
	if(temp&SET_BIT1)
	{
		//if(Cart.Start == CYCLE_ON)
			if(chart_status == CYCLE_ON)
		{
		 // proxy 2 Sensor Detection
		Cart.End_Sensor = 1;
		}
			
	}
	
	temp = PIF;
	if(temp&SET_BIT2)
	{
		// switch 2 action defination start button cycle
		if(Cart.Start != CYCLE_ON)
		{
			if(Cart.Direction == FORWORD)
			{
				Forword_Dir;
				//Cart.Direction = FORWORD;
			}
			{
				Reverse_Dir;
				//Cart.Direction = REVERSE;
			}
			PWM_Enable_int(2,MOTOR_SPEED,PWM0_Periode);
			//Cart.Start = 0x0  CYCLE_ON;
			chart_status = 0x00;
			Pump_status = ON; // manual checking status of pump cleared when pump status is on
			PWM1_Enable_int(1,PUMP_SPEED,PWM1_Periode);
		}
	}
	
	temp = PIF;
	if(temp&SET_BIT3)
	{
		if(Pump_status != ON)
		{
			PWM1_Enable_int(1,PUMP_SPEED,PWM1_Periode);
			Pump_status = ON;
		}
		
		else
		{
			PWM1_Disable_int(1);
			Pump_status = OFF;
		}
		
		// switch 3 action defination   pump start stop manully 
	}
	   PIF = 0x00;
    _pop_(SFRS);
	ENABLE_GLOBAL_INTERRUPT;
}



void Capture_ISR(void) interrupt 12      // Vector @  0x63
{
	unsigned char temp;
	DISABLE_GLOBAL_INTERRUPT;
    _push_(SFRS);
			SFRS=0;
	temp = CAPCON0;
	if(temp&SET_BIT0)
	{
		if(Sensor_1 == TRIGGERED)
		{
			Sensor_1 = CAPTURED;
			C0L = 0x00;
			C0H = 0x00;
		}
		if(Sensor_1 == CAPTURED)
		{
			Sensor_1 = COMPLETE;
			clr_T2CON_TR2; // stope the timer2 from counting
		}
	}
	temp = CAPCON0;
	if(temp&SET_BIT1)
	{
		if(Sensor_2 == TRIGGERED)
		{
			Sensor_2 = CAPTURED;
			C0L = 0x00;
			C0H = 0x00;
		}
		if(Sensor_2 == CAPTURED)
		{
			Sensor_2 = COMPLETE;
			clr_T2CON_TR2; // stope the timer2 from counting
		}
	}
		clr_CAPCON0_CAPF0;
    clr_CAPCON0_CAPF1;
    _pop_(SFRS);
	ENABLE_GLOBAL_INTERRUPT;
}

void Timer1_ISR(void) interrupt 3        // Vector @  0x1B
{
    _push_(SFRS);
   clr_TCON_TF1;
    _pop_(SFRS);
    _pop_(SFRS);
}


void WKT_ISR(void) interrupt 17                 // Vector @  0x8B
{
    _push_(SFRS);
    clr_WKCON_WKTF;
    _pop_(SFRS);
}