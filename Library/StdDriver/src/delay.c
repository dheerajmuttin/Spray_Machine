/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2020 nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//***********************************************************************************************************

#include "MS51_32K.h"

/**
  * @brief Timer0 delay setting
  * @param[in] u32SYSCLK define Fsys clock value. for example 24000000, use the real Fsys value.
  *                       - \ref 24000000 (use HIRC 24MHz)
  *                       - \ref 16000000 (use HIRC 16MHz)
  * @param[in] u8CNT define count time.
  * @param[in] u6DLYUnit define delay time base is us. From 1~10000, the maxima value please not over 10000.
  *                       - \ref 1000 (1ms)
  *                       - \ref 100 (100us)
  * @return  None
  * @note    If use LIRC or LXT as Fsys need adjust this marco.
  * @exmaple :  Timer0_Delay(16000000,200,1000);
*/
void Timer0_Delay(unsigned long u32SYSCLK, unsigned int u16CNT, unsigned int u16DLYUnit)
{
      unsigned char TL0TMP, TH0TMP;

      SFRS = 0;
      TIMER0_FSYS_DIV12;                                  //T0M=0, Timer0 Clock = Fsys/12
      ENABLE_TIMER0_MODE1;                                   //Timer0 is 16-bit mode
      TL0TMP = LOBYTE(65535-((u32SYSCLK/1000000)*u16DLYUnit/12));
      TH0TMP = HIBYTE(65535-((u32SYSCLK/1000000)*u16DLYUnit/12));
  
    while (u16CNT != 0)
    {
      TL0=TL0TMP;
      TH0=TH0TMP;
      set_TCON_TR0;                                    //Start Timer0
      while (!TF0);                       //Check Timer0 Time-Out Flag
      clr_TCON_TF0;
      clr_TCON_TR0;                       //Stop Timer0
      u16CNT --;
    }
    
      TL0  = 0; 
      TH0 = 0;
}