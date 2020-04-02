#ifndef GPIO_H_
#define GPIO_H_
/******************************************************************************/
/** \file       gpio.h
 *******************************************************************************
 *
 *  \brief      General gpio handling for stm32l053
 *
 *  \author     wht4
 *
 ******************************************************************************/
/*
 *  function    gpio_init
 *              gpio_set
 *              gpio_clear
 *              gpio_toogle
 *
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****** Header-Files **********************************************************/
#include <stdint.h>

#include "stm32l0xx.h"

/****** Macros ****************************************************************/

/****** Data types ************************************************************/
typedef enum _GpioMode_t {

    GPIO_MODE_INPUT     = 0x00,
    GPIO_MODE_OUTPUT    = 0x01,
    GPIO_MODE_ALTERNATE = 0x10,
    GPIO_MODE_ANALOG    = 0x11   ///< Reset state

}
GpioMode_t;


typedef enum _GpioOutput_t {

    GPIO_OUTPUT_PUSH_PULL  = 0x00,  ///< Reset state
    GPIO_OUTPUT_OPEN_DRAIN = 0x01

} GpioOutput_t;


typedef enum _GpioSpeed_t {

    GPIO_SPEED_VERY_LOW = 0x00,
    GPIO_SPEED_LOW      = 0x01,
    GPIO_SPEED_MEDIUM   = 0x10,
    GPIO_SPEED_HIGH     = 0x11

} GpioSpeed_t;


typedef enum _GpioPull_t {

    GPIO_PULL_NON       = 0x00,
    GPIO_PULL_UP        = 0x01,
    GPIO_PULL_DOWN      = 0x10

} GpioPull_t;

// TODO create fct for handling alternate function
typedef enum _GpioAF_t {

    GPIO_AF_0 = 0x0000,
    GPIO_AF_1 = 0x0001,
    GPIO_AF_2 = 0x0010,
    GPIO_AF_3 = 0x0011,
    GPIO_AF_4 = 0x0100,
    GPIO_AF_5 = 0x0101,
    GPIO_AF_6 = 0x0110,
    GPIO_AF_7 = 0x0111

} GpioAF_t;


typedef struct _GpioInit_t {

    GpioMode_t    tGpioMode;
    GpioOutput_t  tGpioOutput;
    GpioSpeed_t   tGpioSpeed;
    GpioPull_t    tGpioPull;

} GpioInit_t;


/****** Function prototypes ****************************************************/
extern void
gpio_init (GPIO_TypeDef * ptGpioBase, uint32_t u32Pin, GpioInit_t * ptGpioInit);

/****** Data ******************************************************************/

/****** Implementation ********************************************************/

/*******************************************************************************
 *  function :    gpio_set
 ******************************************************************************/
static inline void
gpio_set(GPIO_TypeDef * ptGpioBase, uint32_t u32Pin) {

    ptGpioBase->BSRR |= (0x01UL << u32Pin);
}


/*******************************************************************************
 *  function :    gpio_clear
 ******************************************************************************/
static inline void
gpio_clear(GPIO_TypeDef * ptGpioBase, uint32_t u32Pin) {

    ptGpioBase->BSRR |= (0x10000UL << u32Pin);
}


/*******************************************************************************
 *  function :    gpio_toogle
 ******************************************************************************/
static inline void
gpio_toogle(GPIO_TypeDef * ptGpioBase, uint32_t u32Pin) {

    ptGpioBase->ODR ^= (1UL << u32Pin);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* GPIO_H_ */
