/******************************************************************************/
/** @file       adc.c
 *******************************************************************************
 *
 *  @brief      Module for initializing the attached hw
 *
 *  @author     wht4
 *
 *  @remark     Last modifications
 *                 \li V1.0, February 2016, wht4, initial release
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              hw_init
 *  functions  local:
 *              hw_initSysclock
 *
 ******************************************************************************/

/****** Header-Files **********************************************************/
#include "stm32l0xx.h"

#include "config.h"
#include "led.h"
#include "lcd.h"
#include "button.h"
#include "selenoid.h"
#include "systick.h"
#include "btn.h"
#include "gpio.h"
#include "adc.h"
#include "nxp_lcd_driver.h"

/*******************************************************************************
 *  function :    adc_bat_read_enable
 ******************************************************************************/
void adc_bat_read_enable(void) 
{
	gpio_set(GPIOA, 1);
}

/*******************************************************************************
 *  function :    adc_bat_read_disable
 ******************************************************************************/
void adc_bat_read_disable(void) 
{
	gpio_clear(GPIOA, 1);
}

/*******************************************************************************
 *  function :    adc_bat_read_enable
 ******************************************************************************/
void adc_sens_read_enable(void) 
{
	gpio_set(GPIOA, 2);
}

/*******************************************************************************
 *  function :    adc_bat_read_disable
 ******************************************************************************/
void adc_sens_read_disable(void) 
{
	gpio_clear(GPIOA, 2);
}

/**
  * Brief   This function enables the clock in the RCC for the ADC
  *        and for the System configuration (mandatory to enable VREFINT)
  * Param   None
  * Retval  None
  */
__INLINE void SetClockForADC(void)
{
  /* (1) Enable the peripheral clock of the ADC */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; /* (1) */
}

/**
  * Brief   This function performs a self-calibration of the ADC
  * Param   None
  * Retval  None
  */
__INLINE void  CalibrateADC(void)
{
  /* (1) Ensure that ADEN = 0 */
  /* (2) Clear ADEN */ 
  /* (3) Set ADCAL=1 */
  /* (4) Wait until EOCAL=1 */
  /* (5) Clear EOCAL */
  if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
  {
    ADC1->CR &= (uint32_t)(~ADC_CR_ADEN);  /* (2) */  
  }
  ADC1->CR |= ADC_CR_ADCAL; /* (3) */
	long unsigned int timeOut = 0;
  while ((ADC1->ISR & ADC_ISR_EOCAL) == 0) /* (4) */
  {
    /* For robust implementation, add here time-out management */
		++timeOut;
		if(timeOut>timeOutVal)
		{
			break;
		}
  }  
  ADC1->ISR |= ADC_ISR_EOCAL; /* (5) */
}


/**
  * Brief   This function configure the ADC to convert the internal reference voltage (VRefInt)
  *         The conversion frequency is 16 MHz 
  * Param   None
  * Retval  None
  */
__INLINE void ConfigureADC(void)
{
  /* (1) Select HSI16 by writing 00 in CKMODE (reset value) */ 
  /* (2) Select the auto off mode */
  /* (3) Select CHSEL17 for VRefInt */
  /* (4) Select a sampling mode of 111 i.e. 239.5 ADC clk to be greater than 17.1us */
  /* (5) Wake-up the VREFINT (only for VLCD, Temp sensor and VRefInt) , low freq */
  ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE; /* (1) */  
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_0; /* (1) */	
  ADC1->CFGR1 |= ADC_CFGR1_AUTOFF; /* (2) */
  ADC1->CHSELR = ADC_CHSELR_CHSEL0; /* (3) */
  ADC1->SMPR |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2; /* (4) */
  ADC->CCR |= ADC_CCR_VREFEN | ADC_CCR_LFMEN; /* (5) */
	
}


/**
  * Brief   This function enables the ADC
  * Param   None
  * Retval  None
  */
__INLINE void EnableADC(void)
{
  /* (1) Enable the ADC */
  /* (2) Wait until ADC ready if AUTOFF is not set */
  ADC1->CR |= ADC_CR_ADEN; /* (1) */
  if ((ADC1->CFGR1 &  ADC_CFGR1_AUTOFF) == 0)
  {
		
		long unsigned int timeOut = 0;	
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0)  /* (2) */
    {
      /* For robust implementation, add here time-out management */
			++timeOut;
			if(timeOut>timeOutVal)
			{
				break;
		}
    }
  }
}

/**
  * Brief   This function disables the ADC
  * Param   None
  * Retval  None
  */
__INLINE void DisableADC(void)
{
  /* (1) Ensure that no conversion on going */
  /* (2) Stop any ongoing conversion */
  /* (3) Wait until ADSTP is reset by hardware i.e. conversion is stopped */
  /* (4) Disable the ADC */
  /* (5) Wait until the ADC is fully disabled */
  if ((ADC1->CR & ADC_CR_ADSTART) != 0) /* (1) */
  {
    ADC1->CR |= ADC_CR_ADSTP; /* (2) */
  }
	long unsigned int timeOut = 0;	
  while ((ADC1->CR & ADC_CR_ADSTP) != 0)  /* (3) */
  {
     /* For robust implementation, add here time-out management */
		++timeOut;
		if(timeOut>timeOutVal)
		{
			break;
		}
  }
  ADC1->CR |= ADC_CR_ADDIS; /* (4) */
	long timeOut2 = 0;	
  while ((ADC1->CR & ADC_CR_ADEN) != 0) /* (5) */
  {
    /* For robust implementation, add here time-out management */
		++timeOut2;
		if(timeOut2>timeOutVal)
		{
			break;
		}
  }  
}

