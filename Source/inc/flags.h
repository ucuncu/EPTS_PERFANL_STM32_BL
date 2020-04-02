/*****************************************************************************************

 * @file	flags.h
 * @target LPC1768
 * @date	09.06.2018
 * @swrevision v0.0
 * @brief	includes flags

******************************************************************************************/
#ifndef __FLAGS_H__
#define __FLAGS_H__
#include "stm32l053xx.h"

#define RD_BIT              0x01

//#define DEBUG_SWD              0x01


extern uint32_t fGPSdataUpdate;
extern int fReadAcc;
extern int fReadGyro;
extern int fReadMagneto;;


#endif  /* __FLAGS_H__ */
