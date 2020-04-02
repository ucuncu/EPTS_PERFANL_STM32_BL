/******************************************************************************/
/** @file       interrupt.c
 *******************************************************************************
 *
 *  @brief      All application defined interrupt handlers
 *
 *  @author     wht4
 *
 *  @remark     Last modifications
 *                 \li V1.0, February 2016, wht4, initial release
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              SysTick_Handler
 *              EXTI0_1_IRQHandler
 *              NMI_Handler
 *              HardFault_Handler
 *              SVC_Handler
 *              PendSV_Handler
 *  functions  local:
 *              .
 *
 ******************************************************************************/

/****** Header-Files **********************************************************/
#include "stm32l0xx.h"
#include "flags.h"
#include "systick.h"
#include "btn.h"
#include "rtc.h"
#include "hw.h"
#include "selenoid.h"
#include "nxp_lcd_driver.h"

/****** Globals ****************************************************************/

int fReadAcc = 0;
int fReadGyro = 0;
int fReadMagneto = 0;

/*******************************************************************************
 *  function :    SysTick_Handler
 ******************************************************************************/
void
SysTick_Handler(void) {

    systick_irq();
}




/*******************************************************************************
 *  function :    RCC_CRS_IRQHandler
 ******************************************************************************/
void RCC_CRS_IRQHandler(void) 
{
	RCC->CIFR &= ~RCC_CIFR_HSIRDYF; // clear interrupt flag
	
	/* Select HSI as system clock */
	RCC->CFGR |= RCC_CFGR_SW_HSI | RCC_CFGR_SWS_HSI;

	/* Select MSI off */
	RCC->CR &= ~RCC_CR_MSION;
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);

	NVIC_SetPriority(RCC_CRS_IRQn, 0); /**/ 
	NVIC_DisableIRQ(RCC_CRS_IRQn); /*DISABLE BUTTON INTERRUPT*/ 

}

/*******************************************************************************
 *  function :    EXTI0_1_IRQHandler
 ******************************************************************************/
void RTC_IRQHandler(void) 
{
/*  Check line 17 has triggered the IT */
 if ((EXTI->PR & EXTI_PR_PR17) != 0)  
 {  	
		EXTI->PR |= EXTI_PR_PR17;
	}
}



/*******************************************************************************
 *  function :    EXTI0_1_IRQHandler
 ******************************************************************************/
void
EXTI0_1_IRQHandler(void) {

	/* Is there an interrupt on line 0 when device in sleep mode */
	if((EXTI->PR & EXTI_PR_PR0) != 0) 
	{
			EXTI->PR |= EXTI_PR_PR0;
	}
}

void
EXTI4_15_IRQHandler(void) {

	if((EXTI->PR & EXTI_PR_PR15) != 0) 
	{
			EXTI->PR |= EXTI_PR_PR15;
			fReadGyro = 1;

	}

	if((EXTI->PR & EXTI_PR_PR14) != 0) 
	{
			EXTI->PR |= EXTI_PR_PR14;
			fReadAcc = 1;

	}

	if((EXTI->PR & EXTI_PR_PR13) != 0) 
	{
			EXTI->PR |= EXTI_PR_PR13;
			fReadMagneto = 1;
	}
}


/*******************************************************************************
 *  function :    NMI_Handler
 ******************************************************************************/
void
NMI_Handler(void) {

    /* Default handler */
}


/*******************************************************************************
 *  function :    HardFault_Handler
 ******************************************************************************/
void
HardFault_Handler(void) {

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
			NVIC_SystemReset();
    }
}


/*******************************************************************************
 *  function :    SVC_Handler
 ******************************************************************************/
void
SVC_Handler(void) {

    /* Default handler */
}


/*******************************************************************************
 *  function :    PendSV_Handler
 ******************************************************************************/
void
PendSV_Handler(void) {

    /* Default handler */
}

/*******************************************************************************
 *  function :    I2C1_IRQHandler
 ******************************************************************************/
void
I2C1_IRQHandler(void) {

    /* Default handler */
	uint32_t I2C_InterruptStatus = I2C1->ISR; /* Get interrupt status */
  
}





