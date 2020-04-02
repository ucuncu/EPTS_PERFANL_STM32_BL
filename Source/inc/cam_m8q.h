/*------------------------------------------------------------------------------------------------------
 * Name:    cam_m8q.h 
 *----------------------------------------------------------------------------------------------------*/

/*-------------------------------------------Include Statements---------------------------------------*/
#include "stm32l053xx.h"

#ifndef cam_m8q_H
#define cam_m8q_H

#define GPS_BUFFER_LENGHT  82  //Max NMEA data 80 byte, 1 byte for safety,1 byte termination char '/0' 
#define GPS_BUFFER_HEIGHT  2
#define GPS_DATA_BUFFER_HEIGHT  1
#define GPS_TIME_BUFFER_LENGHT  10
#define GPS_LAT_BUFFER_LENGHT  11
#define GPS_LON_BUFFER_LENGHT  12
#define GPS_FIX_BUFFER_LENGHT  2
#define GPS_VELOCITY_BUFFER_LENGHT  6
#define GPS_DATE_BUFFER_LENGHT  7
#define GPS_ALTITUDE_BUFFER_LENGHT  6

/* Initialization methods */
extern void USART1_Init(void);
extern void initGPSubxData(void);
extern void GPSdataUpdate(void);

/* USART Methods */
extern int USART1_GetChar(void);
extern char USART1_PutChar(char character);
extern void USART1_Read(void);
extern void USART1_Send(char c[]);


extern char GPS_DATA_BUF[GPS_BUFFER_HEIGHT][GPS_BUFFER_LENGHT];
extern char GPS_DATA_BUF_TMP[GPS_BUFFER_HEIGHT][GPS_BUFFER_LENGHT];
extern char GPS_TIME_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_TIME_BUFFER_LENGHT];
extern char GPS_LAT_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_LAT_BUFFER_LENGHT];
extern char GPS_LON_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_LON_BUFFER_LENGHT];
extern char GPS_FIX_DATA_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_FIX_BUFFER_LENGHT];
extern char GPS_VELOCITY_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_VELOCITY_BUFFER_LENGHT];
extern char GPS_DATE_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_DATE_BUFFER_LENGHT];
extern char GPS_ALTITUDE_DATA_BUF[GPS_DATA_BUFFER_HEIGHT][GPS_ALTITUDE_BUFFER_LENGHT];


#endif
