#ifndef _NXP_LCD_DRIVER_H_
#define _NXP_LCD_DRIVER_H_

#include <stdint.h>

// From a software viewpoint, we have 5 LCDs:
#define LCD_L1 1  /* Large display, line 1 (H4235, top line, 6 digits) */
#define LCD_L2 2  /* Large display, line 2 (H4235, bottom line, 6 digits) */
#define LCD_S1 3  /* Small display, left (H4198, 4 digits) */
#define LCD_S2 4  /* Small display, middle (H4198, 4 digits) */
#define LCD_S3 5  /* Small display, right (H4198, 4 digits) */


// NXP Controller IC's I2C addresses are 0x70 or 0x72
#define LCD_A1 0x70  /* H4198 displays (up to 3, with NXP PCF85176 ICs */
#define LCD_A2 0x72  /* H4235's two sub-displays (2 NXP PCF85134 ICs */

#define timeOutVal 1000000

// Initialize the pic's I2C interface, and the NXP LCD control ICs.
void nxpInit(void);


// Write a string to one of the LCDs
int lcdWrite(int lcd,  // LCD to write to (LCD_L1, LCD_L2, LCD_S1,... )
             char *s); // The string to write; usually digits, with optional periods or commas
             

// ---------------------------------------------------------------------
// Private functions - not intended for external use

// Given a string to display, convert to a raw segmentData[] array
// that, if sent to the LCD controller, would display the input string.
int h4198_SetSegments(const char *displayStr,   // Display string to process
                      uint8_t segmentData[5]);  // Return 5 data bytes (40segments)
int h4235_SetSegments(const char *displayStr,   // String to display
                      uint8_t segmentData[8]);  // 60 bits (7.5 bytes) segment data

// Send a raw segmentData[] array to the LCD controller IC
int h4198_Write(int dispNumber, uint8_t segmentData[5]);
int h4235_Write(int dispNumber, uint8_t segmentData[8]);


int nxpRawWrite(uint8_t i2c_address, uint8_t data[], int n);
uint8_t sevenSegCode(char c);

extern uint32_t myWatchDogCnt;
extern uint32_t afterWakeUpCnt;
extern uint32_t startManuelWorkDelay;


extern uint32_t LCD_Ram_Data_Row[30];
extern uint8_t powerON;
extern uint8_t fPowerOnRelay1OFF;
extern uint8_t fPowerOnRelay2OFF;
extern uint8_t fPowerOnRelay3OFF;
extern uint8_t fPowerOnRelay4OFF;
extern uint32_t cntPowerOn4second;
extern float batteryVoltage;
extern uint32_t delayed_irr_counter;  // aralikli sulama sayaci, 

extern void LCDPowerOFF(void);
extern void LCDPowerON(void);
extern void LCDBufferReset(void);

extern uint8_t fhomePage;
extern uint8_t fwatchAdjustment;
extern uint8_t fselenoidProgramming;
extern uint32_t	flashingCnt;
extern uint32_t	flashingCnt2;
extern uint32_t	flashingCnt3;
extern uint32_t	flashingCnt4;
extern uint32_t	flashingCnt5;
extern uint32_t	flashingCnt6;
extern uint32_t	flashingCnt7;
extern uint32_t	flashingCnt8;
extern uint32_t manuelIrrCnt;

extern uint8_t fHergunSelected;
extern uint8_t fDelayedIrrigationSelected;
extern uint8_t fSpecialSelected;
 
extern uint8_t fdayOfWeekAdjust;
extern uint8_t fminuteAdjust;	
extern uint8_t fhourAdjust;	

extern uint8_t fdayAdjust;	
extern uint8_t fmonthAdjust;		
extern uint8_t fyearAdjust;	

extern uint8_t selectedSelenoid;	

extern uint8_t fselectedSelenoidAdjust;	
extern uint8_t fSelenoidWorkDurAdjust;	
extern uint8_t fSelenoidStartTimeAdjust;
extern uint8_t fDelayValueAdjust;
extern uint8_t fmanuelIrrigationAdjust;
extern uint8_t fmanuelIrrigationPlanAdjust;
extern uint8_t fPasswordAdjust;
extern uint8_t fManuelRelayAdjust;
extern uint8_t fActivateManuelMode;
extern uint8_t fManuelWorkDurationAdjust;
extern uint8_t fStartManuelWork;
extern uint8_t fStartAllRelayManuelWork;
extern uint8_t fActivateManuelPlanMode;
extern uint8_t fPasswordActivateMode;	
extern uint8_t fRecordtoEEPROM;
extern uint8_t fCopySelenoidSettings;	
extern uint8_t fcopyAndPasteSelSettings;	
extern uint8_t fPlanSelenoidSettings;
extern uint8_t fReadfromEEPROM;
extern uint8_t fShowBatteryLevel;
extern uint8_t fsetNewPasswordMode;	
extern uint8_t fPasswordDigit1Adjust;
extern uint8_t fPasswordDigit2Adjust;	
extern uint8_t fPasswordDigit3Adjust;
extern uint8_t fPasswordDigit4Adjust;	
extern uint8_t fUpdatePassword;
extern uint8_t fHomePageWithPassword;
extern uint8_t fEraseAllPrograms;
extern uint8_t fDeviceLockedWithPassword;
extern uint8_t fshowCurrentDayOfWeek;
extern uint8_t fWakeUpButtonPressed;
extern uint8_t fHSIOscillatorInit;
extern uint8_t fSetNextAlarm;
extern uint8_t fWakeUpFromRTC;
extern uint8_t fSystemOFF;
extern uint8_t finitRtcAlarm;
extern uint8_t fUpdateInvPassCnt;
extern uint8_t fproducerPasswordNeeded;
extern uint8_t fproducerPassword_1_Adj;
extern uint8_t fproducerPassword_2_Adj;
extern uint8_t fproducerPassword_3_Adj;
extern uint8_t fproducerPassword_4_Adj;
extern uint8_t fproducerPassword_5_Adj;
extern uint8_t fproducerPassword_6_Adj;
extern uint8_t fproducerPassword_7_Adj;
extern uint8_t fproducerPassword_8_Adj;
extern uint8_t fproducerPassword_9_Adj;
extern uint8_t fproducerPassword_10_Adj;
extern uint8_t fAnyRelayON;	
extern uint8_t fBatteryReady;	
extern uint8_t fAll_Her_Selected;
extern uint8_t fManuelAllRelayWorkDurAdj;
extern uint8_t fRelay1Status;
extern uint8_t fRelay2Status;
extern uint8_t fRelay3Status;
extern uint8_t fRelay4Status;
extern uint8_t fResetLCDBuffer;
extern uint8_t fcelenderPlanDelayed;
extern uint8_t fResetLCDBufferwithDelay;
extern uint32_t resetLCDBufferDelayMs;


extern uint8_t fSelStartTime_1_HourAdjust;
extern uint8_t fSelStartTime_1_MinuteAdjust;
extern uint8_t fSelStartTime_2_HourAdjust;
extern uint8_t fSelStartTime_2_MinuteAdjust;
extern uint8_t fSelStartTime_3_HourAdjust;
extern uint8_t fSelStartTime_3_MinuteAdjust;
extern uint8_t fSelStartTime_4_HourAdjust;
extern uint8_t fSelStartTime_4_MinuteAdjust;

extern uint8_t fSpecialDaySelect_PT;	
extern uint8_t fSpecialDaySelect_SA;	
extern uint8_t fSpecialDaySelect_CA;	
extern uint8_t fSpecialDaySelect_PE;	
extern uint8_t fSpecialDaySelect_CU;	
extern uint8_t fSpecialDaySelect_CT;	
extern uint8_t fSpecialDaySelect_PZ;		

extern uint8_t fOpenRelay_1_WithDelay;
extern uint8_t fOpenRelay_2_WithDelay;
extern uint8_t fOpenRelay_3_WithDelay;
extern uint8_t fOpenRelay_4_WithDelay;
extern uint32_t relayOpenDelayMs;
	
extern uint8_t factoryReseyKEY[10];
extern uint8_t factoryPASSWORD[10];

//CELENDER VALUES		//Relat_1_Start_Date_Time_1, Relat_1_Stop_Date_Time_2,...Relat_4_Start_Date_Time_3, Relat_4_Stop_Date_Time_3
extern uint32_t relayCelendar[32];

extern uint32_t pendingRtcIrq[32];
		
extern uint32_t pendingRelay[32];

//USER ADJUSTED VALUES
extern uint8_t sel_work_duration[4];
extern uint8_t sel_delayed_irr_val[4];
extern uint8_t sel_start_time_min[4][4];
extern uint8_t sel_start_time_hour[4][4];
extern uint8_t sel_special_irr_days[4];
extern uint8_t sel_manuel_work_duration[4];
extern uint8_t password[4];
extern uint8_t tmpPassword[4];
extern uint8_t fPasswordSetted;
extern uint8_t invalidPasswordCnt;
extern uint8_t all_manuel_work_duration;	

#endif
