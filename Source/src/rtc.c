/******************************************************************************/
/** @file       rtc.c
 *******************************************************************************
 *
 *
 ******************************************************************************/
/*
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

uint8_t fUpdateRtcAlarm = 0;
/**
  * Brief   This function configures RTC.
  * Param   None
  * Retval  None
*/
void Configure_RTC(void)  // LSI mode, not used
{
  RCC->CSR |= RCC_CSR_LSION; /* (1) */
  while((RCC->CSR & RCC_CSR_LSIRDY)!=RCC_CSR_LSIRDY) /* (2) */
  { 
    /* add time out here for a robust application */
  }
  RCC->APB1ENR |= RCC_APB1ENR_PWREN; /* (3) */
  PWR->CR |= PWR_CR_DBP; /* (4) */
  RCC->CSR = (RCC->CSR & ~RCC_CSR_RTCSEL) | RCC_CSR_RTCEN | RCC_CSR_RTCSEL_1; /* (5) */
  RCC->APB1ENR &=~ RCC_APB1ENR_PWREN; /* (7) */

}

/**
  * Brief   This function configures RTC.
  * Param   uint32_t New time
  * Retval  None
  */
void Init_RTC(uint32_t Time, uint32_t Date)
{
	RCC->APB1ENR |= (RCC_APB1ENR_PWREN);
	PWR->CR = (PWR->CR & ~(PWR_CR_VOS)) | PWR_CR_VOS_0;
	while(PWR->CSR & PWR_CSR_VOSF);
	 
	/** DBP enable */
	PWR->CR |= PWR_CR_DBP;
	 
	/* Disable PWR clock */
	RCC->APB1ENR &= (uint32_t)(~RCC_APB1ENR_PWREN);
	 

	/** reset RTC */
	RCC->CSR |= RCC_CSR_RTCRST;
	RCC->CSR &= ~RCC_CSR_RTCRST;
	 
	/** enable LSE */
	RCC->CSR |= RCC_CSR_LSEON | RCC_CSR_LSECSSON;
	while((RCC->CSR & RCC_CSR_LSERDY)!=RCC_CSR_LSERDY)
	{
	/* add time out here for a robust application */
	} 
	/** Enable RTC, use LSE as clock */
	RCC->CSR = (RCC->CSR & ~RCC_CSR_RTCSEL) | RCC_CSR_RTCEN | RCC_CSR_RTCSEL_0;


//	
	/** init rtc */
  RTC->WPR = 0xCA; /* (1) */ 
  RTC->WPR = 0x53; /* (1) */
  RTC->ISR = RTC_ISR_INIT; /* (2) */
  while((RTC->ISR & RTC_ISR_INITF)!=RTC_ISR_INITF) /* (3) */
  { 
    /* add time out here for a robust application */
  }

	RTC->PRER = (8<<16) | 80; /// 50Hz, 20ms
	//RTC->PRER = (127<<16) | 255;  /// 1Hz, 1sec
  //RTC->TR = RTC_TR_PM | Time; /* (5) */
	RTC->TR = Time; /* (5) */
	RTC->DR = Date; /* (5) */
	RTC->CR = (RTC->CR & ~RTC_CR_FMT);
  RTC->ISR =~ RTC_ISR_INIT; /* (6) */
  while((RTC->ISR & RTC_ISR_RECALPF) == RTC_ISR_RECALPF) /* (7) */
  { 
    /* add time out here for a robust application */
  }
  //RTC->CALR = RTC_CAL_CALP | 482; /* (8) */	
  RTC->WPR = 0xFE; /* (9) */
  RTC->WPR = 0x64; /* (9) */
}


void configureAlarmA(uint32_t AlarmVal)
{
	/* Enable Alarm interrupt */
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
	RTC->CR &=~ RTC_CR_ALRAE;
	long unsigned int timeOut = 0;
	while((RTC->ISR & RTC_ISR_ALRAWF) != RTC_ISR_ALRAWF) 
	{
	/* add time out here for a robust application */
		++timeOut;
		if(timeOut>timeOutVal)
		{
			break;
		}
	}
	RTC->ALRMAR = AlarmVal;
	RTC->CR |= RTC_CR_ALRAIE | RTC_CR_ALRAE;
	RTC->WPR = 0xFE;
	RTC->WPR = 0x64;

	/**  */
	EXTI->IMR |= EXTI_IMR_IM17;
	EXTI->RTSR |= EXTI_RTSR_TR17;
	NVIC_SetPriority(RTC_IRQn, 0);
	NVIC_EnableIRQ(RTC_IRQn);
}

void configureAlarmB(uint32_t AlarmVal)
{
	/* Enable Alarm interrupt */
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
	RTC->CR &=~ RTC_CR_ALRBE;
	long unsigned int timeOut = 0;
	while((RTC->ISR & RTC_ISR_ALRBWF) != RTC_ISR_ALRBWF) 
	{
	/* add time out here for a robust application */
		++timeOut;
		if(timeOut>timeOutVal)
		{
			break;
		}
	}
	RTC->ALRMBR = AlarmVal;
	RTC->CR |= RTC_CR_ALRBIE | RTC_CR_ALRBE;
	RTC->WPR = 0xFE;
	RTC->WPR = 0x64;

	/**  */
	EXTI->IMR |= EXTI_IMR_IM17;
	EXTI->RTSR |= EXTI_RTSR_TR17;
	NVIC_SetPriority(RTC_IRQn, 0);
	NVIC_EnableIRQ(RTC_IRQn);
}
