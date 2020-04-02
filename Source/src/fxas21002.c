/*
 ******************************************************************************
 * @file    fxas21002.c
 * @author  EPTS Team
 * @brief   fxas210002 driver file
 ******************************************************************************
*/
#include "flags.h"
#include <stdint.h>
#include <stdbool.h>
#include "fxas21002.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>	
#include "i2c.h"
#include "math.h"

void recordToGyroBuffer( int gyroX, int gyroY, int gyroZ);
void filterGyroData(int a[], int b[], int c[], int array_size);
//void updateGyroBuffer( void );
 
volatile int16_t newGyro[3];

int TMP_GYRO_X_BUF[10];
int TMP_GYRO_Y_BUF[10];
int TMP_GYRO_Z_BUF[10];

//int GYRO_X_BUF[10];
//int GYRO_Y_BUF[10];
//int GYRO_Z_BUF[10];

int GYRO_X_BUF = 0;
int GYRO_Y_BUF = 0;
int GYRO_Z_BUF = 0;

//int GYRO_ERR_BUF = 0;

int gyroErrCnt = 0;
int gyroCnt = 0;

/**********************************************************************
 * @brief		Check Gyroscope - Read "Who am I" flag
 **********************************************************************/
void checkGyro(void)
{
	I2C_Read_Reg(FXAS21002_ADD,FXAS21002_WHO_AM_I_ADD);	
	//Check if device signature is correct
//	if (I2C1->RXDR == 215){
//	}
//	else
//	{
//	}
}



/**********************************************************************
 * @brief		INIT AND START GYROSCOPE
 **********************************************************************/
void start_gyro (void) 
{     
	//**
	uint32_t GYRO_DATA[1] = {0x00};
	//**
//	GYRO_DATA[0] = 0x40;     
//	I2C_Write_Bulk(FXAS21002_ADD,FXAS21002_REG_CTRLREG1, GYRO_DATA, 1);	// Reset all registers to POR values 	
//	for(int i=0; i <10000; i++);	 // a little delay
//	do                                                              // Wait for the RST bit to clear     
//	{         
//		reg_val = I2C_Read_Reg(FXAS21002_ADD, FXAS21002_REG_CTRLREG1) & 0x40;     
//	}    
//	while (reg_val);
//	//**
	GYRO_DATA[0] = 0x1F;
	I2C_Write_Bulk(FXAS21002_ADD,FXAS21002_REG_CTRLREG0, GYRO_DATA, 1);		// Active HPF, cut of freq 0.248Hz, 250 dps scale
	//**
	GYRO_DATA[0] = 0x0C;
	I2C_Write_Bulk(FXAS21002_ADD,FXAS21002_REG_CTRLREG2, GYRO_DATA, 1);		// Data Ready interrupt enabled on INT1
	//**
	GYRO_DATA[0] = 0x0E;
	I2C_Write_Bulk(FXAS21002_ADD,FXAS21002_REG_CTRLREG1, GYRO_DATA, 1);	// ODR = 100 Hz, Active mode 	
	//**
}


/**********************************************************************
 * @brief		INIT AND START GYROSCOPE
 **********************************************************************/
void gyro_low_power_mode (void) 
{     
	//
}

/**********************************************************************
 * @brief		READ GYROSCOPE
 **********************************************************************/
void read_gyro (void)
{	
	uint32_t I2CSlaveBuffer[6] = {0,0,0,0,0,0};

	fReadGyro = 0;
	
	I2CSlaveBuffer[0] = I2C_Read_Reg(FXAS21002_ADD,FXAS21002_REG_OUTXMSB);
	I2CSlaveBuffer[1] = I2C_Read_Reg(FXAS21002_ADD,FXAS21002_REG_OUTXLSB);
	I2CSlaveBuffer[2] = I2C_Read_Reg(FXAS21002_ADD,FXAS21002_REG_OUTYMSB);
	I2CSlaveBuffer[3] = I2C_Read_Reg(FXAS21002_ADD,FXAS21002_REG_OUTYLSB);
	I2CSlaveBuffer[4] = I2C_Read_Reg(FXAS21002_ADD,FXAS21002_REG_OUTZMSB);
	I2CSlaveBuffer[5] = I2C_Read_Reg(FXAS21002_ADD,FXAS21002_REG_OUTZLSB);

	 for (int i=0; i<6; i+=2)
	{
		newGyro[i/2] = ((I2CSlaveBuffer[i] << 8) | I2CSlaveBuffer[i+1]);  // Turn the MSB and LSB into a 16-bit value
	}		
	//printf("Gyro(mdps) on X, Y, Z: %d , %d , %d \n",newGyro[0],newGyro[1],newGyro[2]);
		recordToGyroBuffer(newGyro[0],newGyro[1],newGyro[2]);		
}

void recordToGyroBuffer( int gyroX, int gyroY, int gyroZ)
{
	int sampleNumber = 10;
	TMP_GYRO_X_BUF[gyroCnt] = gyroX;
	TMP_GYRO_Y_BUF[gyroCnt] = gyroY;
	TMP_GYRO_Z_BUF[gyroCnt] = gyroZ;
	
	gyroCnt +=1;
	if (gyroCnt >=  sampleNumber)
	{		
		gyroCnt = 0;
		filterGyroData(TMP_GYRO_X_BUF, TMP_GYRO_Y_BUF, TMP_GYRO_Z_BUF,  sampleNumber);
		 //updateGyroBuffer();
	}	
}


/**********************************************************************
 * @brief		UPDATE GYRO BUFFER
 **********************************************************************/
// void updateGyroBuffer( void )
// {
//	 memcpy(GYRO_X_BUF, TMP_GYRO_X_BUF, sizeof(TMP_GYRO_X_BUF));
//	 memcpy(GYRO_Y_BUF, TMP_GYRO_Y_BUF, sizeof(TMP_GYRO_Y_BUF));
//	 memcpy(GYRO_Z_BUF, TMP_GYRO_Z_BUF, sizeof(TMP_GYRO_Z_BUF));
// } 

 void filterGyroData(int a[], int b[], int c[], int array_size)
 {
		int i, j, temp;
	 // Data sorting
		for (i = 0; i < (array_size - 1); ++i)
		{
			for (j = 0; j < array_size - 1 - i; ++j )
			{
				 if (a[j] > a[j+1])
				 {
						temp = a[j+1];
						a[j+1] = a[j];
						a[j] = temp;
				 }
				 if (b[j] > b[j+1])
				 {
						temp = b[j+1];
						b[j+1] = b[j];
						b[j] = temp;
				 }
				 if (c[j] > c[j+1])
				 {
						temp = c[j+1];
						c[j+1] = c[j];
						c[j] = temp;
				 }
			}
		}
		
		// eleminete max and min value and mean value
		
		GYRO_X_BUF = (a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + a[8])/8;
		GYRO_Y_BUF = (b[1] + b[2] + b[3] + b[4] + b[5] + b[6] + b[7] + b[8])/8;
		GYRO_Z_BUF = (c[1] + c[2] + c[3] + c[4] + c[5] + c[6] + c[7] + c[8])/8;
			
 }  

///*******************************************************************************
// * EOF
// ******************************************************************************/
