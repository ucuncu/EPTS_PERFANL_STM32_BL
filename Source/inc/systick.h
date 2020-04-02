#ifndef SYSTICK_H_
#define SYSTICK_H_
/******************************************************************************/
/** \file       systick.h
 *******************************************************************************
 *
 *  \brief      General systick module for stm32l0
 *              <p>
 *              Allows to generate delays in busy loop
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    systick_init
 *              systick_delayMs
 *              systick_getTick
 *              systick_irq
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****** Header-Files **********************************************************/
#include <stdint.h>

/****** Macros ****************************************************************/
#define SLEEP_COUNTER_VAL 30000   /// 30 sayi de uykuya girme
/****** Data types ************************************************************/

/****** Function prototypes ****************************************************/
extern uint32_t u32Tick;
	
	extern void
systick_init(uint32_t u32Ticks);

extern void
systick_delayMs(uint32_t u32DelayMs);

extern uint32_t
systick_getTick(void);

extern void
systick_irq(void);

extern int f1ms;
extern int f2ms;
extern int f20ms;
extern int f200ms;
extern int f1s;
extern int f2s;
extern int f10s;

extern int fsleepEnable;
extern int sleepCounter;
/****** Data ******************************************************************/

/****** Implementation ********************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SYSTICK_H_ */
