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
#include <math.h>	
#include "flags.h"
#include "systick.h"
#include "cam_m8q.h"
#include "zigbee.h"
#include "mma8452q.h"  
#include "fxas21002.h"  
#include "iis2mdc_reg.h" 

#define deneme					"hamit_epts\r"
/*---------------------------------XBee Commands----------------------------------------------------------------------*/
/* Prefix(AT) + ASCII Command + Space(Optional) + Parameter(Optional,HEX) + Carridge Return */
#define ENTER_AT_COMMAND_MODE					"+++"					//Enter three plus characters within 1s there is no \r on purpose
#define EXIT_AT_COMMAND_MODE					"ATCN\r"			//Exit AT command mode
#define READ_SERIAL_ADDRESS_HIGH			"ATSH\r"			//Read the high bits of the serial address
#define READ_SERIAL_ADDRESS_LOW				"ATSL\r"			//Read the lower bits of the serial address
#define SET_ATMY											"ATMY 2\r"		//Source address, unique to this device - (Address of XBee B)
#define SET_ATID											"ATID 3001\r"	//Personal Area Network = 3001
#define SET_ATDH											"ATDH 0\r"		//Destination Address high
#define SET_ATDL											"ATDL 1\r"		//Destination Address Low - (The address of XBee A)
#define SET_ATCH											"ATCH c\r"		//Operating Channel Selection
#define SAVE_SETTINGS									"ATWR\r"			//Saves the Settings you have configured
#define READ_ATMY											"ATMY\r"			//Read MY register
#define READ_ATID											"ATID\r"			//Read ID register
#define READ_ATDH											"ATDH\r"			//Read DH register
#define READ_ATDL											"ATDL\r"			//Read DL register
#define READ_ATCH											"ADCH\r"			//Read CH register
/*---------------------------------XBee 900HP Commands----------------------------------------------------------------*/
#define SET_DESTINATION_H							"ATDH 13A200\r"	//Specific Serial Address of Matt's XBee
#define SET_DESTINATION_L							"ATDL 40E35DC2\r"	//Specific Serial Address of Matt's Xbee
#define SET_ATHP											"ATHP 5\r"				//The preamble ID, must be the same for XBee's to communicate
/*---------------------------------More defines-----------------------------------------------------------------------*/
#define TRUE	1;
#define FALSE 0;
#define PCLK	32000000									// Peripheral Clock
#define BAUD	38400											// Baud rate
/*---------------------------------Globals----------------------------------------------------------------------------*/
char 										RX_Data[512] = 				"";				//Rx
uint8_t 								ChIndex =							0;				//Character Index
char 										XBee_Message[512] = 		"";				//Message Recieved by the XBee
static const char				OK[] = 								"OK";			//When data has been written XBee will send an OK
uint8_t									Device_Ack_Flag = 		FALSE;		//XBee OK acknowledge
uint8_t									XBee_Ready_To_Read = 	FALSE;		//XBee data ready

char latString[12]  = {'0','0','0','0','0','0','0','0','0','0','0','\0'};
char lonString[12]  = {'0','0','0','0','0','0','0','0','0','0','0','\0'};
char serialized_string[33][10] = {NULL};
char dataPack[330];
char strAccX[6];
char strAccY[6];
char strAccZ[6];
char strGyroX[6];
char strGyroY[6];
char strGyroZ[6];
char strMagX[6];
char strMagY[6];
char strMagZ[6];
/*--------------------------------Struct Initialize-------------------------------------------------------------------*/
AT_Data AT;



/**
	\fn			void RNG_LPUART1_IRQHandler(void)
	\brief	Global interrupt handler for LPUART, Currently only handles RX
*/

void RNG_LPUART1_IRQHandler(void){
	if((LPUART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE){
		
		/* Read RX Data */
		RX_Data[ChIndex] = LPUART1->RDR;
		
		/* Check for parachute deployment signal */
		if(RX_Data[ChIndex] == '!'){
			//Servo_Position(180);
		}
		
		/* Check for end of recieved data */
		if(RX_Data[ChIndex] == '\r'){
			
			/* Compare string to OK to see if Acknowledged */
			if(strncmp(OK,RX_Data,(sizeof(OK)-1)) == 0){
				Device_Ack_Flag = TRUE;
			}
			
			/* Copy XBee message */
			strcpy(XBee_Message,RX_Data);
			
			/* set data ready to read flag */
			XBee_Ready_To_Read = TRUE;
			
			/* Clear RX_Data */
			ChIndex = 0;
			memset(RX_Data,0,sizeof(RX_Data));
			
		}else ChIndex++;
	}
}


///**
//	\fn			void LPUART_Init(void)
//	\brief	Initializes the Low Powered UART
//*/

//void LPUART_Init(void){
//	
//	RCC->IOPENR   |=   RCC_IOPENR_GPIOBEN;			/* Enable GPIOB clock */
//	RCC->APB1ENR  |=   RCC_APB1ENR_LPUART1EN;   /* Enable LP USART#1 clock */
//	
//	//interrupt init
//	NVIC_EnableIRQ(RNG_LPUART1_IRQn);
//	NVIC_SetPriority(RNG_LPUART1_IRQn,1);
//	
//	//Configure PB10 to LPUSART1_TX, PB11 to LPUSART1_RX,PB10 and PB11 TX,RX AF0 by default
//  GPIOB->MODER  &= ~(( 3ul << 2* 10) | ( 3ul << 2* 11) );		/* Set to 0 */
//  GPIOB->MODER  |=  (( 2ul << 2* 10) | ( 2ul << 2* 11) );		/* Set to alternate function mode */
//	
//	LPUART1->BRR  	= (unsigned long)((256.0f/BAUD)*PCLK); 		/* 9600 baud @ 32MHz */
//  LPUART1->CR3    = 0x0000;																	/* no flow control */
//	LPUART1->CR2    = 0x0000;																	/* 1 stop bit */
//	
//	/* 1 stop bit, 8 data bits */
//  LPUART1->CR1    = ((USART_CR1_RE) |												/* enable RX  */
//                     (USART_CR1_TE) |												/* enable TX  */
//                     (USART_CR1_UE) |      									/* enable USART */
//										 (USART_CR1_RXNEIE));										/* Enable Interrupt */
//										
//}


/**
  * Brief   This function :
             - Enables GPIO clock
             - Configures the LPUART pins on GPIO PB10 PB11
  * Param   None
  * Retval  None
  */
void Configure_GPIO_LPUART(void)
{
  /* Enable the peripheral clock of GPIOB */
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
	
  /* GPIO configuration for LPUART signals */
  /* (1) Select AF mode (10) on PB10 and PB11 */
  /* (2) AF4 for LPUART signals */
  GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODE10|GPIO_MODER_MODE11))\
                 | (GPIO_MODER_MODE10_1 | GPIO_MODER_MODE11_1); /* (1) */
  GPIOB->AFR[1] = (GPIOB->AFR[1] &~ (0x0000FF00))\
                  | (4 << (2 * 4)) | (4 << (3 * 4)); /* (2) */
}

/**
  * Brief   This function configures LPUART.
  * Param   None
  * Retval  None
  */
void Configure_LPUART(void)
{
	/* (1) Enable power interface clock */
  /* (2) Disable back up protection register to allow the access to the RTC clock domain */
  /* (3) LSE on */
  /* (4) Wait LSE ready */
  /* (5) Enable back up protection register to allow the access to the RTC clock domain */
  /* (6) LSE mapped on LPUART */
  /* (7) Enable the peripheral clock LPUART */
  /* Configure LPUART */
  /* (8) oversampling by 16, 9600 baud */
  /* (9) 8 data bit, 1 start bit, 1 stop bit, no parity, reception mode, stop mode */
  /* (10) Set priority for LPUART1_IRQn */
  /* (11) Enable LPUART1_IRQn */
  RCC->APB1ENR |= (RCC_APB1ENR_PWREN); /* (1) */
  PWR->CR |= PWR_CR_DBP; /* (2) */
  RCC->CSR |= RCC_CSR_LSEON; /* (3) */
  while ((RCC->CSR & (RCC_CSR_LSERDY)) != (RCC_CSR_LSERDY)) /* (4) */
  {
    /* add time out here for a robust application */
  }
  PWR->CR &=~ PWR_CR_DBP; /* (5) */
  RCC->CCIPR |= RCC_CCIPR_LPUART1SEL; /* (6) */
	RCC->APB1ENR |= RCC_APB1ENR_LPUART1EN; /* (7) */
  LPUART1->BRR = 0x369; /* (8) */
	LPUART1->CR1 = USART_CR1_UESM | USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE| USART_CR1_TE; /* (9) */
  NVIC_SetPriority(RNG_LPUART1_IRQn, 0); /* (10) */
  NVIC_EnableIRQ(RNG_LPUART1_IRQn); /* (11) */
}



/**
	\fn				char LPUART1_PutChar(char ch)
	\brief		Sends a character to the XBEE
	\returns 	char ch: The character sent to the XBEE
*/

char LPUART1_PutChar(char ch){

	//Wait for buffer to be empty
  while ((LPUART1->ISR & USART_ISR_TXE) == 0){
			//Nop
	}
	
	//Send character
  LPUART1->TDR = (ch);

  return (ch);
}

/**
	\fn				void LPUART1_Send(char c[])
	\brief		Sends a string to the XBEE
*/

void LPUART1_Send(char c[]){
	
	int String_Length = strlen(c);
	int Counter = 0;
	
	while(Counter < String_Length){
		LPUART1_PutChar(c[Counter]);
		Counter++;
	}
}

/**
	\fn				void LPUART1_Send(char c[])
	\brief		Sends a string to the XBEE
*/

void LPUART1_Send_DataPack(char c[]){
	
	int Counter = 0;
	
	while(Counter < 330){
		LPUART1_PutChar(c[Counter]);
		Counter++;
	}
}

/**
	\fn				void XBee_900HP_Init(void)
	\brief		Initializes the XBEE
*/
void XBee_900HP_Init(void){
	/* Enter AT command mode */
	//Delay(1000);
	LPUART1_Send(ENTER_AT_COMMAND_MODE);
	Wait_For_OK();
	
	/* Serial Address of matt's xbee */
	LPUART1_Send(SET_DESTINATION_H);
	Wait_For_OK();
	
	LPUART1_Send(SET_DESTINATION_L);
	Wait_For_OK();
	
	/* PAN = 3001 */
	LPUART1_Send(SET_ATID);
	Wait_For_OK();
	
	/* HP = 5 */
	LPUART1_Send(SET_ATHP);
	Wait_For_OK();
	
	/* Save the settings */
	LPUART1_Send(SAVE_SETTINGS);
	Wait_For_OK();
	
	/* End AT command mode */
	LPUART1_Send(EXIT_AT_COMMAND_MODE);
	Wait_For_OK();
	
//	printf("#####  XBee 	       Initialized  #####\r\n");
}


/**
	\fn				void XBee_Init(void)
	\brief		Initializes the XBEE
*/

void XBee_ProS1_Init(void){
	
	/* Enter AT command mode */
	//Delay(1000);
	LPUART1_Send(ENTER_AT_COMMAND_MODE);
	//Wait_For_OK();
	
	/* MY address = 2 */
	LPUART1_Send(SET_ATMY);
	//Wait_For_OK();
	
	/* PAN = 3001 */
	LPUART1_Send(SET_ATID);
	Wait_For_OK();
	
	/* Set Destination address high */
	LPUART1_Send(SET_ATDH);
	Wait_For_OK();
	
	/* Set Destination Address low */
	LPUART1_Send(SET_ATDL);
	Wait_For_OK();
	
	/* Set Channel */
	LPUART1_Send(SET_ATCH);
	Wait_For_OK();
	
	/* Save the settings */
	LPUART1_Send(SAVE_SETTINGS);
	Wait_For_OK();
	
	/* End AT command mode */
	LPUART1_Send(EXIT_AT_COMMAND_MODE);
	Wait_For_OK();
	
//	printf("#####  XBee 	       Initialized  #####\r\n");
}

/**
	\fn				void Read_Xbee_Init(void)
	\brief		Reads what the XBEE was initialized to
*/

void Read_Xbee_ProS1_Init(void){
	
	/* Enter AT command mode */
	//Delay(1000);
	LPUART1_Send(ENTER_AT_COMMAND_MODE);
	
	/* Wait for XBee Acknowledge */
	//Wait_For_OK();
	
	/* Read Personal Area Network */
	LPUART1_Send(READ_ATID);
	//Wait_For_Data();
	strncpy(AT.ID,XBee_Message,6);
	
	/* Read MY Address */
	LPUART1_Send(READ_ATMY);
	//Wait_For_Data();
	strncpy(AT.MY,XBee_Message,6);
	
	/* Read Destination address */
	LPUART1_Send(READ_ATDH);
	//Wait_For_Data();
	strncpy(AT.DH,XBee_Message,9);
	
	LPUART1_Send(READ_ATDL);
	//Wait_For_Data();
	strncpy(AT.DL,XBee_Message,9);
	
	/* Read Channel */
	LPUART1_Send(READ_ATCH);
	//Wait_For_Data();
	strncpy(AT.CH,XBee_Message,4);
	
	/* End AT command mode */
	LPUART1_Send(EXIT_AT_COMMAND_MODE);
	//Wait_For_OK();
	
	/* Print to serial monitor */
//	printf("PAN: %s\r\n",AT.ID);
//	printf("MY: %s\r\n",AT.MY);
//	printf("DH: %s\r\n",AT.DH);
//	printf("DL: %s\r\n",AT.DL);
//	printf("CH: %s\r\n",AT.CH);
	
}

/**
	\fn				void Wait_For_OK(void)
	\brief		Waits until the XBEE has sent the OK message
						This is done when it has changed its settings
*/

void Wait_For_OK(void){
	
	/* Wait for XBee Acknowledge */
	while(Device_Ack_Flag == 0){
		//Nop
	}
	
	/* Reset Flags */
	Device_Ack_Flag = FALSE;
	XBee_Ready_To_Read = FALSE;
	
}

/**
	\fn				void Wait_For_Data(void)
	\brief		This waits for the data the XBEE writes to the bus
*/

void Wait_For_Data(void){
	
	/* Wait for data to be copied */
	while(XBee_Ready_To_Read == 0){
		//Nop
	}
	
	/* Reset Flags */
	Device_Ack_Flag = FALSE;
	XBee_Ready_To_Read = FALSE;
}



/**********************************************************************
 * @brief		Functions to Convert Float to String
 **********************************************************************/
// reverses a string 'str' of length 'len' 
void reverse(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; 
    } 
} 
  
 // Converts a given integer x to string str[].  d is the number 
 // of digits required in output. If d is more than the number 
 // of digits in x, then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) 
    { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
void ftoa(float n, char *res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) 
    { 
        res[i] = '.';  // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter is needed 
        // to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 

/**********************************************************************
 * @brief		Convert GPS Data fto Decimal Degree Format
 **********************************************************************/
void conGpsDataToDecDegree(void)
{
	float lat = 0;
	float latNew = 0;
	double latInt= 0;
	float latFraction= 0;
	float lon= 0;
	float lonNew= 0;
	double lonInt= 0;
	float lonFraction= 0;

	//GPS value format dönüsümü
	lat = atof(&GPS_LAT_BUF[0][0])/100;
	lon = atof(&GPS_LON_BUF[0][0])/100;
	latFraction =100*modf(lat, &latInt)/60;
	lonFraction =100*modf(lon, &lonInt)/60;
	latNew = latInt + latFraction;
	lonNew = lonInt + lonFraction;
	ftoa(latNew, latString, 8); 
	ftoa(lonNew, lonString, 8);		
	
}


// inline function to swap two numbers
void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverseInt(char *buffer, int i, int j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = abs(value);

	int i = 0;
	while (n)
	{
		int r = n % base;

		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string 
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverseInt(buffer, 0,(i-1));
}


/**********************************************************************
 * @brief		IMU Val to String
 **********************************************************************/
void IMUtoString(void)
{
	itoa(ACC_X_BUF, strAccX, 10);
	itoa(ACC_Y_BUF, strAccY, 10);
	itoa(ACC_Z_BUF, strAccZ, 10);
	itoa(GYRO_X_BUF, strGyroX, 10);
	itoa(GYRO_Y_BUF, strGyroY, 10);
	itoa(GYRO_Z_BUF, strGyroZ, 10);
	itoa(MAG_X_BUF, strMagX, 10);
	itoa(MAG_Y_BUF, strMagY, 10);
	itoa(MAG_Z_BUF, strMagZ, 10);
}	

/**********************************************************************
 * @brief		Pack ALL Sensor Data for Zigbee
 **********************************************************************/
void packAllDataforZigbee(void)
{
	strcpy(serialized_string[0], "EPTS00001");
	strcpy(serialized_string[1], "&");
	strcpy(serialized_string[2], &GPS_FIX_DATA_BUF[0][0]);
	strcpy(serialized_string[3], "&");	
	strcpy(serialized_string[4], &GPS_DATE_BUF[0][0]);
	strcpy(serialized_string[5], "&");	
	strcpy(serialized_string[6], &GPS_TIME_BUF[0][0]);
	strcpy(serialized_string[7], "&");	
	strcpy(serialized_string[8], latString);
	strcpy(serialized_string[9], "&");	
	strcpy(serialized_string[10], lonString);
	strcpy(serialized_string[11], "&");	
	strcpy(serialized_string[12], &GPS_ALTITUDE_DATA_BUF[0][0]);
	strcpy(serialized_string[13], "&");	
	strcpy(serialized_string[14], &GPS_VELOCITY_BUF[0][0]);
	strcpy(serialized_string[15], "&");
	strcpy(serialized_string[16], strAccX);
	strcpy(serialized_string[17], "&");
	strcpy(serialized_string[18], strAccY);
	strcpy(serialized_string[19], "&");	
	strcpy(serialized_string[20], strAccZ);
	strcpy(serialized_string[21], "&");	
	strcpy(serialized_string[22], strGyroX);
	strcpy(serialized_string[23], "&");	
	strcpy(serialized_string[24], strGyroY);
	strcpy(serialized_string[25], "&");	
	strcpy(serialized_string[26], strGyroZ);
	strcpy(serialized_string[27], "&");	
	strcpy(serialized_string[28], strMagX);
	strcpy(serialized_string[29], "&");	
	strcpy(serialized_string[30], strMagY);
	strcpy(serialized_string[31], "&");
	strcpy(serialized_string[32], strMagZ);	
;
	memcpy(dataPack, serialized_string, 330);
	dataPack[329] = '\0'; 
}	
