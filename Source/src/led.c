/******************************************************************************/
/** @file       led.c
 *******************************************************************************
 *
 *  @brief      Module for handling the attached user led
 *              <p>
 *              There are two user led's attached to the stm32l053:
 *              <ul>
 *                  <li> LED_GREEN: I/O PB4
 *                  <li> LED_RED: I/O PA5
 *              </ul>
 *
 *  @author     wht4
 *
 *  @remark     Last modifications
 *                 \li V1.0, February 2016, wht4, initial release
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              led_init
 *              led_set
 *              led_clear
 *              led_toogle
 *  functions  local:
 *              .
 *
 ******************************************************************************/

/****** Header-Files **********************************************************/
#include "led.h"
#include "gpio.h"

/****** Macros ****************************************************************/

/****** Function prototypes ****************************************************/

/****** Data ******************************************************************/

/****** Implementation ********************************************************/

/*******************************************************************************
 *  function :    led_init
 ******************************************************************************/
void led_init(void) {

    GpioInit_t tGpioInit = {GPIO_MODE_OUTPUT,
                            GPIO_OUTPUT_PUSH_PULL,
                            GPIO_SPEED_MEDIUM,
                            GPIO_PULL_NON
                           };

    /* Enable the peripheral clock of GPIOA */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    /* Enable the peripheral clock of GPIOB */
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	
    /* Select output mode (01) on GPIOA pin 15 */
    gpio_init(GPIOA, 15, &tGpioInit);													 
													 
    /* Select output mode (01) on GPIOB pin 0 */
    gpio_init(GPIOB, 0, &tGpioInit);
    /* Select output mode (01) on GPIOB pin 1 */
    gpio_init(GPIOB, 1, &tGpioInit);
		/* Select output mode (01) on GPIOB pin 2 */
    gpio_init(GPIOB, 2, &tGpioInit);
    /* Select output mode (01) on GPIOB pin 3 */
    gpio_init(GPIOB, 3, &tGpioInit);
		/* Select output mode (01) on GPIOB pin 4 */
    gpio_init(GPIOB, 4, &tGpioInit);
													 
		/* Select output mode (01) on GPIOB pin 4 */
    gpio_init(GPIOB, 12, &tGpioInit);

}


/*******************************************************************************
 *  function :    led_set
 ******************************************************************************/
void bl_led_set(void) 
{
   gpio_set(GPIOB, 2);
}


/*******************************************************************************
 *  function :    led_clear
 ******************************************************************************/
void bl_led_clear(void) 
{
	gpio_clear(GPIOB, 2);
}


/*******************************************************************************
 *  function :    led_toogle
 ******************************************************************************/
void bl_led_toogle(void) 
{
   gpio_toogle(GPIOB, 2);
}


/*******************************************************************************
 *  function :    led_set
 ******************************************************************************/
void rf_led_set(void) 
{
   gpio_set(GPIOB, 0);
}


/*******************************************************************************
 *  function :    led_clear
 ******************************************************************************/
void rf_led_clear(void) 
{
	gpio_clear(GPIOB, 0);
}
/*******************************************************************************
 *  function :    led_toogle
 ******************************************************************************/
void rf_led_toogle(void) 
{
   gpio_toogle(GPIOB, 0);
}

/*******************************************************************************
 *  function :    led_set
 ******************************************************************************/
void rgb_red_led_set(void) 
{
   gpio_set(GPIOA, 15);
}


/*******************************************************************************
 *  function :    led_clear
 ******************************************************************************/
void rgb_red_led_clear(void) 
{
	gpio_clear(GPIOA, 15);
}

/*******************************************************************************
 *  function :    led_set
 ******************************************************************************/
void rgb_blue_led_set(void) 
{
   gpio_set(GPIOB, 1);
}


/*******************************************************************************
 *  function :    led_clear
 ******************************************************************************/
void rgb_blue_led_clear(void) 
{
	gpio_clear(GPIOB, 1);
}
/*******************************************************************************
 *  function :    led_set
 ******************************************************************************/
void rgb_green_led_set(void) 
{
   gpio_set(GPIOB, 3);
}


/*******************************************************************************
 *  function :    led_clear
 ******************************************************************************/
void rgb_green_led_clear(void) 
{
	gpio_clear(GPIOB, 3);
}

/*******************************************************************************
 *  function :    led_set
 ******************************************************************************/
void rgb_control_set(void) 
{
   gpio_set(GPIOB, 4);
}


/*******************************************************************************
 *  function :    led_clear
 ******************************************************************************/
void rgb_control_clear(void) 
{
	gpio_clear(GPIOB, 4);
}

