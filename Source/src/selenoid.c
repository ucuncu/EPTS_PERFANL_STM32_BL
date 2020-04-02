/******************************************************************************/
/** @file       lcd.c
 *******************************************************************************
 *
 *  @brief      
 *
 *  @author    
 *
 *  @remark     
 *
 ******************************************************************************/
/*
 *              .
 *
 ******************************************************************************/

/****** Header-Files **********************************************************/
#include "selenoid.h"
#include "gpio.h"
#include "systick.h"
#include "nxp_lcd_driver.h"

/****** Macros ****************************************************************/

uint8_t fOpenRelay1 = 0;
uint8_t fCloseRelay1 = 0;
uint8_t fOpenRelay2 = 0;
uint8_t fCloseRelay2 = 0;
uint8_t fOpenRelay3 = 0;
uint8_t fCloseRelay3 = 0;
uint8_t fOpenRelay4 = 0;
uint8_t fCloseRelay4 = 0;

/****** Function prototypes ****************************************************/

/****** Data ******************************************************************/

/****** Implementation ********************************************************/

/*******************************************************************************
 *  function :    led_init
 ******************************************************************************/
void selenoid_init(void) {

    GpioInit_t tGpioInit = {GPIO_MODE_OUTPUT,
                            GPIO_OUTPUT_PUSH_PULL,
                            GPIO_SPEED_MEDIUM,
                            GPIO_PULL_NON
                           };

    /* Enable the peripheral clock of GPIOA and GPIOB */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN;

    /* Select output mode on GPIOA pin 3 - SELENOID_4_RESET */  
    gpio_init(GPIOA, 3, &tGpioInit);
		/* Select output mode on GPIOA pin 4 - SELENOID_3_RESET */
    gpio_init(GPIOA, 4, &tGpioInit);
		/* Select output mode on GPIOA pin 5 - SELENOID_2_RESET */
    gpio_init(GPIOA, 5, &tGpioInit);													 													 
    /* Select output mode on GPIOA pin 6 - SELENOID_1_RESET */  
    gpio_init(GPIOA, 6, &tGpioInit);
		/* Select output mode on GPIOA pin 7 - SELENOID_4_SET */
    gpio_init(GPIOA, 7, &tGpioInit);
		/* Select output mode on GPIOA pin 8 - SELENOID_3_SET */
    gpio_init(GPIOA, 8, &tGpioInit);
		/* Select output mode on GPIOA pin 11 - SELENOID_2_SET */
    gpio_init(GPIOA, 11, &tGpioInit);
		/* Select output mode on GPIOA pin 12 - SELENOID_1_SET */
    gpio_init(GPIOA, 12, &tGpioInit);
		/* Select output mode on GPIOA pin 15 - SELENOID_ENABLE */
    gpio_init(GPIOA, 15, &tGpioInit);		
		
    /* Enable the peripheral clock of GPIOB */
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;	
		/* Select output mode on GPIOB pin 15 - SELENOID_SLEEP */
    gpio_init(GPIOB, 15, &tGpioInit);			
}


/*******************************************************************************
 *  function :    selenoid_pin_set
 ******************************************************************************/
void selenoid_pin_set(Selenoid_t tSelenoid) {

    gpio_set(GPIOA, 12);
}


/*******************************************************************************
 *  function :    selenoid_pin_reset
 ******************************************************************************/
void selenoid_pin_reset(Selenoid_t tSelenoid) {

    gpio_clear(GPIOA, 6);
}

/*******************************************************************************
 *  function :    selenoid_enable
 ******************************************************************************/
void selenoid_enable( void ) {

    gpio_set(GPIOA, 15);
}

/*******************************************************************************
 *  function :    selenoid_disable
 ******************************************************************************/
void selenoid_disable( void ) {

    gpio_clear(GPIOA, 15);
}

/*******************************************************************************
 *  function :    selenoid_sleep
 ******************************************************************************/
void selenoid_sleep( void ) {

    gpio_clear(GPIOB, 15);
}

/*******************************************************************************
 *  function :    selenoid_sleep
 ******************************************************************************/
void selenoid_wakeup( void ) {

    gpio_set(GPIOB, 15);
}

/*******************************************************************************
 *  function :    closeAllSelenoids   -- ONLY USE TESTING PURPOSE
 ******************************************************************************/
void closeAllSelenoids( void ) 
{
//
}

/*******************************************************************************
 *  function :    openAllSelenoids    -- ONLY USE TESTING PURPOSE
 ******************************************************************************/
void openAllSelenoids( void ) 
{
//
}

///*******************************************************************************
// *  function :    Open Relay 1
// ******************************************************************************/
//void openRelay1( void ) 
//{
//// Selenoid 1 ONN
//  gpio_clear(GPIOA, 6);
//	gpio_set(GPIOA, 12);
//	systick_delayMs(10);
//	gpio_clear(GPIOA, 6);
//	gpio_clear(GPIOA, 12);
//}

///*******************************************************************************
// *  function :    Close Relay 1
// ******************************************************************************/
//void closeRelay1( void ) 
//{
//// Selenoid 1 OFF	
//  gpio_set(GPIOA, 6);
//	gpio_clear(GPIOA, 12);
//	systick_delayMs(10);
//	gpio_clear(GPIOA, 6);
//	gpio_clear(GPIOA, 12);
//}

///*******************************************************************************
// *  function :    Open Relay 2
// ******************************************************************************/
//void openRelay2( void ) 
//{
//// Selenoid 2 ONN
//  gpio_clear(GPIOA, 5);
//	gpio_set(GPIOA, 11);
//	systick_delayMs(10);
//	gpio_clear(GPIOA, 5);
//	gpio_clear(GPIOA, 11);
//}

///*******************************************************************************
// *  function :    Close Relay 2
// ******************************************************************************/
//void closeRelay2( void ) 
//{
//// Selenoid 2 OFF	
//  gpio_set(GPIOA, 5);
//	gpio_clear(GPIOA, 11);
//	systick_delayMs(10);
//	gpio_clear(GPIOA, 5);
//	gpio_clear(GPIOA, 11);
//}

///*******************************************************************************
// *  function :    Open Relay 3
// ******************************************************************************/
//void openRelay3( void ) 
//{
//// Selenoid 3 ONN
//  gpio_clear(GPIOA, 4);
//	gpio_set(GPIOA, 8);
//	systick_delayMs(10);
//	gpio_clear(GPIOA, 4);
//	gpio_clear(GPIOA, 8);
//}

///*******************************************************************************
// *  function :    Close Relay 3
// ******************************************************************************/
//void closeRelay3( void ) 
//{
//// Selenoid 3 OFF	
//  gpio_set(GPIOA, 4);
//	gpio_clear(GPIOA, 8);
//	systick_delayMs(10);
//	gpio_clear(GPIOA, 4);
//	gpio_clear(GPIOA, 8);
//}
///*******************************************************************************
// *  function :    Open Relay 4
// ******************************************************************************/
//void openRelay4( void ) 
//{
//// Selenoid 4 ONN
//  gpio_clear(GPIOA, 3);
//	gpio_set(GPIOA, 7);
//	systick_delayMs(10);
//	gpio_clear(GPIOA, 3);
//	gpio_clear(GPIOA, 7);
//}

///*******************************************************************************
// *  function :    Close Relay 4
// ******************************************************************************/
//void closeRelay4( void ) 
//{
//// Selenoid 4 OFF	
//  gpio_set(GPIOA, 3);
//	gpio_clear(GPIOA, 7);
//	systick_delayMs(10);
//	gpio_clear(GPIOA, 3);
//	gpio_clear(GPIOA, 7);
//}


/*******************************************************************************
// *  function :    Open Relay 1
// ******************************************************************************/
void openRelay1( void ) 
{
// Selenoid 1 ONN
  gpio_set(GPIOA, 6);
	systick_delayMs(10);
	gpio_clear(GPIOA, 6);
}

/*******************************************************************************
 *  function :    Close Relay 1
 ******************************************************************************/
void closeRelay1( void ) 
{
// Selenoid 1 OFF	
	GPIOA->ODR |= 0x00001920; // GPIOA  5 8 11 12 set
	systick_delayMs(10);
	GPIOA->ODR &= 0xFFFFE69F; // GPIOA  5 6 9 11 12 clear 
}

/*******************************************************************************
 *  function :    Open Relay 2
 ******************************************************************************/
void openRelay2( void ) 
{
// Selenoid 1 ONN
  gpio_set(GPIOA, 11);
	systick_delayMs(10);
	gpio_clear(GPIOA, 11);
}

/*******************************************************************************
 *  function :    Close Relay 2
 ******************************************************************************/
void closeRelay2( void ) 
{
// Selenoid 2 OFF	
	GPIOA->ODR |= 0x00001160; // GPIOA  5 6 8 12 set
	systick_delayMs(10);
	GPIOA->ODR &= 0xFFFFE69F; // GPIOA  5 6 9 11 12 clear
}

/*******************************************************************************
 *  function :    Open Relay 3
 ******************************************************************************/
void openRelay3( void ) 
{
// Selenoid 3 ONN
  gpio_set(GPIOA, 5);
	systick_delayMs(10);
	gpio_clear(GPIOA, 5);
}

/*******************************************************************************
 *  function :    Close Relay 3
 ******************************************************************************/
void closeRelay3( void ) 
{
// Selenoid 3 OFF	
	GPIOA->ODR |= 0x00001940; // GPIOA  6 8 11 12 set
	systick_delayMs(10);
	GPIOA->ODR &= 0xFFFFE69F; // GPIOA  5 6 9 11 12 clear
}
/*******************************************************************************
 *  function :    Open Relay 4
 ******************************************************************************/
void openRelay4( void ) 
{
// Selenoid 4 ONN
  gpio_set(GPIOA, 8);
	systick_delayMs(10);
	gpio_clear(GPIOA, 8);
}

/*******************************************************************************
 *  function :    Close Relay 4
 ******************************************************************************/
void closeRelay4( void ) 
{
// Selenoid 4 OFF	
	GPIOA->ODR |= 0x00001860; // GPIOA  5 6 11 12 set
	systick_delayMs(10);
	GPIOA->ODR &= 0xFFFFE69F; // GPIOA  5 6 9 11 12 clear
}




/*******************************************************************************
 *  function :    selenoid Control
 ******************************************************************************/
void selenoidCon( void ) 
{

	if(fCloseRelay1)
	{
		fRelay1Status = 0;
		fAnyRelayON = 0;
		LCDBufferReset();      // Röle açildiginda/kapandiginde sprink isaretini ekrandan kaldirmak için		
		systick_delayMs(20);
		selenoid_wakeup();
		selenoid_enable();
		systick_delayMs(20);
		fCloseRelay1 = 0;
		closeRelay1();	
		systick_delayMs(20);		
		selenoid_sleep();
		selenoid_disable();		
	}	
	if(fCloseRelay2)
	{
		fRelay2Status = 0;
		fAnyRelayON = 0;
		LCDBufferReset();      // Röle açildiginda/kapandiginde sprink isaretini ekrandan kaldirmak için		
		systick_delayMs(20);
		selenoid_wakeup();
		selenoid_enable();
		systick_delayMs(20);
		fCloseRelay2 = 0;
		closeRelay2();	
		systick_delayMs(20);		
		selenoid_sleep();
		selenoid_disable();			
	}			
	if(fCloseRelay3)
	{
		fRelay3Status = 0;
		fAnyRelayON = 0;
		LCDBufferReset();      // Röle açildiginda/kapandiginde sprink isaretini ekrandan kaldirmak için		
		systick_delayMs(20);
		selenoid_wakeup();
		selenoid_enable();
		systick_delayMs(20);
		fCloseRelay3 = 0;
		closeRelay3();	
		systick_delayMs(20);		
		selenoid_sleep();
		selenoid_disable();				
	}
	if(fCloseRelay4)
	{
		fRelay4Status = 0;
		fAnyRelayON = 0;
		LCDBufferReset();      // Röle açildiginda/kapandiginde sprink isaretini ekrandan kaldirmak için
		systick_delayMs(20);
		selenoid_wakeup();
		selenoid_enable();
		systick_delayMs(20);
		fCloseRelay4 = 0;
		closeRelay4();	
		systick_delayMs(20);		
		selenoid_sleep();
		selenoid_disable();		
	}		
	if(fOpenRelay1)
	{
		fRelay1Status = 1;
		fAnyRelayON = 1;
		LCDBufferReset();      // Röle açildiginda/kapandiginde sprink isaretini ekrandan kaldirmak için		
		systick_delayMs(20);
		selenoid_wakeup();
		selenoid_enable();
		systick_delayMs(20);
		fOpenRelay1 = 0;
		openRelay1();
		systick_delayMs(20);		
		selenoid_sleep();
		selenoid_disable();			
	}	
	
	if(fOpenRelay2)
	{
		fRelay2Status = 1;
		fAnyRelayON = 1;
		LCDBufferReset();      // Röle açildiginda/kapandiginde sprink isaretini ekrandan kaldirmak için		
		systick_delayMs(20);
		selenoid_wakeup();
		selenoid_enable();
		systick_delayMs(20);
		fOpenRelay2 = 0;
		openRelay2();	
		systick_delayMs(20);		
		selenoid_sleep();
		selenoid_disable();			
	}

	if(fOpenRelay3)
	{
		fRelay3Status = 1;
		fAnyRelayON = 1;
		LCDBufferReset();      // Röle açildiginda/kapandiginde sprink isaretini ekrandan kaldirmak için		
		systick_delayMs(20);
		selenoid_wakeup();
		selenoid_enable();
		systick_delayMs(20);
		fOpenRelay3 = 0;
		openRelay3();	
		systick_delayMs(20);		
		selenoid_sleep();
		selenoid_disable();			
	}

	if(fOpenRelay4)
	{
		fRelay4Status = 1;
		fAnyRelayON = 1;
		LCDBufferReset();      // Röle açildiginda/kapandiginde sprink isaretini ekrandan kaldirmak için		
		systick_delayMs(20);
		selenoid_wakeup();
		selenoid_enable();
		systick_delayMs(20);
		fOpenRelay4 = 0;
		openRelay4();	
		systick_delayMs(20);		
		selenoid_sleep();
		selenoid_disable();				
	}		
}


