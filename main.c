/******************************************************************************/
/** @file       main.c
 *******************************************************************************
 *
 *  @brief      SUPIRI  
 *
 ******************************************************************************/

/****** Header-Files **********************************************************/
/****** Header-Files **********************************************************/
#include "stm32l0xx.h"
#include "system_stm32l0xx.h"
#include "hw.h"
#include "gpio.h"
#include "led.h"
#include "button.h"
#include "systick.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "mma8452q.h"
#include "fxas21002.h"
#include "iis2mdc_reg.h"
#include "i2c.h"
#include "cam_m8q.h"
#include "flags.h"
#include "zigbee.h"
#include "rtc.h"


#define end	"\r"
#define line	"\n"

void ConfigureExtendedIT(void);
void ImuIntPinsInit(void);
void counterUpdate(void);
void checkRTCtimerTrig(void);
void ledUpdate(void);

uint32_t RGB200msCnt = 0;

uint32_t sensorReadCnt = 0;
uint32_t sensorReadErrorCnt = 0;


#define CONFIG_SYSTICK_1MS           ( 32000UL )

/*******************************************************************************
 *  function :    main
 ******************************************************************************/
int main(void) 
{
	/*************************************
	......
	....
	..
	.
	INIT SYSTEM
	.
	..
	....
	......
	**************************************/
	SystemInit();
	hw_initSysclock();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	systick_delayMs(10);
	led_init();
	systick_delayMs(10);
	gpio_set(GPIOB, 12);
	
	//Init_RTC(0x00000000, 0x0019D207 ); //Time, Date  ilk kurulum bu sekilde.
	systick_delayMs(100);
	hw_init();
	systick_delayMs(10);
	checkAccelerometer();
	start_acc();
	systick_delayMs(10); 	
	checkGyro();
	start_gyro();
	systick_delayMs(10);
	checkMagnetometer();
	start_magneto();
	systick_delayMs(10); 
	ImuIntPinsInit();
	ConfigureExtendedIT();
	USART1_Init();
	systick_delayMs(10); 
	initGPSubxData();
	Configure_GPIO_LPUART();
  Configure_LPUART();
	systick_delayMs(50); 
	while (1) 
	{
		if (fGPSdataUpdate)
		{
			fGPSdataUpdate = 0;
			GPSdataUpdate();
			conGpsDataToDecDegree();	
			IMUtoString();
			packAllDataforZigbee();			
		}
		if (fReadAcc)  //100Hz
		{
			read_acc();
			sensorReadCnt = sensorReadCnt + 5;
			if(sensorReadCnt == 23)
			{
				sensorReadCnt = 0;
			}		
		}		
		if (fReadMagneto)  //100Hz
		{
			read_magneto();	
			sensorReadCnt = sensorReadCnt + 7;				
			if(sensorReadCnt == 23)
			{
				sensorReadCnt = 0;
			}		
		}				
		if (fReadGyro)  //100Hz
		{
			read_gyro();
			sensorReadCnt = sensorReadCnt + 11;
			if(sensorReadCnt == 23)
			{
				sensorReadCnt = 0;
			}							
		}
		if(sensorReadCnt > 23)   // 100 Hz ile 3 sensörü interrupt ile okudugumuz için interrupt kaçirabiliyoruz. Kontrol yapip kitlenen sensörü tekrar aktif ediyoruz
		{
			sensorReadCnt = 0;
			sensorReadErrorCnt = sensorReadErrorCnt + 1;
			if (sensorReadErrorCnt >= 3)
			{
				sensorReadErrorCnt = 0;
				fReadAcc = 1;
				fReadMagneto = 1;
				fReadGyro = 1;
			}
		}			
		
		if(f20ms)
		{
			f20ms = 0;
			butread();		
		}
		if(f200ms)
		{
			f200ms = 0;
			ledUpdate(); 	
		}
		if(f2s)
		{	
			f2s = 0;
			LPUART1_Send_DataPack(dataPack);
			LPUART1_Send(end);
			LPUART1_Send(line);
		}		
	}
}



/**********************************************************************
 * @brief		Update Leds
 **********************************************************************/
void ledUpdate(void)
{
	if ((GPS_FIX_DATA_BUF[0][0] != '1') && (GPS_FIX_DATA_BUF[0][0] != '2'))  // GPS not fixed, looking for...
	{
		if (RGB200msCnt == 1)		
		{
			rgb_control_set();
			rgb_blue_led_set();
			rgb_red_led_set();
			rgb_green_led_set();
			rgb_blue_led_clear();
		}
		else if (RGB200msCnt == 2)		
		{
			rgb_control_set();
			rgb_blue_led_set();
			rgb_red_led_set();
			rgb_green_led_set();
			rgb_red_led_clear();	
		}
		else if (RGB200msCnt == 3)
		{
			rgb_control_set();
			rgb_blue_led_set();
			rgb_red_led_set();
			rgb_green_led_set();
			rgb_green_led_clear();			
		}			
		else
		{
			rgb_control_clear();
			rgb_blue_led_clear();
			rgb_red_led_clear();
			rgb_green_led_clear();
			RGB200msCnt = 0;
		}
		RGB200msCnt++;
	}
	else
	{
		rgb_control_set();
		rgb_blue_led_set();
		rgb_red_led_set();
		rgb_green_led_set();
		rgb_blue_led_clear();
	}
}

void ImuIntPinsInit(void) {

    GpioInit_t tGpioInit = {GPIO_MODE_INPUT,
                            GPIO_OUTPUT_PUSH_PULL,
                            GPIO_SPEED_MEDIUM,
                            GPIO_PULL_NON
                           };

    /* Enable the peripheral clock of GPIOB */
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Select INPUT mode (00) on GPIOB pin 15 */
    gpio_init(GPIOB, 15, &tGpioInit);
		/* Select INPUT mode (00) on GPIOB pin 14 */
    gpio_init(GPIOB, 14, &tGpioInit);
    /* Select INPUT mode (00) on GPIOB pin 13 */
    gpio_init(GPIOB, 13, &tGpioInit);													 
}

void ConfigureExtendedIT(void)
{  
  /* (1) Enable the peripheral clock of GPIOB */ 
  /* (2) Select input mode (00) on GPIOB pin 15,14,13 */  
  /* (3) Select Port A for pin 0 extended interrupt by writing 0000 
         in EXTI0 (reset value)*/
  /* (4) Configure the corresponding mask bit in the EXTI_IMR register */
  /* (5) Configure the Trigger Selection bits of the Interrupt line 
         on rising edge*/
  /* (6) Configure the Trigger Selection bits of the Interrupt line 
         on falling edge*/
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN; /* (1) */
  GPIOB->MODER = (GPIOB->MODER & ~((GPIO_MODER_MODE15)|(GPIO_MODER_MODE14)|(GPIO_MODER_MODE13))); /* (2) */  
  //SYSCFG->EXTICR[0] &= (uint16_t)~SYSCFG_EXTICR1_EXTI0_PA; /* (3) */
  EXTI->IMR |= 0xE000; /* (4) */ 
  //EXTI->RTSR |= 0xE000; /* (5) */
  EXTI->FTSR |= 0xE000; /* (6) */
  
  /* Configure NVIC for Extended Interrupt */
  /* (6) Enable Interrupt on EXTI0_1 */
  /* (7) Set priority for EXTI0_1 */
  NVIC_EnableIRQ(EXTI4_15_IRQn); /* (6) */
  NVIC_SetPriority(EXTI4_15_IRQn,0); /* (7) */
}
