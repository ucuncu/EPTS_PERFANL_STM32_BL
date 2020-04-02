#ifndef RTC_H_
#define RTC_H_
/******************************************************************************/
/** \file       rtc.h
 *******************************************************************************
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
#include "btn.h"


#define HSI_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define PLL_TIMEOUT_VALUE          ((uint32_t)100)  /* 100 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  ((uint32_t)5000) /* 5 s    */


#define SHORT_DELAY 200
#define LONG_DELAY 1000


#define WARNING_MEASURE 0x01

#define ERROR_HSI_TIMEOUT 0x02
#define ERROR_PLL_TIMEOUT 0x03
#define ERROR_CLKSWITCH_TIMEOUT 0x04

/* Internal voltage reference calibration value address */
#define VREFINT_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF80078))
#define VDD_CALIB ((uint16_t) (300))
#define VDD_APPLI ((uint16_t) (330))

/****** Macros ****************************************************************/
extern uint8_t fUpdateRtcAlarm;

/****** Function prototypes ****************************************************/
extern void Configure_RTC(void);
void Init_RTC(uint32_t Time, uint32_t Date);
extern void configureAlarmA(uint32_t AlarmVal);
extern void configureAlarmB(uint32_t AlarmVal);
/****** Data ******************************************************************/

/****** Implementation ********************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RTC_H_ */
