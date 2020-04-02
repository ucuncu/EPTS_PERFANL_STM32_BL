/*
 ******************************************************************************
 * @file    mma8452qc.c
 * @author  EPTS Team
 * @brief   mma8452q driver file
 ******************************************************************************
*/
#include "flags.h"
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>	
#include "mma8452q.h"
#include <math.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 	
	
#define WORD_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
#define WORD_TO_BINARY(word)  \
  (word & 0x8000 ? '1' : '0'), \
  (word & 0x4000 ? '1' : '0'), \
  (word & 0x2000 ? '1' : '0'), \
  (word & 0x1000 ? '1' : '0'), \
  (word & 0x0800 ? '1' : '0'), \
  (word & 0x0400 ? '1' : '0'), \
  (word & 0x0200 ? '1' : '0'), \
  (word & 0x0100 ? '1' : '0'), \
  (word & 0x0080 ? '1' : '0'), \
  (word & 0x0040 ? '1' : '0'), \
  (word & 0x0020 ? '1' : '0'), \
  (word & 0x0010 ? '1' : '0'), \
  (word & 0x0008 ? '1' : '0'), \
  (word & 0x0004 ? '1' : '0'), \
  (word & 0x0002 ? '1' : '0'), \
  (word & 0x0001 ? '1' : '0')
	

int TMP_ACC_X_BUF[10];
int TMP_ACC_Y_BUF[10];
int TMP_ACC_Z_BUF[10];

int ACC_X_BUF = 0;
int ACC_Y_BUF = 0;
int ACC_Z_BUF = 0;

int accErrCnt = 0;
int accCnt = 0;

void recordToAccBuffer( int accX, int accY, int accZ);
void filterAccData(int a[], int b[], int c[], int array_size);
//void updateAccBuffer( void );	

/**********************************************************************
 * @brief		Check Accelerometer - Read "Who am I" flag
 **********************************************************************/
void checkAccelerometer(void)
{
	I2C_Read_Reg(MMA8452Q_ADD, MMA8452Q_WHO_AM_I_ADD);	
//	if (I2C1->RXDR == 42)
//	{
//	}
//	else
//	{
//	}
}

/**********************************************************************
 * @brief		INIT AND START ACCELEROMETER
 **********************************************************************/
void start_acc (void)
{	
	uint32_t ACC_DATA[1] = {0x00};
	//**
	ACC_DATA[0] = 0x00;	
	I2C_Write_Bulk(MMA8452Q_ADD, 0x2A, ACC_DATA, 1);	// Reset  
	//**
	ACC_DATA[0] = 0x02;
	I2C_Write_Bulk(MMA8452Q_ADD, 0x2B, ACC_DATA, 1);	//	High Resolution		
	//**
	ACC_DATA[0] = 0x00;
	I2C_Write_Bulk(MMA8452Q_ADD, 0x2C, ACC_DATA, 1);		
	//**/	
	ACC_DATA[0] = 0x01;
	I2C_Write_Bulk(MMA8452Q_ADD, 0x2D, ACC_DATA, 1);	//Data Ready Int Enabled
	//**
	ACC_DATA[0] = 0x10;
	I2C_Write_Bulk(MMA8452Q_ADD, 0x0E, ACC_DATA, 1);	//	High Pass Filter Enabled, range to +/- 2g
	//**
	ACC_DATA[0] = 0x1D;
	I2C_Write_Bulk(MMA8452Q_ADD, 0x2A, ACC_DATA, 1);	// 	ODR 100Hz, Select mode register(0x2A)	// Active mode(0x01)	
	//**
}


/**********************************************************************
 * @brief		INIT AND START ACCELEROMETER
 **********************************************************************/
void acc_low_power_mode (void)
{	
	//**
}


/**********************************************************************
 * @brief		READ ACCELEROMETER
 **********************************************************************/
void read_acc (void)
{
		uint32_t I2CSlaveBuffer[6] = {0,0,0,0,0,0};
		volatile int decAcc[3];
		volatile int16_t newAcc[3];
		decAcc[0]=0;
		decAcc[1]=0;
		decAcc[2]=0;
	
		fReadAcc = 0;
		
		I2CSlaveBuffer[0] = I2C_Read_Reg(MMA8452Q_ADD,OUT_X_MSB);
		I2CSlaveBuffer[1] = I2C_Read_Reg(MMA8452Q_ADD,OUT_X_LSB);
		I2CSlaveBuffer[2] = I2C_Read_Reg(MMA8452Q_ADD,OUT_Y_MSB);
		I2CSlaveBuffer[3] = I2C_Read_Reg(MMA8452Q_ADD,OUT_Y_LSB);
		I2CSlaveBuffer[4] = I2C_Read_Reg(MMA8452Q_ADD,OUT_Z_MSB);
		I2CSlaveBuffer[5] = I2C_Read_Reg(MMA8452Q_ADD,OUT_Z_LSB);
		
	  for (int i=0; i<6; i+=2)
		{
			newAcc[i/2] = ((I2CSlaveBuffer[i] << 8) | I2CSlaveBuffer[i + 1]) >> 4;  // Turn the MSB and LSB into a 12-bit value
		}	

	  for (int i=0; i<3; i+=1)
		{		
			if (newAcc[i] & (1<<0))
			{
				decAcc[i] += 1;
			}
			if (newAcc[i] & (1<<1))
			{
				decAcc[i] += 2;
			}		
			if (newAcc[i] & (1<<2))
			{
				decAcc[i] += 4;
			}
			if (newAcc[i] & (1<<3))
			{
				decAcc[i] +=  8;
			}
			if (newAcc[i] & (1<<4))
			{
				decAcc[i] +=  15;
			}
			if (newAcc[i] & (1<<5))
			{
				decAcc[i] +=  31;
			}
			if (newAcc[i] & (1<<6))
			{
				decAcc[i] += 63;
			}		
			if (newAcc[i] & (1<<7))
			{
				decAcc[i] += 125;
			}
			if (newAcc[i] & (1<<8))
			{
				decAcc[i] += 250;
			}
			if (newAcc[i] & (1<<9))
			{
				decAcc[i] += 500;
			}		
			if (newAcc[i] & (1<<10))
			{
				decAcc[i] += 1000;
			}
			if (newAcc[i] & (1<<11))
			{
				decAcc[i] -= 2000;
			}
		}			
			recordToAccBuffer(decAcc[0],decAcc[1],decAcc[2]);
}

/**********************************************************************
 * @brief		RECORD TO ACCELEROMETER BUFFER
 **********************************************************************/
void recordToAccBuffer( int accX, int accY, int accZ)
{
		int sampleNumber = 10;
	TMP_ACC_X_BUF[accCnt] = accX;
	TMP_ACC_Y_BUF[accCnt] = accY;
	TMP_ACC_Z_BUF[accCnt] = accZ;
	
	accCnt +=1;
	if (accCnt >= sampleNumber)
	{		
		accCnt = 0;
		filterAccData(TMP_ACC_X_BUF, TMP_ACC_Y_BUF, TMP_ACC_Z_BUF,  sampleNumber); // en küçük ve büyük degeri çikarip ortalama alir
		//updateAccBuffer();
	}	
}

/**********************************************************************
 * @brief		UPDATE ACCELEROMETER BUFFER
 **********************************************************************/
// void updateAccBuffer( void )
// {
//	 memcpy(ACC_X_BUF, TMP_ACC_X_BUF, sizeof(TMP_ACC_X_BUF));
//	 memcpy(ACC_Y_BUF, TMP_ACC_Y_BUF, sizeof(TMP_ACC_Y_BUF));
//	 memcpy(ACC_Z_BUF, TMP_ACC_Z_BUF, sizeof(TMP_ACC_Z_BUF));
// } 

/**********************************************************************
 * @brief		FILTER ACCELEROMETER DATA
 **********************************************************************/
 void filterAccData(int a[], int b[], int c[], int array_size)
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
		
		ACC_X_BUF = (a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + a[8])/8;
		ACC_Y_BUF = (b[1] + b[2] + b[3] + b[4] + b[5] + b[6] + b[7] + b[8])/8;
		ACC_Z_BUF = (c[1] + c[2] + c[3] + c[4] + c[5] + c[6] + c[7] + c[8])/8;
	
 }  

