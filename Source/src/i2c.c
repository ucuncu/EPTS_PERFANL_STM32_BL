/*------------------------------------------------------------------------------------------------------
 * Name:    I2C.c
 * Purpose: Initializes and reads and writes to I2C
 * Date: 		6/18/15
 * Author:	Christopher Jordan - Denny
 *------------------------------------------------------------------------------------------------------
 * Note(s): The read and write sequence is specific to the ISK01A1, so these functions may not work
						for a different Devices I2C.
 *----------------------------------------------------------------------------------------------------*/

/*-------------------------------------------Include Statements---------------------------------------*/
#include "stm32l053xx.h"                  // Specific Device header
#include "I2C.h"
#include "interrupt.h"
#include "systick.h"
#include "nxp_lcd_driver.h"
/*-------------------------------------------Global Variables-----------------------------------------*/
uint32_t I2C1_RX_Data = 0;
/*-------------------------------------------Functions------------------------------------------------*/

void I2C_Init(void){
  /* Enable the peripheral clock of GPIOB */
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; /*Enable Clock for I2C*/
  RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;   /* (2) */

	int SCL = 6;							//SCL pin on PORTB alt fnc 4
	int SDA = 7;							//SDA pin on PORTB alt fnc 4
	
	
	I2C1->CR1 |= (1<<8);			

	//GPIOB->MODER|= GPIO_MODER_MODE6|GPIO_MODER_MODE7;
	GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7; /* (2) */
	GPIOB->MODER = ~((~GPIOB->MODER) | ((1 << 2*SCL) + (1 << 2*SDA)));	/*(2)*/
	GPIOB->AFR[0] = 0x11000000;																					/*(3)*/
	GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEED6|GPIO_OSPEEDER_OSPEED7;//predkosc 50MHZ	

	
	I2C1->CR1|=I2C_CR1_PE;                    //set PE
  I2C1->CR1&=~I2C_CR1_PE;                    //reset PE	

	while(I2C1->CR1&I2C_CR1_PE);		//while PE ==1

	I2C1->TIMINGR = (uint32_t)0x00503D5A;	  //*	Standard Mode @100kHz with I2CCLK = 16MHz, rise time = 100ns, fall time = 10ns.(1)
	//I2C1->TIMINGR = (uint32_t)0x00300619;   // 400kHZ
	I2C1->CR1 |= (I2C_CR1_PE);							//set PE
	while(!(I2C1->CR1&I2C_CR1_PE));            //while PE ==1

//	//Set CR2 for 2-byte transfer for Device
//	I2C1->CR2 |= (0x70<<0);  //  slave address
//	
//	I2C1->CR2 &=~ I2C_CR2_RD_WRN;                        //write
		
	/* Don't forget to also enable which interrupts you want in CR1 */
	NVIC_EnableIRQ(I2C1_IRQn);
	NVIC_SetPriority(I2C1_IRQn,0);	

}



/**
  \fn				void Reset_I2C(void)
  \brief		I2C Reset, clears and then sets I2C_CR1_PE
*/

void Reset_I2C(void){
	int x = 0;		//1 to set bit, 0 to clear bit
	
	I2C1->CR1 ^= (-x ^ I2C1->CR1) & I2C_CR1_PE;		//Clear bit for reset
	I2C1->CR1 |= I2C_CR1_PE;
}



/**
  \fn					uint32_t I2C_Read_Reg(uint32_t Register)
  \brief			Reads a register, the entire sequence to read
	\param			uint32_t Device: The slave address of the device
	\param			uint32_t Register: The Register to read from
	\returns		uint32_t I2C1_RX_Data: The data read
*/

uint32_t I2C_Read_Reg(uint32_t Device,uint32_t Register){
	
	//Reset CR2 Register
	I2C1->CR2 = 0x00000000;
	
	//Check to see if the bus is busy
	while((I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);
	
	
	//Set CR2 for 1-byte transfer for Device
	I2C1->CR2 |=(1UL<<16) | (Device);
	
		//Start communication
	I2C1->CR2 |= I2C_CR2_START;	

	//Check Tx empty before writing to it
	if((I2C1->ISR & I2C_ISR_TXE) == (I2C_ISR_TXE)){
		I2C1->TXDR = Register;
	}
	
	//Wait for transfer to complete
	while((I2C1->ISR & I2C_ISR_TC) == 0);

	//Clear CR2 for new configuration
	I2C1->CR2 = 0x00000000;
	
	//Set CR2 for 1-byte transfer, in read mode for Device
	I2C1->CR2 |= (1UL<<16) | I2C_CR2_RD_WRN | (Device);
	
	//Start communication
	I2C1->CR2 |= I2C_CR2_START;
	
	//Wait for transfer to complete
	while((I2C1->ISR & I2C_ISR_TC) == 0);
	
	//Send Stop Condition
	I2C1->CR2 |= I2C_CR2_STOP;
	
	//Check to see if the bus is busy
	while((I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);

	//Clear Stop bit flag
	I2C1->ICR |= I2C_ICR_STOPCF;
	
	I2C1_RX_Data = I2C1->RXDR;
	
	return(I2C1_RX_Data);
}



void I2C_Write_Bulk(uint32_t Device, uint32_t Register, uint32_t *Data, uint32_t Lenght)
{

	//Reset CR2 Register
	I2C1->CR2 = 0x00000000;
	
	//Check to see if the bus is busy
	while((I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);

	
		//Set CR2 for 1-byte transfer for Device
	I2C1->CR2 |= ((Lenght+1) << 16) | (Device);
	
		//Start communication
	I2C1->CR2 |= I2C_CR2_START;	


	
	//Check Tx empty before writing to it
	if((I2C1->ISR & I2C_ISR_TXE) == (I2C_ISR_TXE)){
		I2C1->TXDR = Register;
	}
	
	//Wait for TX Register to clear
	while((I2C1->ISR & I2C_ISR_TXE) == 0);

	
	for(int i=0; i<Lenght; i++)      // TIMEOUT KOY, ERROR DURUMDA ÇIKIS KOY, WATCHDOG EKLE VSVS
	{
		//Check Tx empty before writing to it
		if((I2C1->ISR & I2C_ISR_TXE) == (I2C_ISR_TXE))
		{
			I2C1->TXDR = Data[i];
		}
		
		//Wait for transfer to complete
		while((I2C1->ISR & I2C_ISR_TXE) == 0);
	}
	
	//Wait for transfer to complete
	while((I2C1->ISR & I2C_ISR_TC) == 0);
	
	//Send Stop Condition
	I2C1->CR2 |= I2C_CR2_STOP;	
	
	//Check to see if the bus is busy
	while((I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);
	
	//Clear Stop bit flag
	I2C1->ICR |= I2C_ICR_STOPCF;	
	
}
