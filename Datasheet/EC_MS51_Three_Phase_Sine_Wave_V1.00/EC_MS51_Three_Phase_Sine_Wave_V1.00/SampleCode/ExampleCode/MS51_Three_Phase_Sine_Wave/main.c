/**************************************************************************//**
 * @file     main.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 19/10/24 8:14p $
 * @brief    Use MS51 PWM Channel 0 (P1.2), Channel 1 (P1.1) and Channel 2 (P1.0)
 *           to generate 60 Hz three phase sine wave for application
 *
 * @note
 * Copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include "MS51_16K.H"
#include <math.h>

/*---------------------------------------------------------------------------*/
/* Define                                                                    */
/*---------------------------------------------------------------------------*/
/******************************************************************
  Time-out period = (0xFF - TIMER0_VALUE) / clock frequency
                  = 1 / (sine wave frequency * sine wave resolution)
  TIMER0_VALUE = 0xFF - ((16 MHz / 12) / (60 Hz * 360) = 0xC2
******************************************************************/
#define TIMER0_VALUE            0xC2
#define Pi_Value                3.14159
/******************************************************************
  PWM frequency = Fsys / PWM_Period
                = 16 MHz / 800 = 20 kHz
******************************************************************/
#define PWM_Period              800

/*---------------------------------------------------------------------------*/
/* Global variables                                                          */
/*---------------------------------------------------------------------------*/
UINT16 xdata g_au16Sine[360];
UINT16 data g_u16Sine_Count_u = 0;
UINT16 data g_u16Sine_Count_v = 120;
UINT16 data g_u16Sine_Count_w = 240;

/*---------------------------------------------------------------------------*/
/* Functions                                                                 */
/*---------------------------------------------------------------------------*/
void Timer0_ISR(void) interrupt 1
{
    /* Set PWM new setting according to sine phase from lookup table */
    PWM0H = HIBYTE(g_au16Sine[g_u16Sine_Count_u]);
    PWM0L = LOBYTE(g_au16Sine[g_u16Sine_Count_u]);
    PWM1H = HIBYTE(g_au16Sine[g_u16Sine_Count_v]);
    PWM1L = LOBYTE(g_au16Sine[g_u16Sine_Count_v]);
    PWM2H = HIBYTE(g_au16Sine[g_u16Sine_Count_w]);
    PWM2L = LOBYTE(g_au16Sine[g_u16Sine_Count_w]);

    /* Add sine array pointer */
    g_u16Sine_Count_u++;
    g_u16Sine_Count_v++;
    g_u16Sine_Count_w++;

    /* Reset sine array pointer */
    if (g_u16Sine_Count_u == 360)
        g_u16Sine_Count_u = 0;

    if (g_u16Sine_Count_v == 360)
        g_u16Sine_Count_v = 0;

    if (g_u16Sine_Count_w == 360)
        g_u16Sine_Count_w = 0;

    /* Load PWM new setting and start PWM */
    set_PWMCON0_LOAD;
    set_PWMCON0_PWMRUN;
}

void main(void)
{
    UINT16 i;

    /* Create sine table for lookup */
    for (i = 0; i < 360; i++)
        /* Add 1.0 to offset sine result from [-1, 1] to [0, 2],
        and divide with 2.0 to compress to [0, 1] */
        g_au16Sine[i] = ((sin((i * Pi_Value) / 180.0) + 1.0) / 2.0) * PWM_Period;

    /* Enable Timer0 Mode 2 (8-bit Timer Auto-reload) */
    TIMER0_MODE2_ENABLE;
    TIMER0_FSYS_DIV12;
    /* Set Timer0 counter value */
    TH0 = TL0 = TIMER0_VALUE;
    /* Enable Timer0 interrupt */
    ENABLE_TIMER0_INTERRUPT;
    /* Enable Global Interrupt */
    ENABLE_GLOBAL_INTERRUPT;

    /* Set GPIO P1.2, P1.1 and P1.0 as Push-pull mode */
    P12_PUSHPULL_MODE;
    P11_PUSHPULL_MODE;
    P10_PUSHPULL_MODE;
    /* Enable PWM Channel 0, Channel 1 and Channel 2 output */
    PWM0_P12_OUTPUT_ENABLE;
    PWM1_P11_OUTPUT_ENABLE;
    PWM2_P10_OUTPUT_ENABLE;
    /* Set PWM as independent mode */
    PWM_IMDEPENDENT_MODE;
    /* Set PWM Period */
    PWMPH = HIBYTE(PWM_Period);
    PWMPL = LOBYTE(PWM_Period);
    /* Set PWM Channel 0 Duty */
    PWM0H = HIBYTE(g_au16Sine[g_u16Sine_Count_u]);
    PWM0L = LOBYTE(g_au16Sine[g_u16Sine_Count_u]);
    /* Set PWM Channel 1 Duty */
    PWM1H = HIBYTE(g_au16Sine[g_u16Sine_Count_v]);
    PWM1L = LOBYTE(g_au16Sine[g_u16Sine_Count_v]);
    /* Set PWM Channel 2 Duty */
    PWM2H = HIBYTE(g_au16Sine[g_u16Sine_Count_w]);
    PWM2L = LOBYTE(g_au16Sine[g_u16Sine_Count_w]);
    /* Load PWM setting */
    set_PWMCON0_LOAD;

    /* Add sine array pointer */
    g_u16Sine_Count_u++;
    g_u16Sine_Count_v++;
    g_u16Sine_Count_w++;

    /* Start Timer0 */
    set_TCON_TR0;
    /* Start PWM */
    set_PWMCON0_PWMRUN;

    while (1);
}