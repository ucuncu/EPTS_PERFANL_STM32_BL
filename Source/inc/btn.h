#ifndef BTN_H_
#define BTN_H_
/******************************************************************************/
/** \file       btn.h
 *******************************************************************************
 *
 *  \brief      Module for handling user push buttons.
 *              <p>
 *              Each push button is initialized to generate a interrupt on
 *              rising edge. There is currently one user push button attached
 *              to the stm32l053:
 *              <ul>
 *                  <li> BTN_1: I/O PA0
 *              </ul>
 *              <p>
 *              The user can register a callback fct for handling the button
 *              either in isr context or background context.
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    btn_init
 *              btn_registerCallback
 *              btn_isPressed
 *              btn_isr
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****** Header-Files **********************************************************/

/****** Macros ****************************************************************/

/****** Data types ************************************************************/
typedef enum _Btn_t {

    BTN_1        = 0,
    BTN_QUANTITY = 1

}
Btn_t;


typedef enum _BtnHandlingCtx_t {

    BTN_HANDLING_CTX_ISR        = 0,
    BTN_HANDLING_CTX_BACKGROUND = 1,
    BTN_HANDLING_CTX_INVALID    = 2

} BtnHandlingCtx_t;

typedef void (*pfBtnHandlingFct) (Btn_t tBtn, BtnHandlingCtx_t tBtnHandlingCtx);

/****** Function prototypes ****************************************************/
extern void
btn_init(void);

extern void
btn_registerCallback(Btn_t tBtn,
                     BtnHandlingCtx_t tBtnHandlingCtx,
                     pfBtnHandlingFct pfCallback);


extern void
btn_isPressed(Btn_t tBtn);

extern void
btn_isr(Btn_t tBtn);


/****** Data ******************************************************************/

/****** Implementation ********************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* BTN_H_ */
