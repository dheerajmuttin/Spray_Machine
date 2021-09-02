/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2020 nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : June/21/2020
//***********************************************************************************************************
#include "MS51_32K.h"
bit BIT_TMP;
unsigned char data  TA_REG_TMP,BYTE_TMP,SFRS_TMP;

  /**
  * @brief This API configures modify system HIRC value
  * @param[in] u8FsysMode . Valid values are:
  *                       - \ref HIRC_24                 :Internal HIRC 24MHz .
  *                       - \ref HIRC_16                 :Internal HIRC 16MHz.
  *                       - \ref HIRC_166                :Internal HIRC 16.6MHz.
  * @note      None.
  * @exmaple : MODIFY_HIRC(HIRC_24);
  */
void MODIFY_HIRC(unsigned char u8HIRCSEL)
{
    unsigned char data hircmap0,hircmap1;
    unsigned int trimvalue16bit;
    /* Check if power on reset, modify HIRC */
    SFRS = 0 ;
    switch (u8HIRCSEL)
    {
      case HIRC_24:
        IAPAL = 0x38;
      break;
      case HIRC_16:
        IAPAL = 0x30;
      break;
      case HIRC_166:
        IAPAL = 0x30;
      break;
    }
    set_CHPCON_IAPEN;
    IAPAH = 0x00;
    IAPCN = READ_UID;
    set_IAPTRG_IAPGO;
    hircmap0 = IAPFD;
    IAPAL++;
    set_IAPTRG_IAPGO;
    hircmap1 = IAPFD;
    clr_CHPCON_IAPEN;
    switch (u8HIRCSEL)
    {
      case HIRC_166:
        trimvalue16bit = ((hircmap0 << 1) + (hircmap1 & 0x01));
        trimvalue16bit = trimvalue16bit - 15;
        hircmap1 = trimvalue16bit & 0x01;
        hircmap0 = trimvalue16bit >> 1;

      break;
      default: break;
    }
    TA = 0xAA;
    TA = 0x55;
    RCTRIM0 = hircmap0;
    TA = 0xAA;
    TA = 0x55;
    RCTRIM1 = hircmap1;
    clr_CHPCON_IAPEN;
    PCON &= CLR_BIT4;
}