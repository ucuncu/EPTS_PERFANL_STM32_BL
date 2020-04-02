/*------------------------------------------------------------------------------------------------------
 * Name:    cam_m8q.h 
 *----------------------------------------------------------------------------------------------------*/

/*-------------------------------------------Include Statements---------------------------------------*/
#include "stm32l053xx.h"

#ifndef zigbee_H
#define zigbee_H

/* Initialization methods */
extern void conGpsDataToDecDegree(void);
void packAllDataforZigbee(void);


 typedef struct AT_Data
 {
	 char MY[6];		/* Source Address unique to this device */
	 char ID[6];		/* Personal Area Network 								*/
	 char DH[9];		/* Destination register high 						*/
	 char DL[9];		/* Destination register low							*/
	 char CH[4];		/* Channel Selection										*/
 } AT_Data;
 
 //extern void LPUART_Init(void);
 extern void Configure_GPIO_LPUART(void);
 extern void Configure_LPUART(void);
 extern void XBee_ProS1_Init(void);
 extern void XBee_900HP_Init(void);
 extern void LPUART1_Send(char c[]);
 extern void LPUART1_Send_DataPack(char c[]);
 extern void Read_Xbee_ProS1_Init(void);
 extern void IMUtoString(void);
 extern char dataPack[330];
 
void Wait_For_OK(void);
void Wait_For_Data(void);

#endif
