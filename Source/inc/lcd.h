#ifndef LCD_H_
#define LCD_H_
/******************************************************************************/
/** \file       lcd.h
 *******************************************************************************
 *
 *  \brief      

 *
 *  \author    
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

/****** Macros ****************************************************************/

/****** Data types ************************************************************/
typedef enum _Lcd_Com_t {

    LCD_COM_1 = 10,
    LCD_COM_2 = 11,
    LCD_COM_3 = 12,
		LCD_COM_4 = 13,
	  LCD_SRDATA = 5,
    LCD_SRCLK = 6,
    LCD_SRRES = 7

}
Lcd_Com_t;

/****** Function prototypes ****************************************************/
extern void lcd_power_on(void);

extern void lcd_pin_set(Lcd_Com_t tLcd_Com);

extern void lcd_pin_clear(Lcd_Com_t tLcd_Com);

extern void lcd_pin_opendrain(Lcd_Com_t tLcd_Com);

extern void lcd_pin_output(Lcd_Com_t tLcd_Com);

extern void updateCOM( void );

extern void clearCOM( void );


extern void updateLCDBuffer(void);	

extern void updateLCD(void);	




/****** Data ******************************************************************/

/****** Implementation ********************************************************/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LCD_H_ */
