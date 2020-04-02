#ifndef INTERRUPT_H_
#define INTERRUPT_H_
/******************************************************************************/
/** \file       interrupt.h
 *******************************************************************************
 *
 *  \brief      All application defined interrupt handlers
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    SysTick_Handler
 *              EXTI0_1_IRQHandler
 *              NMI_Handler
 *              HardFault_Handler
 *              SVC_Handler
 *              PendSV_Handler
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****** Header-Files **********************************************************/

/****** Macros ****************************************************************/

/****** Data types ************************************************************/

/****** Function prototypes ****************************************************/
extern void
SysTick_Handler(void);

extern void
EXTI0_1_IRQHandler(void);

extern void
NMI_Handler(void);

extern void
HardFault_Handler(void);

extern void
SVC_Handler(void);

extern void
PendSV_Handler(void);

/****** Data ******************************************************************/

/****** Implementation ********************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INTERRUPT_H_ */
