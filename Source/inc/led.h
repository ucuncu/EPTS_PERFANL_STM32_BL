#ifndef LED_H_
#define LED_H_
/******************************************************************************/
/** \file       led.h
 *******************************************************************************
 *
 *  \brief      Module for handling the attached user led
 *              <p>
 *              There are two user led's attached to the stm32l053:
 *              <ul>
 *                  <li> LED_GREEN: I/O PB4
 *                  <li> LED_RED: I/O PA5
 *              </ul>
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    led_init
 *              led_set
 *              led_clear
 *              led_toogle
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****** Header-Files **********************************************************/
#include "stm32l0xx.h"

/****** Macros ****************************************************************/

/****** Function prototypes ****************************************************/
extern void led_init(void);

extern void bl_led_set(void); 
extern void bl_led_clear(void);
extern void bl_led_toogle(void); 
extern void rf_led_set(void);
extern void rf_led_clear(void); 
extern void rf_led_toogle(void); 
extern void rgb_red_led_set(void); 
extern void rgb_red_led_clear(void); 
extern void rgb_blue_led_set(void); 
extern void rgb_blue_led_clear(void); 
extern void rgb_green_led_set(void); 
extern void rgb_green_led_clear(void); 
extern void rgb_control_set(void); 
extern void rgb_control_clear(void); 


/****** Data ******************************************************************/

/****** Implementation ********************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LED_H_ */
