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
#include "lcd.h"
#include "gpio.h"

/****** Macros ****************************************************************/

/****** Function prototypes ****************************************************/

/****** Data ******************************************************************/

/****** Implementation ********************************************************/

/*******************************************************************************
 *  function :    led_Power_On
 ******************************************************************************/
void lcd_power_on(void) 
{
    GpioInit_t tGpioInit = {GPIO_MODE_OUTPUT,
                            GPIO_OUTPUT_PUSH_PULL,
                            GPIO_SPEED_HIGH,
                            GPIO_PULL_NON
                           };
    /* Enable the peripheral clock of GPIOB */
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Select output mode on GPIOB pin 14 - LCD POWER CONTROL */  
    gpio_init(GPIOB, 14, &tGpioInit);
}


/*******************************************************************************
 *  function :    led_set
 ******************************************************************************/
void lcd_pin_set(Lcd_Com_t tLed) {

    gpio_set(GPIOB, tLed);
	
//	    GpioInit_t tGpioInit = {GPIO_MODE_INPUT,
//                            GPIO_OUTPUT_PUSH_PULL,
//                            GPIO_SPEED_MEDIUM,
//                            GPIO_PULL_NON
//                           };
//	    /* Select input mode on GPIOB pin tLed */  
//			gpio_init(GPIOB, tLed, &tGpioInit);		
			
}


/*******************************************************************************
 *  function :    led_clear
 ******************************************************************************/
void lcd_pin_clear(Lcd_Com_t tLed) {

    gpio_clear(GPIOB, tLed);
//		  GpioInit_t tGpioInit = {GPIO_MODE_OUTPUT,
//                            GPIO_OUTPUT_PUSH_PULL,
//                            GPIO_SPEED_MEDIUM,
//                            GPIO_PULL_NON
//                           };
//	    /* Select input mode on GPIOB pin tLed */  
//			gpio_init(GPIOB, tLed, &tGpioInit);	
}

/*******************************************************************************
 *  function :    lcd_pin_opendrain
 ******************************************************************************/
void lcd_pin_opendrain(Lcd_Com_t tLed) {

		  GpioInit_t tGpioInit = {GPIO_MODE_INPUT,
                            GPIO_OUTPUT_OPEN_DRAIN,  
                            GPIO_SPEED_MEDIUM,
                            GPIO_PULL_NON				
                           };
	    /* Select input mode on GPIOB pin tLed */  
			gpio_init(GPIOB, tLed, &tGpioInit);	
}

/*******************************************************************************
 *  function :    lcd_pin_opendrain
 ******************************************************************************/
void lcd_pin_output(Lcd_Com_t tLed) {

		  GpioInit_t tGpioInit = {GPIO_MODE_OUTPUT,
                            GPIO_OUTPUT_PUSH_PULL,
                            GPIO_SPEED_MEDIUM,
                            GPIO_PULL_NON
                           };
	    /* Select input mode on GPIOB pin tLed */  
			gpio_init(GPIOB, tLed, &tGpioInit);	
}


/*=====================================================================
***** WRITE THE DISPLAY DATA TO SHIFT REGISTER AND UPDATE COM 1   *****			
=====================================================================*/

void updateCOM(void)	
{
	//lcd_pin_clear(LCD_SRRES);
	int dspdatacnt = 0;
	while (++dspdatacnt < 49)
	{
		asm("nop");
		asm("nop");	
		asm("nop");
		asm("nop");
		lcd_pin_set(LCD_SRDATA);	
		asm("nop");
		asm("nop");			
		lcd_pin_set(LCD_SRCLK);			
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");			
		lcd_pin_clear(LCD_SRCLK);					
	}	
	lcd_pin_set(LCD_SRRES);
}

/*=====================================================================
***** WRITE THE DISPLAY DATA TO SHIFT REGISTER AND UPDATE COM 1   *****			
=====================================================================*/

void clearCOM(void)	
{
	//lcd_pin_clear(LCD_SRRES);
	int dspdatacnt = 0;
	while (++dspdatacnt < 49)
	{
		asm("nop");
		asm("nop");	
		asm("nop");
		asm("nop");
		lcd_pin_clear(LCD_SRDATA);	
		asm("nop");
		asm("nop");			
		lcd_pin_set(LCD_SRCLK);			
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");			
		lcd_pin_clear(LCD_SRCLK);					
	}	
	lcd_pin_set(LCD_SRRES);
}

/*=====================================================================
***** UPDATE LCD BUFFER   *****			
=====================================================================*/

void updateLCDBuffer(void)	
{
}


/*=====================================================================
***** UPDATE LCD BUFFER   *****			
=====================================================================*/

void updateLCD(void)	
{
	
		//I2C_Write_Reg(HTS221_ADDRESS,HTS221_AV_CONF,AV_CONF_Init);
	
}

