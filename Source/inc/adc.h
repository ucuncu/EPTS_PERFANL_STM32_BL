#ifndef ADC_H_
#define ADC_H_
/******************************************************************************/
/** \file       adc.h
 *******************************************************************************
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****** Header-Files **********************************************************/
#include "stm32l0xx.h"

/****** Macros ****************************************************************/

extern void adc_bat_read_enable(void) ;
extern void adc_bat_read_disable(void) ;
extern void adc_sens_read_enable(void) ;
extern void adc_sens_read_disable(void) ;

extern void SetClockForADC(void);
extern void CalibrateADC(void);
extern void ConfigureADC(void);
extern void EnableADC(void);
extern void DisableADC(void);


/****** Data ******************************************************************/

/****** Implementation ********************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ADC_H_ */
