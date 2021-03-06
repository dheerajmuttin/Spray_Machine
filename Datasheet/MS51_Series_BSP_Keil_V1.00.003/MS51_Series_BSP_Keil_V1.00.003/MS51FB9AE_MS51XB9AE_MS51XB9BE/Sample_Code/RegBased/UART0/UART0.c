/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/* Website: http://www.nuvoton.com                                                                         */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : Jan/21/2019                                                                                   */
/***********************************************************************************************************/

/************************************************************************************************************/
/*  File Function: MS51 UART0 initial demo code                                                             */
/************************************************************************************************************/
#include "MS51_16K.H"

/******************************************************************************
 * Use VCOM function check with COM port.
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION_PURPOSE: Main function 
 ******************************************************************************/
void main (void)
{
/* Modify HIRC to 24MHz for UART baud rate function only */
    MODIFY_HIRC(HIRC_24);
    P06_QUASI_MODE;
    UART_Open(24000000,UART0_Timer3,115200);
    ENABLE_UART0_PRINTF; /* check uart.c */
    while(1)
    {
        UART_Send_Data(UART0,0x55);      /* check uart.c*/
        Timer0_Delay(16000000,200,1000);            /* check delay.c*/
    }
    
}
  
