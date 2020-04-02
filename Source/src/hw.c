/******************************************************************************/
/** @file       hw.c
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
#include "i2c.h"
#include "rtc.h"
#include "adc.h"
#include "nxp_lcd_driver.h"

/****** Macros ****************************************************************/

/****** Data types ************************************************************/

/****** Function prototypes ****************************************************/
void adc_con_pins_init(void);


/****** Data ******************************************************************/

/****** Implementation ********************************************************/


/*******************************************************************************
 *  function :    hw_initSysclock
 ******************************************************************************/
/** @brief        This function configures the system clock @16MHz and voltage
 *                scale 1 assuming the registers have their reset value before
 *                the call.
 *                <p>
 *                POWER SCALE   = RANGE 1
 *                SYSTEM CLOCK  = PLL MUL8 DIV2
 *                PLL SOURCE    = HSI/4
 *                FLASH LATENCY = 0
 *
 *  @copyright    Licensed under MCD-ST Liberty SW License Agreement V2,
 *                (the "License");
 *                You may not use this file except in compliance with the
 *                License. You may obtain a copy of the License at:
 *                http://www.st.com/software_license_agreement_liberty_v2
 *
 *  @type         global
 *
 *  @param[in]    in
 *  @param[out]   out
 *
 *  @return       void
 *
 ******************************************************************************/
void hw_initSysclock(void) 
{
  /* Enable HSI */
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);
	
	/* Wait for HSI to be ready */
  while ((RCC->CR & RCC_CR_HSERDY) == 0){
		// Nop
	}

	/* Set HSI as the System Clock */
  RCC->CFGR = RCC_CFGR_SW_HSE;
	
	/* Wait for HSI to be used for teh system clock */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE){
		// Nop
	}
	
	FLASH->ACR |= FLASH_ACR_PRFTEN;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_LATENCY;                         // Flash 1 wait state

  RCC->APB1ENR |= RCC_APB1ENR_PWREN;                       // Enable the PWR APB1 Clock
  PWR->CR = PWR_CR_VOS_0;                                  // Select the Voltage Range 1 (1.8V)
  while((PWR->CSR & PWR_CSR_VOSF) != 0);                   // Wait for Voltage Regulator Ready

  /* PLLCLK = (HSE * 4)/2 = 32 MHz */
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL | RCC_CFGR_PLLDIV);				/* Clear */
  RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL4 | RCC_CFGR_PLLDIV2);	/* Set   */
	
	/* Peripheral Clock divisors */
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         // HCLK = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;                        // PCLK1 = HCLK
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;                        // PCLK2 = HCLK

	/* Enable PLL */
  RCC->CR &= ~RCC_CR_PLLON;		/* Disable PLL */
  RCC->CR |= RCC_CR_PLLON;		/* Enable PLL	 */
	
	/* Wait until the PLL is ready */
  while((RCC->CR & RCC_CR_PLLRDY) == 0){
		//Nop
	}
	
	/* Select PLL as system Clock */
  RCC->CFGR &= ~RCC_CFGR_SW;			/* Clear */
  RCC->CFGR |=  RCC_CFGR_SW_PLL;	/* Set   */
	
	/* Wait for PLL to become system core clock */
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
		//Nop
	}
}



/*******************************************************************************
 *  function :    hw_init
 ******************************************************************************/
void hw_init(void) {
	//
	//
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;	
	//	
	systick_delayMs(10);	
	I2C_Init();
	systick_delayMs(10);
	button_init();
	systick_delayMs(10);
	Configure_DBG();
	//	
}
