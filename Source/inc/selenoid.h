#ifndef SELENOID_H_
#define SELENOID_H_
/******************************************************************************/
/** \file       selenoid.h
 *******************************************************************************
 *
 *  \brief      

 *
 *  \author    
 *
 ******************************************************************************/
/*
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****** Header-Files **********************************************************/
#include "stm32l0xx.h"

/****** Macros ****************************************************************/

/****** Data types ************************************************************/
typedef enum _Selenoid_t {

    SELENOID_4_SET = 7,
    SELENOID_4_RESET = 3,
	  SELENOID_3_SET = 8,
    SELENOID_3_RESET = 4,
	  SELENOID_2_SET = 11,
    SELENOID_2_RESET = 5,
	  SELENOID_1_SET = 12,
    SELENOID_1_RESET = 6
}
Selenoid_t;

/****** Function prototypes ****************************************************/
extern void selenoid_init(void);

extern void selenoid_pin_set(Selenoid_t tSelenoid);

extern void selenoid_pin_reset(Selenoid_t tSelenoid);

extern void selenoid_enable(void);

extern void selenoid_disable(void);

extern void selenoid_sleep(void);

extern void selenoid_wakeup(void);

extern void closeAllSelenoids(void);

extern void openAllSelenoids(void);

extern void selenoidCon( void ) ;

extern uint8_t fOpenRelay1;
extern uint8_t fCloseRelay1;
extern uint8_t fOpenRelay2;
extern uint8_t fCloseRelay2;
extern uint8_t fOpenRelay3;
extern uint8_t fCloseRelay3;
extern uint8_t fOpenRelay4;
extern uint8_t fCloseRelay4;


/****** Data ******************************************************************/

/****** Implementation ********************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SELENOID_H_ */
