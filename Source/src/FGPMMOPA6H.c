/*------------------------------------------------------------------------------------------------------
 * Name:    cam_m8q.c
 * Purpose: Initializes USART1 on PA9 and PA10 and communicates with the CAM-M8Q GPS Module via UART
 * Date: 		16/03/2020
 * Author:	Hamit UCUNCU
 ----------------------------------------------------------------------------------------------------
 * Note(s): This is created to be used with the adafruit GPS module.
 * A jumper between rx to pin 2 when on soft serial mode must 
 * be present.
 *----------------------------------------------------------------------------------------------------*/

/*---------------------------------Include Statements-------------------------------------------------*/
#include "stm32l053xx.h"			//Specific Device Header
#include <stdint.h>
#include <stdio.h>						//Standard input and output
#include <string.h>						//Various useful string functions
#include <stdlib.h>						//Various useful conversion functions
#include "FGPMMOPA6H.h"
#include "flags.h"
#include "systick.h"


/*---------------------------------Define Statments---------------------------------------------------*/
#define TRUE				0x1				//Truth value is 1
#define FALSE				0x0				//False value is 0

#define PCLK	32000000				// Peripheral Clock for STM32L053 on EPTS_PERF_ANL_V0_XX board
#define BAUD	9600						// Baud rate for UART1 ( GPS module and uC comm. )



/*---------------------------------Globals------------------------------------------------------------*/
int i,j,k = 0;

uint32_t fGPSdataUpdate = 0;   // Flag to check and update GPS datas

char GPS_DATA_BUF[GPS_BUFFER_HEIGHT][GPS_BUFFER_LENGHT];
char GPS_DATA_BUF_TMP[GPS_BUFFER_HEIGHT][GPS_BUFFER_LENGHT];
char GPS_TIME_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_TIME_BUFFER_LENGHT] = {'0','0','0','0','0','0','.','0','0','\0'};
char GPS_LAT_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_LAT_BUFFER_LENGHT] = {'0','0','0','0','.','0','0','0','0','0','\0'};
char GPS_LON_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_LON_BUFFER_LENGHT] = {'0','0','0','0','0','.','0','0','0','0','0','\0'};
char GPS_FIX_DATA_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_FIX_BUFFER_LENGHT] = {'0','\0'};
char GPS_VELOCITY_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_VELOCITY_BUFFER_LENGHT] = {'0','.','0','0','0','\0'};
char GPS_DATE_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_DATE_BUFFER_LENGHT] = {'0','1','0','1','1','9','\0'};      // GPS cold start yaparken zamanin 0 yada null olmasi web uygulamada JSON dönüsümünde hataya yol açiyor.Init deger verdik
char GPS_ALTITUDE_DATA_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_ALTITUDE_BUFFER_LENGHT] = {'0','.','0','0','0','\0'};



/*********************************************************************//**
 * @brief	INIT GPS SuBx Data
 ************************************************************************/
void initGPSubxData(void)
{
	//fInitGPS = TRUE;
	
	//GGA Off
	//	uint8_t GPSConfig[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x23};	
	//	for ( i = 0; i<16; i++)
	//	{
	//		UART_SendByte( (LPC_UART_TypeDef *)LPC_UART3, GPSConfig[i]);
	//	}
	//	Delay(100);
	
	//GxGLL off
	char GPSConfig1[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2A};	
	USART1_Send(GPSConfig1);
	
	// GxGSA off
	systick_delayMs(100);
	char GPSConfig2[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x31};	
	for ( i = 0; i<16; i++)
	{
		USART1_Send(GPSConfig2);
	}
	systick_delayMs(100);
	
	// GxGSV of
	char GPSConfig3[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x38};	
	USART1_Send(GPSConfig3);
	
	systick_delayMs(100);
	
	// GxRMC off
	//	uint8_t GPSConfig4[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x3F};	
	//	for ( i = 0; i<16; i++)
	//	{
	//		UART_SendByte( (LPC_UART_TypeDef *)LPC_UART3, GPSConfig4[i]);
	//	}
	//	Delay(100);
	
	
	// GxVTG off
	char GPSConfig5[16] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x46};	
	USART1_Send(GPSConfig5);
	systick_delayMs(100);
	
	// RATE 10 Hz
	char GPSConfig6[14] = {0xB5,0x62,0x06,0x08,0x06,0x00,0x64,0x00,0x01,0x00,0x01,0x00,0x7A,0x12};	
	USART1_Send(GPSConfig6);
	systick_delayMs(100);	

	char ubloxSbasInit[16] = { 0xB5, 0x62, 0x06, 0x16, 0x08, 0x00, 0x03, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0xE5 };
	USART1_Send(ubloxSbasInit);	
}



/*********************************************************************//**
 * @brief	Record Data To GPS Buffer
 ************************************************************************/
int RecDataToGPSbuffer( uint8_t GpsDatChar)
{
	uint8_t tmp = GpsDatChar;

	if(tmp == '$') //New NMEA Data is comming from GPS
	{
		j=0;
		GPS_DATA_BUF[i][j]=tmp;
		j++;
	}
	else if(tmp == '*') // NMEA Data ended, next data will be checksum (not used, if needed, add validation later ) !!! USE MALLOC LATER !!!
	{
		for (int z=j; z<GPS_BUFFER_LENGHT ; z++)
		{
			GPS_DATA_BUF[i][z]='*'; // No empty Area left
		}
		j=0;
		i++;
		if ( i >= GPS_BUFFER_HEIGHT )
		{
			i=0;
			//Now all data is ready,
			memcpy(&GPS_DATA_BUF_TMP[0][0], &GPS_DATA_BUF[0][0],(GPS_BUFFER_HEIGHT*GPS_BUFFER_LENGHT));
			// Add termination line
			for ( int h=0; h<GPS_BUFFER_HEIGHT; h++)
			{
				GPS_DATA_BUF_TMP[h][GPS_BUFFER_LENGHT-1]='\0';
			}
			fGPSdataUpdate = 1;    // Set GPS Data Update Flag
		}
	}
	else 
	{
		GPS_DATA_BUF[i][j]=tmp;
		j++;
		if ( j >= (GPS_BUFFER_LENGHT+1) )
		{
			j=0;
			return -1;
		}
	}
return 0;
}

/*********************************************************************//**
 * @brief	NMEA_GxGGA_RECEIVED
 ************************************************************************/

/*****************************************************************************
	GGA - essential fix data which provide 3D location and accuracy data.

 $GxGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47

Where:
     GGA          Global Positioning System Fix Data
     123519       Fix taken at 12:35:19 UTC
     4807.038,N   Latitude 48 deg 07.038' N
     01131.000,E  Longitude 11 deg 31.000' E
     1            Fix quality: 0 = invalid
                               1 = GPS fix (SPS)
                               2 = DGPS fix
                               3 = PPS fix
															 4 = Real Time Kinematic
															 5 = Float RTK
                               6 = estimated (dead reckoning) (2.3 feature)
															 7 = Manual input mode
															 8 = Simulation mode
     08           Number of satellites being tracked
     0.9          Horizontal dilution of position
     545.4,M      Altitude, Meters, above mean sea level
     46.9,M       Height of geoid (mean sea level) above WGS84
                      ellipsoid
     (empty field) time in seconds since last DGPS update
     (empty field) DGPS station ID number
     *47          the checksum data, always begins with *
		 
		 NOTE: NMEA Data cannot have more then 80 characters ( without line terminators )
****************************************************************************************/
void NMEA_GxGGA_RECEIVED(int bufferline)
{
	int bufline = bufferline;
	int i,j = 0;
	uint8_t comma[14];
	//uint8_t time[10]={};
	//printf("GPGGA data has beed updated --> bufferline: %d\n",bufline);
	
	for (i=0; i<GPS_BUFFER_LENGHT; i++)
	{
	 if(GPS_DATA_BUF_TMP[bufline][i] == ',')
	 {
		comma[j]= i;
		j++;
	 }
	 if (j > 14)
	 {
		 break;
	 }
	}
	memcpy(&GPS_TIME_BUF[0][0],&GPS_DATA_BUF_TMP[bufline][comma[0]+1],(comma[1]-comma[0]-1));
	GPS_TIME_BUF[0][8]='\0';
	memcpy(&GPS_LAT_BUF[0][0],&GPS_DATA_BUF_TMP[bufline][comma[1]+1],(comma[2]-comma[1]-1));
	GPS_LAT_BUF[0][9]='\0';
	memcpy(&GPS_LON_BUF[0][0],&GPS_DATA_BUF_TMP[bufline][comma[3]+1],(comma[4]-comma[3]-1));
	GPS_LON_BUF[0][10]='\0';
	memcpy(&GPS_FIX_DATA_BUF[0][0],&GPS_DATA_BUF_TMP[bufline][comma[5]+1],(comma[6]-comma[5]-1));
	GPS_FIX_DATA_BUF[0][1]='\0';
	memcpy(&GPS_ALTITUDE_DATA_BUF[0][0],&GPS_DATA_BUF_TMP[bufline][comma[8]+1],(comma[9]-comma[8]-1));
	GPS_ALTITUDE_DATA_BUF[0][5]='\0';
}



/*********************************************************************//**
 * @brief	NMEA_GxRMC_RECEIVED
 ************************************************************************/
/*********************************************************************************
$GxRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A

Where:
     RMC          Recommended Minimum sentence C
     123519       Fix taken at 12:35:19 UTC
     A            Status A=active or V=Void.
     4807.038,N   Latitude 48 deg 07.038' N
     01131.000,E  Longitude 11 deg 31.000' E
     022.4        Speed over the ground in knots
     084.4        Track angle in degrees True
     230394       Date - 23rd of March 1994
     003.1,W      Magnetic Variation
     *6A          The checksum data, always begins with *
*************************************************************************************/
void NMEA_GxRMC_RECEIVED(int bufferline)
{

	int bufline = bufferline;
	int i,j = 0;
	uint8_t comma[14];
	//uint8_t time[10]={};
	//printf("GPRMC data has beed updated --> bufferline: %d\n",bufline);	
	
	for (i=0; i<GPS_BUFFER_LENGHT; i++)
	{
	 if(GPS_DATA_BUF_TMP[bufline][i] == ',')
	 {
		comma[j]= i;
		j++;
	 }
	 if (j > 14)
	 {
		 break;
	 }
	}
	memcpy(&GPS_VELOCITY_BUF[0][0],&GPS_DATA_BUF_TMP[bufline][comma[6]+1],(comma[7]-comma[6]-1));
	GPS_VELOCITY_BUF[0][5]='\0';
	memcpy(&GPS_DATE_BUF[0][0],&GPS_DATA_BUF_TMP[bufline][comma[8]+1],(comma[9]-comma[8]-1));
	GPS_DATE_BUF[0][6]='\0';
}


/*********************************************************************//**
 * @brief	GPS DATA UPDATE
 ************************************************************************/

void GPSdataUpdate(void)
{
	for ( int k=0; k<GPS_BUFFER_HEIGHT; k++ )
	{
		if (GPS_DATA_BUF_TMP[k][0] == '$')
		{
			if (GPS_DATA_BUF_TMP[k][1] == 'G')
			{
				if (GPS_DATA_BUF_TMP[k][2] == 'P' || GPS_DATA_BUF_TMP[k][2] == 'L' || GPS_DATA_BUF_TMP[k][2] == 'S' || GPS_DATA_BUF_TMP[k][2] == 'N' )
				{
					if (GPS_DATA_BUF_TMP[k][3] == 'G')          
					{
						if (GPS_DATA_BUF_TMP[k][4] == 'G')				//  
						{
							if (GPS_DATA_BUF_TMP[k][5] == 'A')			// $GPGGA,$GLGGA,$GSGGA,$GNGGA
							{
								NMEA_GxGGA_RECEIVED(k);									
							}							
						}
					}
					else if (GPS_DATA_BUF_TMP[k][3] == 'R')		//
					{
						if (GPS_DATA_BUF_TMP[k][4] == 'M')			//  
						{
							if (GPS_DATA_BUF_TMP[k][5] == 'C')		// $GPRMC,$GLRMC,$GNRMC,$GSRMC
							{
								NMEA_GxRMC_RECEIVED(k);								
							}						
						}		
					}
				}					
			}		
		}	
	}
}

/*********************************************************************//**
 * @brief	USART1_Interrupt Handler
 ************************************************************************/

void USART1_IRQHandler(void)
{
	if(USART1->ISR & USART_ISR_RXNE)
	{	
		/* Reads and CLEARS RXNE Flag */
		RecDataToGPSbuffer(USART1->RDR);
	}
}

/**
  \fn          void USART1_Init(void)
  \brief       Initializes USART1 on PA9 and PA10
								
*/


/*********************************************************************//**
 * @brief	USART1 Init
 ************************************************************************/
void USART1_Init(void){
	
	 /* Local variables */
  uint16_t USARTDIV = 0;
  uint16_t USART_FRACTION = 0;
  uint16_t USART_MANTISSA = 0;

	RCC->IOPENR   |=   RCC_IOPENR_GPIOAEN;			/* Enable GPIOA clock */
	RCC->APB2ENR  |=   RCC_APB2ENR_USART1EN;    /* Enable USART#1 clock */
	
	//interrupt init
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn,0);
	
//	//Make PA8 an input with a pull up resistor
//	GPIOA->MODER &= ~(( 3ul << 2* 8) | ( 3ul << 2* 8) ); /* Set to input */
//	GPIOA->PUPDR &= ~(( 3ul << 2* 8) | ( 3ul << 2* 8) ); /* Set to 0 */
//	GPIOA->PUPDR |=  (( 2ul << 2* 8) | ( 2ul << 2* 8) ); /* Set to pull down */
	
	//Configure PA9 to USART1_TX, PA10 to USART1_RX
  GPIOA->AFR[1] &= ~((15ul << 4* 1) | (15ul << 4* 2) );		/* Set to 0 */
  GPIOA->AFR[1] |=  (( 4ul << 4* 1) | ( 4ul << 4* 2) );		/* Set to alternate function 4 */
  GPIOA->MODER  &= ~(( 3ul << 2* 9) | ( 3ul << 2* 10) );		/* Set to 0 */
  GPIOA->MODER  |=  (( 2ul << 2* 9) | ( 2ul << 2* 10) );		/* Set to alternate function mode */
	
	  /* Check to see if oversampling by 8 or 16 to properly set baud rate*/
  if((USART1->CR1 & USART_CR1_OVER8) == 1){
  	USARTDIV = PCLK/BAUD;
  	USART_FRACTION = ((USARTDIV & 0x0F) >> 1) & (0xB);
  	USART_MANTISSA = ((USARTDIV & 0xFFF0) << 4);
  	USARTDIV = USART_MANTISSA | USART_FRACTION;
  	USART1->BRR = USARTDIV;															/* 9600 Baud with 32MHz peripheral clock 8bit oversampling */
  }
  else{
  	USART1->BRR = PCLK/BAUD;														/* 9600 Baud with 32MHz peripheral clock 16bit oversampling */	
  }

  USART1->CR3    = 0x0000;								/* no flow control */
  //USART1->CR2    |= USART_CR2_SWAP;				/* Swap Tx and Rx */
	
	/* 1 stop bit, 8 data bits */
  USART1->CR1    = ((USART_CR1_RE) |												/* enable RX  */
                     (USART_CR1_TE) |												/* enable TX  */
                     (USART_CR1_UE) |      									/* enable USART */
										 (USART_CR1_RXNEIE));										/* Enable Interrupt */
}


/*********************************************************************//**
 * @brief	USART1 Put One Char
	 @fn				char USART1_PutChar(char ch)
	 @returns	ch: The character written to the USART
 ************************************************************************/

char USART1_PutChar(char ch) {

	//Wait for buffer to be empty
  while ((USART1->ISR & USART_ISR_TXE) == 0){
			//Nop
	}
	
	//Send character
  USART1->TDR = (ch);

  return (ch);
}

/*********************************************************************//**
 * @brief	Sends a string to the USART
	 @fn				void USART1_Send(char c[])
 ************************************************************************/
void USART1_Send(char c[]){
	
	int String_Length = strlen(c);
	int Counter = 0;
	
	while(Counter < String_Length){
		USART1_PutChar(c[Counter]);
		Counter++;
	}
}

