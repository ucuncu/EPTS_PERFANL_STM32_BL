//
// nxp_lcd_driver
//


#include "nxp_lcd_driver.h"
#include "systick.h"
#include "gpio.h"
#include "i2c.h"
#include <ctype.h> 
#include <stdio.h> 
#include <button.h> 
#include <math.h> 


 void updateBatLevelDigit(void);

	// watch on
	uint32_t currentTime = 0;
	uint32_t	secondUnits = 0;
	uint32_t	secondTens = 0;
	uint32_t	minuteUnits = 0;
	uint32_t	minuteTens = 0;
	uint32_t	hourUnits = 0;
	uint32_t	hourTens = 0;
	
	uint32_t myWatchDogCnt = 0;
	uint32_t afterWakeUpCnt = 10000; // uyandiktan sonra hemen pil kontrolüne baslamasin, 10 sn ye sonra, stabilizasyon için
	uint32_t startManuelWorkDelay = 0;
	
	char C_secondUnits[1] = {'0'};
	char C_secondTens[1] = {'0'};
	char C_minuteUnits[1] = {'0'};
	char C_minuteTens[1] = {'0'};
	char C_hourUnits[1] = {'0'};
	char C_hourTens[1] = {'0'};

	uint32_t currentDate = 0;
	uint32_t dateUnits = 0;
	uint32_t dateTens = 0;
	uint32_t monthUnits = 0;
	uint32_t monthTens = 0;
	uint32_t yearUnits = 0;
	uint32_t yearTens = 0;	
	uint32_t currentDayOfWeek = 0;
	uint32_t delayed_irr_counter = 0;  // aralikli sulama sayaci, 

	
	char C_dateUnits[1] = {'0'};
	char C_dateTens[1] = {'0'};
	char C_monthUnits[1] = {'0'};
	char C_monthTens[1] = {'0'};
	char C_yearUnits[1] = {'0'};
	char C_yearTens[1] = {'0'};
	
	float tmpBatLevel = 0;
	uint32_t tmpBatLevelInt = 0;
	uint32_t tmpBatLevelInt_LSB = 0;
	uint32_t tmpBatLevelInt_MSB = 0;
	
	char tmp_bat_char_1[1] = {'0'};
	char tmp_bat_char_2[1] = {'0'};
	

	uint32_t flashingCnt = 0;
	uint32_t flashingCnt2 = 0;
	uint32_t flashingCnt3 = 0;
	uint32_t flashingCnt4 = 0;	
	uint32_t flashingCnt5 = 0;
	uint32_t flashingCnt6 = 0;
	uint32_t flashingCnt7 = 0;	
	uint32_t flashingCnt8 = 0;	
	
	uint32_t manuelIrrCnt = 0;
	
	
	float batteryVoltage = 9; //assume battery is full on reset
	uint8_t powerON = 1;
	uint32_t cntPowerOn4second = 3200;
	uint8_t fPowerOnRelay1OFF = 0;
	uint8_t fPowerOnRelay2OFF = 0;
	uint8_t fPowerOnRelay3OFF = 0;
	uint8_t fPowerOnRelay4OFF = 0;
	
	//screens
	uint8_t fhomePage = 0;
	uint8_t fwatchAdjustment = 0;
	uint8_t fselenoidProgramming = 0;
	
	//screen detais
	uint8_t fdayOfWeekAdjust = 0;
	uint8_t fminuteAdjust = 0;	
	uint8_t fhourAdjust = 0;	
	uint8_t fselectedSelenoidAdjust = 0;
	uint8_t fSelenoidWorkDurAdjust = 0;
	uint8_t fSelenoidStartTimeAdjust = 0;
	uint8_t fHergunSelected = 0;
	uint8_t fDelayedIrrigationSelected = 0;
	uint8_t fSpecialSelected = 0;
	uint8_t fDelayValueAdjust = 0;	
	uint8_t fSelStartTime_1_HourAdjust = 0;
	uint8_t fSelStartTime_1_MinuteAdjust = 0;
	uint8_t fSelStartTime_2_HourAdjust = 0;
	uint8_t fSelStartTime_2_MinuteAdjust = 0;
	uint8_t fSelStartTime_3_HourAdjust = 0;				//SONRA EKLE
	uint8_t fSelStartTime_3_MinuteAdjust = 0;				//SONRA EKLE
	uint8_t fSelStartTime_4_HourAdjust = 0;				//SONRA EKLE      
	uint8_t fSelStartTime_4_MinuteAdjust = 0;				//SONRA EKLE	
	uint8_t fdayAdjust = 0;	
	uint8_t fmonthAdjust = 0;		
	uint8_t fyearAdjust = 0;	
	uint8_t fSpecialDaySelect_PT = 0;	
	uint8_t fSpecialDaySelect_SA = 0;	
	uint8_t fSpecialDaySelect_CA = 0;	
	uint8_t fSpecialDaySelect_PE = 0;	
	uint8_t fSpecialDaySelect_CU = 0;	
	uint8_t fSpecialDaySelect_CT = 0;	
	uint8_t fSpecialDaySelect_PZ = 0;		
	uint8_t	fmanuelIrrigationAdjust = 0;
	uint8_t	fmanuelIrrigationPlanAdjust = 0;
	uint8_t	fPasswordAdjust = 0;
	uint8_t fManuelRelayAdjust = 0;
	uint8_t fActivateManuelMode = 0;
	uint8_t fManuelWorkDurationAdjust = 0;
	uint8_t fStartManuelWork = 0;
	uint8_t fStartAllRelayManuelWork = 0;
	uint8_t fRelay1Status = 0;
	uint8_t fRelay2Status = 0;
	uint8_t fRelay3Status = 0;
	uint8_t fRelay4Status = 0;
	uint8_t fResetLCDBuffer = 0;


	uint8_t fActivateManuelPlanMode = 0;
	uint8_t fPasswordActivateMode = 0;
	uint8_t	fRecordtoEEPROM = 0;
	uint8_t	fCopySelenoidSettings = 0;
	uint8_t	fcopyAndPasteSelSettings = 0;
	uint8_t	fPlanSelenoidSettings = 0;	
	uint8_t fReadfromEEPROM = 1;
	uint8_t fShowBatteryLevel = 1;
	uint8_t fsetNewPasswordMode = 0;
	uint8_t fPasswordDigit1Adjust = 0;
	uint8_t fPasswordDigit2Adjust = 0;	
	uint8_t fPasswordDigit3Adjust = 0;
	uint8_t fPasswordDigit4Adjust = 0;	
	uint8_t fUpdatePassword = 0;
	uint8_t fHomePageWithPassword = 0;
	uint8_t fDeviceLockedWithPassword = 0;	
	uint8_t fEraseAllPrograms = 0;
	uint8_t fshowCurrentDayOfWeek = 0;
	uint8_t fWakeUpButtonPressed = 0;
	uint8_t fHSIOscillatorInit = 1;
	uint8_t fSetNextAlarm = 0;
	uint8_t fWakeUpFromRTC = 0;
	uint8_t fSystemOFF = 0;
	uint8_t finitRtcAlarm = 0;
	uint8_t fUpdateInvPassCnt = 0;
	uint8_t fproducerPasswordNeeded = 0;
	uint8_t	fproducerPassword_1_Adj = 0;
	uint8_t	fproducerPassword_2_Adj = 0;
	uint8_t	fproducerPassword_3_Adj = 0;
	uint8_t	fproducerPassword_4_Adj = 0;
	uint8_t	fproducerPassword_5_Adj = 0;
	uint8_t fproducerPassword_6_Adj = 0;
	uint8_t fproducerPassword_7_Adj = 0;
	uint8_t fproducerPassword_8_Adj = 0;
	uint8_t fproducerPassword_9_Adj = 0;
	uint8_t fproducerPassword_10_Adj = 0;	
	uint8_t fAnyRelayON = 0;	
	uint8_t fBatteryReady = 0;
	uint8_t fAll_Her_Selected = 0;
	uint8_t fManuelAllRelayWorkDurAdj = 0;
	uint8_t fcelenderPlanDelayed = 0;
	uint8_t fResetLCDBufferwithDelay = 0;
	uint32_t resetLCDBufferDelayMs = 0;
	
	
	uint8_t fOpenRelay_1_WithDelay = 0;
	uint8_t fOpenRelay_2_WithDelay = 0;
	uint8_t fOpenRelay_3_WithDelay = 0;
	uint8_t fOpenRelay_4_WithDelay = 0;
	uint32_t relayOpenDelayMs = 0;



	uint8_t factoryReseyKEY[10] = {1,0,0,0,0,0,0,0,0,1};
	uint8_t factoryPASSWORD[10] = {0,0,0,0,0,0,0,0,0,0};
	
	//CELENDER VALUES						//Relat_1_Start_Date_Time_1, Relat_1_Stop_Date_Time_2,...Relat_4_Start_Date_Time_3, Relat_4_Stop_Date_Time_3
	uint32_t relayCelendar[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	uint32_t pendingRtcIrq[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		
	uint32_t pendingRelay[32] = {1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4};		
		
	//USER ADJUSTED VALUES
	uint8_t sel_work_duration[4] = {0,0,0,0};
	uint8_t sel_delayed_irr_val[4] = {0,0,0,0};   // ilk deger 0 olsun
	uint8_t	sel_manuel_work_duration[4] = {0,0,0,0};
	uint8_t sel_special_irr_days[4] = {0x00,0x00,0x00,0x00};   // ilk deger 0 olsun  Bits --> ACTIVATED, Pz,Ct,Cu,Pr,Ca,Sa,Pt -->
	uint8_t	sel_start_time_min[4][4]  = {0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0xFF};
	uint8_t	sel_start_time_hour[4][4] = {0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0xFF};  // 0xFF baslama  saati yok demek baslama saati olamaz
	uint8_t password[4] = {0,0,0,0};
	uint8_t tmpPassword[4] = {0,0,0,0};
	uint8_t fPasswordSetted = 0;
	uint8_t invalidPasswordCnt = 0;
	uint8_t all_manuel_work_duration = 0;					
	
	uint8_t selectedSelenoid = 1;

	uint32_t LCD_Ram_Data_Row[30] = {  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 
																		 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																		 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
																		 0x00,0x00,0x00,0x00,0x00,0x00};

/*******************************************************************************
 *  function :    LCD DRIVER INIT
 ******************************************************************************/
void nxpInit(void)
{

	GpioInit_t tGpioInit = {GPIO_MODE_OUTPUT,
                            GPIO_OUTPUT_PUSH_PULL,
                            GPIO_SPEED_HIGH,
                            GPIO_PULL_NON
                           };
    /* Enable the peripheral clock of GPIOB */
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Select output mode on GPIOB pin 14 - LCD POWER CONTROL */  
  gpio_init(GPIOB, 14, &tGpioInit);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");													 
	gpio_set(GPIOB, 14);	// LCD POWER ON //
	asm("nop");
	asm("nop");
	for(int i = 0; i<10000; i++)
	{
		asm("nop");
		asm("nop");	 
		asm("nop");
		asm("nop");
		asm("nop");	 
		asm("nop");
		asm("nop");
		asm("nop");	 
		asm("nop");		
		asm("nop");// after Power ON LCD, wait for stabilation ( miniumum 1ms )
		asm("nop");
		asm("nop");	
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");	 
		asm("nop");	
		asm("nop");
		asm("nop");	 
		asm("nop");		
	}	
	uint32_t command = 0xc8;  //mode set  -- ENABLE, 1/3 Bias, 1/4 mux
	I2C_Write(0x80,command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");													 
	uint32_t command1 = 0x00;  //load data pointer to 0
	I2C_Write(0x80,command1);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");	
}

/*******************************************************************************
 *  function :   LCD POWER OFF
 ******************************************************************************/
void LCDPowerOFF(void)
{
		gpio_clear(GPIOB, 14);	// LCD POWER OFF //
}


/*******************************************************************************
 *  function :   LCD POWER ON
 ******************************************************************************/
void LCDPowerON(void)
{
		gpio_set(GPIOB, 14);	// LCD POWER ON //
}



/*******************************************************************************
 *  function :  UPDATE LCD BUFFER
 ******************************************************************************/

void updateLCDBuffer(void)
{
	/*************************************
	......
	....
	..
	.
	FIRSTLY RESET ALL BUFFER
	.
	..
	....
	......
	**************************************/		
	
	//LCDBufferReset();
	
	/*************************************
	......
	....
	..
	.
	POWER ON SCREEN
	.
	..
	....
	......
	**************************************/		
	if(powerON)
	{
		//
		LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | 0x80; // supiri logo on;
		//
		//
		updateBatLevelDigit(); // show battery level;
		//	
		// Update flashings	"KAPALI" on powerON
		//		
		++flashingCnt;
		if(flashingCnt < 30)
		{
				LCD_Ram_Data_Row[10] = sevenSegCode('I');
				LCD_Ram_Data_Row[11] = sevenSegCode('L');
				LCD_Ram_Data_Row[12] = sevenSegCode('A');		// PowerOn durumda röleler sirasiyla kapatilir ve ekranda kapali yanip söner.
				LCD_Ram_Data_Row[13] = sevenSegCode('P');
				LCD_Ram_Data_Row[14] = sevenSegCode('A');
				LCD_Ram_Data_Row[15] = sevenSegCode('H');	
			
				;
				LCD_Ram_Data_Row[7] = sevenSegCode('0');
				LCD_Ram_Data_Row[6] = sevenSegCode('F');   // OFF
				LCD_Ram_Data_Row[5] = sevenSegCode('F');	
		}
		else if (flashingCnt == 40)
		{
			flashingCnt = 0;
		}
		else
		{
				LCD_Ram_Data_Row[10] = 0x00;
				LCD_Ram_Data_Row[11] = 0x00;
				LCD_Ram_Data_Row[12] = 0x00;			
				LCD_Ram_Data_Row[13] = 0x00;
				LCD_Ram_Data_Row[14] = 0x00;
				LCD_Ram_Data_Row[15] = 0x00;
			
				LCD_Ram_Data_Row[7] = 0x00;
				LCD_Ram_Data_Row[6] = 0x00;  // OFF
				LCD_Ram_Data_Row[5] = 0x00;
		}
		//
		//
		LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0x20;
		LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] | 0x01;
		LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] | 0x01; 	// fiskiye on
		LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] | 0x01;
		LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] | 0x01;
		LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0xEC;		
		//
		if(fPowerOnRelay1OFF)
		{
			LCD_Ram_Data_Row[20] = 0b00000110;   // XXXXabcd   //RELAY_1_LCD_ON     1--> 00000110 
			LCD_Ram_Data_Row[21] = 0b00000000;		// fgeXXXXX												 1--> 00000000
		}
		else if(fPowerOnRelay2OFF)
		{
			LCD_Ram_Data_Row[20] = 0b00001101;   // XXXXabcd   //RELAY_2_LCD_ON     1--> 00001101 
			LCD_Ram_Data_Row[21] = 0b01100000;		// fgeXXXXX												 1--> 01100000
		}
		else if(fPowerOnRelay3OFF)
		{
			LCD_Ram_Data_Row[20] = 0b00001111;   // XXXXabcd   //RELAY_3_LCD_ON     1--> 00001111 
			LCD_Ram_Data_Row[21] = 0b01000000;		// fgeXXXXX												 1--> 01000000
		}
		else if(fPowerOnRelay4OFF)
		{
			LCD_Ram_Data_Row[20] = 0b00000110;   // XXXXabcd   //RELAY_4_LCD_ON     1--> 00000110 
			LCD_Ram_Data_Row[21] = 0b11000000;		// fgeXXXXX												 1--> 11000000
		}
		else
		{
			LCD_Ram_Data_Row[20] = 0b00000000;  // OFF
			LCD_Ram_Data_Row[21] = 0b00000000;	
		}
	}
	
	else // AFTER POWER ON
	{
	/*************************************
	......
	....
	..
	.
	SYSTEM OFF SCREEN
	.
	..
	....
	......
	**************************************/				
		if(fSystemOFF)
		{
			
			LCD_Ram_Data_Row[10] = sevenSegCode('I');
			LCD_Ram_Data_Row[11] = sevenSegCode('L');
			LCD_Ram_Data_Row[12] = sevenSegCode('A');		// PowerOn durumda röleler sirasiyla kapatilir ve ekranda kapali yanip söner.
			LCD_Ram_Data_Row[13] = sevenSegCode('P');
			LCD_Ram_Data_Row[14] = sevenSegCode('A');
			LCD_Ram_Data_Row[15] = sevenSegCode('H');	
		
			;
			LCD_Ram_Data_Row[7] = sevenSegCode('0');
			LCD_Ram_Data_Row[6] = sevenSegCode('F');   // OFF
			LCD_Ram_Data_Row[5] = sevenSegCode('F');	
			
			++flashingCnt;
			if(flashingCnt >= 30)
			{
				LCD_Ram_Data_Row[10] = 0x00;
				LCD_Ram_Data_Row[11] = 0x00;
				LCD_Ram_Data_Row[12] = 0x00;			
				LCD_Ram_Data_Row[13] = 0x00;
				LCD_Ram_Data_Row[14] = 0x00;
				LCD_Ram_Data_Row[15] = 0x00;
			
				LCD_Ram_Data_Row[7] = 0x00;
				LCD_Ram_Data_Row[6] = 0x00;  // OFF
				LCD_Ram_Data_Row[5] = 0x00;
			}
			if (flashingCnt == 50)
			{
				flashingCnt = 0;
			}
		}
		
		/*************************************
		......
		....
		..
		.
		PRODUCER PASSWORD SCREEN
		.
		..
		....
		......
	**************************************/	
		else if(fproducerPasswordNeeded)
		{
			LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | 0x80; // supiri logo on;
			//
			//
			updateBatLevelDigit(); // show battery level;
			
			// HOUR UPDATE 2 -- PASSWORD				
			LCD_Ram_Data_Row[1] = (sevenSegCode('5')<<4)|(sevenSegCode('5')>>4);//'S' //fgeXabcd
			LCD_Ram_Data_Row[2] = (sevenSegCode('F')<<4)|(sevenSegCode('F')>>4);//'F' //fgeXabcd
			LCD_Ram_Data_Row[3] = (sevenSegCode('R')<<4)|(sevenSegCode('r')>>4);//'R' //fgeXabcd
			LCD_Ram_Data_Row[4] = (sevenSegCode('P')<<4)|(sevenSegCode('P')>>4);//'C' //fgeXabcd
			
			if((factoryPASSWORD[0] == 0)&(factoryPASSWORD[1] == 0)&(factoryPASSWORD[2] == 0)&(factoryPASSWORD[3] == 0)&(factoryPASSWORD[4] == 0)&(factoryPASSWORD[5] == 0)&(factoryPASSWORD[6] == 0)&(factoryPASSWORD[7] == 0)&(factoryPASSWORD[8] == 0)&(factoryPASSWORD[9] == 0))
			{
				LCD_Ram_Data_Row[10] = LCD_Ram_Data_Row[10] | sevenSegCode('_');
				LCD_Ram_Data_Row[11] = LCD_Ram_Data_Row[11] | sevenSegCode('_');
				LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] | sevenSegCode('_');	//
				LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] | sevenSegCode('_');
				LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] | sevenSegCode('_');
				LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] | sevenSegCode('_');	
				LCD_Ram_Data_Row[8] = LCD_Ram_Data_Row[8] | sevenSegCode('_'); 
				LCD_Ram_Data_Row[7] = LCD_Ram_Data_Row[7] | sevenSegCode('_');	
				LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] | sevenSegCode('_');	
				LCD_Ram_Data_Row[5] = LCD_Ram_Data_Row[5] | sevenSegCode('_');
			}
			
			else
			{
				char factoryPass_1[1] = {'0'};
				char factoryPass_2[1] = {'0'};
				char factoryPass_3[1] = {'0'};
				char factoryPass_4[1] = {'0'};
				char factoryPass_5[1] = {'0'};
				char factoryPass_6[1] = {'0'};
				char factoryPass_7[1] = {'0'};
				char factoryPass_8[1] = {'0'};
				char factoryPass_9[1] = {'0'};
				char factoryPass_10[1] = {'0'};

				
				sprintf(factoryPass_1, "%d", factoryPASSWORD[0]);
				sprintf(factoryPass_2, "%d", factoryPASSWORD[1]);
				sprintf(factoryPass_3, "%d", factoryPASSWORD[2]);
				sprintf(factoryPass_4, "%d", factoryPASSWORD[3]);
				sprintf(factoryPass_5, "%d", factoryPASSWORD[4]);
				sprintf(factoryPass_6, "%d", factoryPASSWORD[5]);
				sprintf(factoryPass_7, "%d", factoryPASSWORD[6]);
				sprintf(factoryPass_8, "%d", factoryPASSWORD[7]);
				sprintf(factoryPass_9, "%d", factoryPASSWORD[8]);
				sprintf(factoryPass_10, "%d", factoryPASSWORD[9]);
				
				LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] | sevenSegCode(factoryPass_1[0]);
				LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] | sevenSegCode(factoryPass_2[0]);
				LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] | sevenSegCode(factoryPass_3[0]);
				LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] | sevenSegCode(factoryPass_4[0]);
				LCD_Ram_Data_Row[11] = LCD_Ram_Data_Row[11] | sevenSegCode(factoryPass_5[0]);
				LCD_Ram_Data_Row[10] = LCD_Ram_Data_Row[10] | sevenSegCode(factoryPass_6[0]);
				LCD_Ram_Data_Row[8] = LCD_Ram_Data_Row[8] | sevenSegCode(factoryPass_7[0]); 
				LCD_Ram_Data_Row[7] = LCD_Ram_Data_Row[7] | sevenSegCode(factoryPass_8[0]);	
				LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] | sevenSegCode(factoryPass_9[0]);	
				LCD_Ram_Data_Row[5] = LCD_Ram_Data_Row[5] | sevenSegCode(factoryPass_10[0]);		
			}
			
			if(fproducerPassword_1_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			if(fproducerPassword_2_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			if(fproducerPassword_3_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}					
			if(fproducerPassword_4_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			if(fproducerPassword_5_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[11] = LCD_Ram_Data_Row[11] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			if(fproducerPassword_6_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[10] = LCD_Ram_Data_Row[10] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			if(fproducerPassword_7_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[8] = LCD_Ram_Data_Row[8] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			if(fproducerPassword_8_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[7] = LCD_Ram_Data_Row[7] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			if(fproducerPassword_9_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			if(fproducerPassword_10_Adj)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[5] = LCD_Ram_Data_Row[5] & 0b00000001;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}						
		}			
	/*************************************
		......
		....
		..
		.
		HOME SCREEN WITH PASSWORD
		.
		..
		....
		......
	**************************************/

		else if(fHomePageWithPassword)
		{
			LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | 0x80; // supiri logo on;
			//
			//
			updateBatLevelDigit(); // show battery level;
			//
			//
			LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9]  | 0x40;  // kilit açik;	
			//	
			//
			// DATE UPDATE
			currentDate= RTC->DR;
			dateUnits = currentDate & 0x0000000F;
			dateTens = ((currentDate>>4) & 0x00000003);
			monthUnits = ((currentDate>>8) & 0x000000F);
			monthTens = ((currentDate>>12) & 0x00000001);
			yearUnits = ((currentDate>>16) & 0x0000000F);
			yearTens = ((currentDate>>20) & 0x0000000F);
		
			sprintf(C_dateUnits, "%d", dateUnits);
			sprintf(C_dateTens, "%d", dateTens);   // sralamasi degisince çalismiyor ?????
			sprintf(C_monthUnits, "%d", monthUnits);
			sprintf(C_monthTens, "%d", monthTens);
			sprintf(C_yearUnits, "%d", yearUnits);
			sprintf(C_yearTens, "%d", yearTens);
			
			LCD_Ram_Data_Row[11] = LCD_Ram_Data_Row[11] |  sevenSegCode(C_yearTens[0]);
			LCD_Ram_Data_Row[10] = LCD_Ram_Data_Row[10] |  sevenSegCode(C_yearUnits[0]);
			LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] |  sevenSegCode(C_monthTens[0]);
			LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] | sevenSegCode(C_monthUnits[0]);	
			LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] |  sevenSegCode(C_dateTens[0]);
			LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] |  sevenSegCode(C_dateUnits[0]);

			// HOUR UPDATE
			currentTime = RTC->TR;
			secondUnits = currentTime & 0x0000000F;
			secondTens = ((currentTime>>4) & 0x00000007);
			minuteUnits = ((currentTime>>8) & 0x000000F);
			minuteTens = ((currentTime>>12) & 0x00000007);
			hourUnits = ((currentTime>>16) & 0x0000000F);
			hourTens = ((currentTime>>20) & 0x00000003);
		
			//sprintf(C_secondUnits, "%d", secondUnits);
			//sprintf(C_secondTens, "%d", secondTens);   // sralamasi degisince çalismiyor ?????
			sprintf(C_minuteUnits, "%d", minuteUnits);
			sprintf(C_minuteTens, "%d", minuteTens);
			sprintf(C_hourUnits, "%d", hourUnits);
			sprintf(C_hourTens, "%d", hourTens);
			
			LCD_Ram_Data_Row[8] = sevenSegCode(C_hourTens[0]);
			LCD_Ram_Data_Row[7] = sevenSegCode(C_hourUnits[0]);
			LCD_Ram_Data_Row[6] = sevenSegCode(C_minuteTens[0]);
			LCD_Ram_Data_Row[5] = sevenSegCode(C_minuteUnits[0]);		
			LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] | 0x01;   // saat arasindaki ":"
			//
			//
			//HOUR 2 UPDATE --SIFRE
			//
			//
			if((tmpPassword[0] == 0) & (tmpPassword[1] == 0) & (tmpPassword[2] == 0) & (tmpPassword[3] == 0)) 
			{
				// HOUR UPDATE 2 -- PASSWORD	
				LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] & 0b00010000;   // abcdfgeX
				LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] & 0b00000000;   // abcdfgeX
				LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] & 0b00010000;   // abcdfgeX
				LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] & 0b00000000;   // abcdfgeX
				
				LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] |  0b00000001;
				LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] |  0b00000001;
				LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] |  0b00000001;
				LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] |  0b00000001;	
			}
			else
			{
				char tmp_char_1[1];
				char tmp_char_2[1];
				char tmp_char_3[1];
				char tmp_char_4[1];
				
				sprintf(tmp_char_1, "%d", (tmpPassword[0]));
				sprintf(tmp_char_2, "%d", (tmpPassword[1]));
				sprintf(tmp_char_3, "%d", (tmpPassword[2]));
				sprintf(tmp_char_4, "%d", (tmpPassword[3]));
				
				// HOUR UPDATE 2 -- PASSWORD				
				LCD_Ram_Data_Row[4] = (sevenSegCode(tmp_char_4[0])<<4)|(sevenSegCode(tmp_char_4[0])>>4);
				LCD_Ram_Data_Row[3] = (sevenSegCode(tmp_char_3[0])<<4)|(sevenSegCode(tmp_char_3[0])>>4);
				LCD_Ram_Data_Row[2] = (sevenSegCode(tmp_char_2[0])<<4)|(sevenSegCode(tmp_char_2[0])>>4);
				LCD_Ram_Data_Row[1] = (sevenSegCode(tmp_char_1[0])<<4)|(sevenSegCode(tmp_char_1[0])>>4);
			}
			if(fPasswordDigit1Adjust)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] & 0b00010000;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			else if(fPasswordDigit2Adjust)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] & 0b00000000;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}			
			}
			else if(fPasswordDigit3Adjust)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] & 0b00010000;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}	
			else if(fPasswordDigit4Adjust)  // password 1 ayarlaniyor
			{
				++flashingCnt;
				if(flashingCnt > 20)
				{
					LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] & 0b00000000;   // abcdfgeX
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;	
				}				
			}		
		}
	/*************************************
		......
		....
		..
		.
		HOME SCREEN
		.
		..
		....
		......
	**************************************/		
		else if(fhomePage)
		{
			LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | 0x80; // supiri logo on;
			//
			//
			updateBatLevelDigit(); // show battery level;
			//					
			if(fShowBatteryLevel)
			{		
				tmpBatLevel = batteryVoltage-6;
				tmpBatLevel = tmpBatLevel/3;
				tmpBatLevel = tmpBatLevel * 100;
				tmpBatLevelInt = ceil(tmpBatLevel);
				tmpBatLevelInt = bcd2cnv(tmpBatLevelInt);
				tmpBatLevelInt_LSB = tmpBatLevelInt&0x0000000F;
				tmpBatLevelInt_MSB = (tmpBatLevelInt&0x000000F0)>>4;
				sprintf(tmp_bat_char_1, "%d", tmpBatLevelInt_LSB);
				sprintf(tmp_bat_char_2, "%d", tmpBatLevelInt_MSB);	
				
				LCD_Ram_Data_Row[11] = sevenSegCode(tmp_bat_char_2[0]);
				LCD_Ram_Data_Row[10] = sevenSegCode(tmp_bat_char_1[0]);
				
			}
			else
			{
				// DATE UPDATE
				currentDate= RTC->DR;
				dateUnits = currentDate & 0x0000000F;
				dateTens = ((currentDate>>4) & 0x00000003);
				monthUnits = ((currentDate>>8) & 0x000000F);
				monthTens = ((currentDate>>12) & 0x00000001);
				yearUnits = ((currentDate>>16) & 0x0000000F);
				yearTens = ((currentDate>>20) & 0x0000000F);
			
				sprintf(C_dateUnits, "%d", dateUnits);
				sprintf(C_dateTens, "%d", dateTens);   // sralamasi degisince çalismiyor ?????
				sprintf(C_monthUnits, "%d", monthUnits);
				sprintf(C_monthTens, "%d", monthTens);
				sprintf(C_yearUnits, "%d", yearUnits);
				sprintf(C_yearTens, "%d", yearTens);
				
				LCD_Ram_Data_Row[11] = LCD_Ram_Data_Row[11] |  sevenSegCode(C_yearTens[0]);
				LCD_Ram_Data_Row[10] = LCD_Ram_Data_Row[10] |  sevenSegCode(C_yearUnits[0]);
				LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] |  sevenSegCode(C_monthTens[0]);
				LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] | sevenSegCode(C_monthUnits[0]);	
				LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] |  sevenSegCode(C_dateTens[0]);
				LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] |  sevenSegCode(C_dateUnits[0]);

				// HOUR UPDATE
				currentTime = RTC->TR;
				secondUnits = currentTime & 0x0000000F;
				secondTens = ((currentTime>>4) & 0x00000007);
				minuteUnits = ((currentTime>>8) & 0x000000F);
				minuteTens = ((currentTime>>12) & 0x00000007);
				hourUnits = ((currentTime>>16) & 0x0000000F);
				hourTens = ((currentTime>>20) & 0x00000003);
			
				//sprintf(C_secondUnits, "%d", secondUnits);
				//sprintf(C_secondTens, "%d", secondTens);   // sralamasi degisince çalismiyor ?????
				sprintf(C_minuteUnits, "%d", minuteUnits);
				sprintf(C_minuteTens, "%d", minuteTens);
				sprintf(C_hourUnits, "%d", hourUnits);
				sprintf(C_hourTens, "%d", hourTens);
				
				LCD_Ram_Data_Row[8] = sevenSegCode(C_hourTens[0]);
				LCD_Ram_Data_Row[7] = sevenSegCode(C_hourUnits[0]);
				LCD_Ram_Data_Row[6] = sevenSegCode(C_minuteTens[0]);
				LCD_Ram_Data_Row[5] = sevenSegCode(C_minuteUnits[0]);		
				LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] | 0x01;   // saat arasindaki ":"
				
				// HOUR 2 UPDATE  - used for day of week - ANASAYFADA ILERI VE GERI TUSLARINA BASILDIGINDA GOSTERIR
				if(fshowCurrentDayOfWeek)
				{
					if(currentDayOfWeek == 1)
					{
						LCD_Ram_Data_Row[3] = 0xEC; // "P"     // Pazartesi PT
						LCD_Ram_Data_Row[4] = 0xE1; // "T"
					}
					else if(currentDayOfWeek == 2)
					{
						LCD_Ram_Data_Row[3] = 0xCB; // "S"     // Sali SA
						LCD_Ram_Data_Row[4] = 0xEE; // "A"
					}
					else if(currentDayOfWeek == 3)
					{
						LCD_Ram_Data_Row[3] = 0xA9; // "C"     //Çarsamba CA
						LCD_Ram_Data_Row[4] = 0xEE; // "A"
					}
					else if(currentDayOfWeek == 4)
					{
						LCD_Ram_Data_Row[3] = 0xEC; // "P"     //Persembe PE
						LCD_Ram_Data_Row[4] = 0xE9; // "E"
					}
					else if(currentDayOfWeek == 5)
					{
						LCD_Ram_Data_Row[3] = 0xA9; // "C"     //Cuma CU
						LCD_Ram_Data_Row[4] = 0xA7; // "U"
					}
					else if(currentDayOfWeek == 6)
					{
						LCD_Ram_Data_Row[3] = 0xA9; // "C"     //Cumartesi CT
						LCD_Ram_Data_Row[4] = 0xE1; // "T"
					}
					else if(currentDayOfWeek == 7)
					{
						LCD_Ram_Data_Row[3] = 0xEC; // "P"     //Pazar PZ
						LCD_Ram_Data_Row[4] = 0x6D; // "Z"
					}
				}		
			}
		}
			
	/*************************************
		......
		....
		..
		.
		WATCH ADJUSTMENT SCREEN
		.
		..
		....
		......
	**************************************/		
		else if(fwatchAdjustment)
		{
			//
			LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | 0x80; // supiri logo on;
			//
			//
			updateBatLevelDigit(); // show battery level;
			//

			++flashingCnt;
			if(flashingCnt < 30)
			{			
				// DATE UPDATE
				currentDate= RTC->DR;
				dateUnits = currentDate & 0x0000000F;
				dateTens = ((currentDate>>4) & 0x00000003);
				monthUnits = ((currentDate>>8) & 0x000000F);
				monthTens = ((currentDate>>12) & 0x00000001);
				yearUnits = ((currentDate>>16) & 0x0000000F);
				yearTens = ((currentDate>>20) & 0x0000000F);
				currentDayOfWeek = ((currentDate>>13) & 0x00000007);
			
				sprintf(C_dateUnits, "%d", dateUnits);
				sprintf(C_dateTens, "%d", dateTens);   // sralamasi degisince çalismiyor ?????
				sprintf(C_monthUnits, "%d", monthUnits);
				sprintf(C_monthTens, "%d", monthTens);
				sprintf(C_yearUnits, "%d", yearUnits);
				sprintf(C_yearTens, "%d", yearTens);
				
				LCD_Ram_Data_Row[11] = LCD_Ram_Data_Row[11] | sevenSegCode(C_yearTens[0]);
				LCD_Ram_Data_Row[10] = LCD_Ram_Data_Row[10] | sevenSegCode(C_yearUnits[0]);
				LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] | sevenSegCode(C_monthTens[0]);
				LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] | sevenSegCode(C_monthUnits[0]);	
				LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] | sevenSegCode(C_dateTens[0]);
				LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] | sevenSegCode(C_dateUnits[0]);				
				
				// HOUR UPDATE
				currentTime = RTC->TR;
				secondUnits = currentTime & 0x0000000F;
				secondTens = ((currentTime>>4) & 0x00000007);
				minuteUnits = ((currentTime>>8) & 0x000000F);
				minuteTens = ((currentTime>>12) & 0x00000007);
				hourUnits = ((currentTime>>16) & 0x0000000F);
				hourTens = ((currentTime>>20) & 0x00000003);
			
				//sprintf(C_secondUnits, "%d", secondUnits);
				//sprintf(C_secondTens, "%d", secondTens);   // sralamasi degisince çalismiyor ?????
				sprintf(C_minuteUnits, "%d", minuteUnits);
				sprintf(C_minuteTens, "%d", minuteTens);
				sprintf(C_hourUnits, "%d", hourUnits);
				sprintf(C_hourTens, "%d", hourTens);
				
				LCD_Ram_Data_Row[8] = sevenSegCode(C_hourTens[0]);
				LCD_Ram_Data_Row[7] = sevenSegCode(C_hourUnits[0]);
				LCD_Ram_Data_Row[6] = sevenSegCode(C_minuteTens[0]);
				LCD_Ram_Data_Row[5] = sevenSegCode(C_minuteUnits[0]);		
				LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] | 0x01;   // saat arasindaki ":"
						
			}
			else if (flashingCnt == 50)
			{
				flashingCnt = 0;
			}
			else
			{
				if(fyearAdjust)
				{
					LCD_Ram_Data_Row[11] = LCD_Ram_Data_Row[11] & 0x01;
					LCD_Ram_Data_Row[10] = 0x00;
				}
				else if(fmonthAdjust)
				{
					LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] & 0x01;
					LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] & 0x01;
				}	
				else if(fdayAdjust)
				{
					LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] & 0x01;
					LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] & 0x01;
				}
				else if(fhourAdjust)
				{
					LCD_Ram_Data_Row[8] = 0x00;
					LCD_Ram_Data_Row[7] = 0x00;
				}
				else if(fminuteAdjust)
				{
					LCD_Ram_Data_Row[6] = 0x01;  // ":" toggle yapmasin
					LCD_Ram_Data_Row[5] = 0x00;
				}
				else
				{
					LCD_Ram_Data_Row[10] = 0x00;
					LCD_Ram_Data_Row[11] = LCD_Ram_Data_Row[11] & 0x01;
					LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] & 0x01;
					LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] & 0x01;
					LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] & 0x01;
					LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] & 0x01;
					LCD_Ram_Data_Row[8] = 0x00;
					LCD_Ram_Data_Row[7] = 0x00;
					LCD_Ram_Data_Row[6] = 0x00;
					LCD_Ram_Data_Row[5] = 0x00;
				}					
			}		
		}
		
	/*************************************
		......
		....
		..
		.
		SELENOID PROGRAMMING SCREEN
		.
		..
		....
		......
	**************************************/		
		else if(fselenoidProgramming)
		{
			//
			LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | 0x80; // supiri logo on;
			//
			//
			updateBatLevelDigit(); // show battery level;
			//
			//
			if(fPlanSelenoidSettings)
			{
					// HOUR UPDATE 1				
					LCD_Ram_Data_Row[8] = sevenSegCode('P');
					LCD_Ram_Data_Row[7] = sevenSegCode('L');   // PLAN
					LCD_Ram_Data_Row[6] = sevenSegCode('A');
					LCD_Ram_Data_Row[5] = sevenSegCode('N');								
			}
			else
			{
				
				if(fCopySelenoidSettings)
				{
					++flashingCnt;
					if((flashingCnt == 50)|(flashingCnt == 100)|(flashingCnt == 150))
					{
						if(selectedSelenoid == 4)
						{
							fCopySelenoidSettings = 0;
							flashingCnt = 0;
							
						}
						else
						{
							LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] & 0b11110000;   // XXXXabcd
							LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] & 0b00011111;		// fgeXXXXX		
							++selectedSelenoid;
						}
					}
				}
				//***********************
				// SELENOID NUMBER UPDATE
				//***********************
				if(selectedSelenoid == 1)
				{
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00000110;   // XXXXabcd   // 1
					LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b00000000;		// fgeXXXXX		
				}
				else if(selectedSelenoid == 2)
				{
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00001101;   // XXXXabcd   // 2
					LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b01100000;		// fgeXXXXX		
				}			
				else if(selectedSelenoid == 3)
				{
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00001111;   // XXXXabcd   // 3
					LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b01000000;		// fgeXXXXX			
				}			
				else if(selectedSelenoid == 4)
				{
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00000110;   // XXXXabcd   // 4
					LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b11000000;		// fgeXXXXX		
				}			

				if(fselectedSelenoidAdjust)
				{
					++flashingCnt;
					if(flashingCnt >= 30)
					{
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] & 0b11110000;   // XXXXabcd
						LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] & 0b00011111;		// fgeXXXXX		
					}
					if (flashingCnt == 50)
					{
						flashingCnt = 0;
					}				
				}		
				//******************************
				// SELENOID WORK DURATION UPDATE
				//******************************		
				if(fSelenoidWorkDurAdjust)
				{
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0x30; // sand watch icon on;
					++flashingCnt;
					if(flashingCnt == 10)
					{
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0x40; // sand watch full on;
					}
					else if(flashingCnt == 20)
					{
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt == 30)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x08; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt == 40)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0C; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt == 50)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0E; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt == 60)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0F; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt == 70)
					{
						flashingCnt = 0;
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] &  0xF0; // sand watch animation full off;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] &   0x3F; // sand watch animation full off;
					}					
				}			
				if((fSelenoidWorkDurAdjust) | ((fSelStartTime_1_HourAdjust)|(fSelStartTime_1_MinuteAdjust))
																		|((fSelStartTime_2_HourAdjust) |(fSelStartTime_2_MinuteAdjust)) 
																		| ((fSelStartTime_3_HourAdjust)|(fSelStartTime_3_MinuteAdjust)) 
																		| ((fSelStartTime_4_HourAdjust)|(fSelStartTime_4_MinuteAdjust)) 
																		|(fHergunSelected)|(fDelayedIrrigationSelected)|(fSpecialSelected))   
				{
					uint32_t tmp_work_duration = 0;
					char tmpDurTens[1];
					char tmpDurUnits[1];
					
					tmp_work_duration = sel_work_duration[selectedSelenoid-1];
					
					if(tmp_work_duration > 99)
					{
						LCD_Ram_Data_Row[10] = sevenSegCode('2');  //pilin yaninda 2 yazisi çiksin 2. 99 daki ayarlandigi belli olur
//						LCD_Ram_Data_Row[11] = sevenSegCode('0');
//						LCD_Ram_Data_Row[12] = sevenSegCode('0');		//
//						LCD_Ram_Data_Row[13] = sevenSegCode('1');
//						LCD_Ram_Data_Row[14] = sevenSegCode('X');
//						LCD_Ram_Data_Row[15] = sevenSegCode('+');	
						tmp_work_duration = tmp_work_duration - 100;
					}
					
					tmp_work_duration = bcd2cnv(tmp_work_duration);
					
					sprintf(tmpDurTens, "%d", (tmp_work_duration>>4)&0x0F);
					sprintf(tmpDurUnits, "%d", (tmp_work_duration)&0x0F);
						
					// DURATION MIN UNITES UPDATE
					LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] |  ((sevenSegCode(tmpDurTens[0])>>4) & 0x0F);    // XXXXabcd
					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  ((sevenSegCode(tmpDurTens[0])<<4) & 0xE0);	 // fgeXXXXX	
					// DURATION MIN TENS UPDATE
					LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | ((sevenSegCode(tmpDurUnits[0])>>4) & 0x0F);   // XXXXabcd 
					LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] | ((sevenSegCode(tmpDurUnits[0])<<4) & 0xE0);		// fgeXXXXX
					
					if(fSelenoidWorkDurAdjust)
					{
						++flashingCnt2;
						if(flashingCnt2 >= 30)
						{
							LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] & 0b11110000;   // XXXXabcd
							LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] & 0b00010000;		// fgeXXXXX	
							LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] & 0b00011111;		// fgeXXXXX							
						}
						if (flashingCnt2 == 50)
						{
							flashingCnt2 = 0;
						}
					}					
				}
				//**********************
				// STARTING HOURS UPDATE .1 .2
				//**********************				
				if((fSelStartTime_1_HourAdjust)|(fSelStartTime_1_MinuteAdjust)|(fSelStartTime_2_HourAdjust)|(fSelStartTime_2_MinuteAdjust)|(fHergunSelected)|(fDelayedIrrigationSelected)|(fSpecialSelected))
				{
					char tmp_char_1[1];
					char tmp_char_2[1];
					char tmp_char_3[1];
					char tmp_char_4[1];
					
					uint8_t tmp_start_hour = 0;
					uint8_t tmp_start_min = 0;
					
					tmp_start_hour = bcd2cnv(sel_start_time_hour[selectedSelenoid-1][0]);
					tmp_start_min = bcd2cnv(sel_start_time_min[selectedSelenoid-1][0]);

					sprintf(tmp_char_1, "%d", (tmp_start_hour>>4)&0x0F);
					sprintf(tmp_char_2, "%d", (tmp_start_hour)&0x0F);
					sprintf(tmp_char_3, "%d", (tmp_start_min>>4)&0x0F);
					sprintf(tmp_char_4, "%d", (tmp_start_min)&0x0F);
					
					// HOUR UPDATE 1				
					LCD_Ram_Data_Row[5] = sevenSegCode(tmp_char_4[0]);
					LCD_Ram_Data_Row[6] = sevenSegCode(tmp_char_3[0]);
					LCD_Ram_Data_Row[7] = sevenSegCode(tmp_char_2[0]);
					LCD_Ram_Data_Row[8] = sevenSegCode(tmp_char_1[0]);								
					LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] | 0x01;   // saat arasindaki ":"	
					
					if((sel_start_time_min[selectedSelenoid-1][1] == 0xFF)&(sel_start_time_hour[selectedSelenoid-1][1] == 0xFF))
					{				
						// HOUR UPDATE 2
						LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] |  0b00000001;
						LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] |  0b00000001;
						LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] |  0b00000001;
						LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] |  0b00000001;	
						LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] |  0x10;   // saat arasindaki ":"	
					}			
					else
					{
						tmp_start_hour = bcd2cnv(sel_start_time_hour[selectedSelenoid-1][1]);
						tmp_start_min = bcd2cnv(sel_start_time_min[selectedSelenoid-1][1]);

						sprintf(tmp_char_1, "%d", (tmp_start_hour>>4)&0x0F);
						sprintf(tmp_char_2, "%d", (tmp_start_hour)&0x0F);
						sprintf(tmp_char_3, "%d", (tmp_start_min>>4)&0x0F);
						sprintf(tmp_char_4, "%d", (tmp_start_min)&0x0F);
						
						// HOUR UPDATE 2				
						LCD_Ram_Data_Row[4] = (sevenSegCode(tmp_char_4[0])<<4)|(sevenSegCode(tmp_char_4[0])>>4);
						LCD_Ram_Data_Row[3] = (sevenSegCode(tmp_char_3[0])<<4)|(sevenSegCode(tmp_char_3[0])>>4);
						LCD_Ram_Data_Row[2] = (sevenSegCode(tmp_char_2[0])<<4)|(sevenSegCode(tmp_char_2[0])>>4);
						LCD_Ram_Data_Row[1] = (sevenSegCode(tmp_char_1[0])<<4)|(sevenSegCode(tmp_char_1[0])>>4);								
						LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] | 0x10;   // saat arasindaki ":"	
					}
				
					// HOUR SELECT DIGITS 1. 2. 3. 4.
					LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] | 0x10;   // saatten önceki 1. 2. ":"
					LCD_Ram_Data_Row[8] = LCD_Ram_Data_Row[8] | 0x01;   // saatten önceki 1. 2. ":"
					//LCD_Ram_Data_Row[0] = LCD_Ram_Data_Row[0] | 0x0C;   // saatten önceki 3. 4. ":"
						
					//
					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0F; // sand watch full on;
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |  0xF0; // sand watch full on;
					//
					if(fSelStartTime_1_HourAdjust)   // ayar modunda flashlasin
					{
						++flashingCnt;
						if(flashingCnt >= 30)
						{
							LCD_Ram_Data_Row[7] = LCD_Ram_Data_Row[7] & 0b00000001;   // abcdfgeX
							LCD_Ram_Data_Row[8] = LCD_Ram_Data_Row[8] & 0b00000001;		// abcdfgeX							
						}
						if (flashingCnt == 50)
						{
							flashingCnt = 0;
						}
					}		
					else if(fSelStartTime_1_MinuteAdjust)
					{
						++flashingCnt;
						if(flashingCnt >= 30)
						{
							LCD_Ram_Data_Row[5] = LCD_Ram_Data_Row[5] & 0b00000001;   // abcdfgeX
							LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] & 0b00000001;		// abcdfgeX							
						}
						if (flashingCnt == 50)
						{
							flashingCnt = 0;
						}
					}		
					else if(fSelStartTime_2_HourAdjust)   // ayar modunda flashlasin
					{
						++flashingCnt;
						if(flashingCnt >= 30)
						{
							LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] & 0b00010000;   // abcdfgeX
							LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] & 0b00000000;		// abcdfgeX							
						}
						if (flashingCnt == 50)
						{
							flashingCnt = 0;
						}
					}		
					else if(fSelStartTime_2_MinuteAdjust)
					{
						++flashingCnt;
						if(flashingCnt >= 30)
						{
							LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] & 0b00010000;   // abcdfgeX
							LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] & 0b00000000;		// abcdfgeX							
						}
						if (flashingCnt == 50)
						{
							flashingCnt = 0;
						}
					}	
				}
				//**********************
				// STARTING HOURS UPDATE .3 .4 
				//**********************				
				else if((fSelStartTime_3_HourAdjust)|(fSelStartTime_3_MinuteAdjust)|(fSelStartTime_4_HourAdjust)|(fSelStartTime_4_MinuteAdjust))
				{
					char tmp_char_1[1];
					char tmp_char_2[1];
					char tmp_char_3[1];
					char tmp_char_4[1];
					
					uint8_t tmp_start_hour = 0;
					uint8_t tmp_start_min = 0;
					
					if((sel_start_time_min[selectedSelenoid-1][2] == 0xFF)&(sel_start_time_hour[selectedSelenoid-1][2] ==0xFF))
					{
						LCD_Ram_Data_Row[5] = LCD_Ram_Data_Row[5] |  0x10;
						LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] |  0x10;
						LCD_Ram_Data_Row[7] = LCD_Ram_Data_Row[7] |  0x10;
						LCD_Ram_Data_Row[8] = LCD_Ram_Data_Row[8] |  0x10;							
						LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] | 0x01;   // saat arasindaki ":"	
					}
					else	
					{	
						tmp_start_hour = bcd2cnv(sel_start_time_hour[selectedSelenoid-1][2]);
						tmp_start_min = bcd2cnv(sel_start_time_min[selectedSelenoid-1][2]);

						sprintf(tmp_char_1, "%d", (tmp_start_hour>>4)&0x0F);
						sprintf(tmp_char_2, "%d", (tmp_start_hour)&0x0F);
						sprintf(tmp_char_3, "%d", (tmp_start_min>>4)&0x0F);
						sprintf(tmp_char_4, "%d", (tmp_start_min)&0x0F);

						// HOUR UPDATE 1				
						LCD_Ram_Data_Row[5] = sevenSegCode(tmp_char_4[0]);
						LCD_Ram_Data_Row[6] = sevenSegCode(tmp_char_3[0]);
						LCD_Ram_Data_Row[7] = sevenSegCode(tmp_char_2[0]);
						LCD_Ram_Data_Row[8] = sevenSegCode(tmp_char_1[0]);								
						LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] | 0x01;   // saat arasindaki ":"	
					
					}
					//
					//
					if((sel_start_time_min[selectedSelenoid-1][3] == 0xFF)&(sel_start_time_hour[selectedSelenoid-1][3] ==0xFF))
					{				
						// HOUR UPDATE 2
						LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] |  0b00000001;
						LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] |  0b00000001;
						LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] |  0b00000001;
						LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] |  0b00000001;	
						LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] |  0x10;   // saat arasindaki ":"	
					}			
					else
					{
						tmp_start_hour = bcd2cnv(sel_start_time_hour[selectedSelenoid-1][3]);
						tmp_start_min = bcd2cnv(sel_start_time_min[selectedSelenoid-1][3]);

						sprintf(tmp_char_1, "%d", (tmp_start_hour>>4)&0x0F);
						sprintf(tmp_char_2, "%d", (tmp_start_hour)&0x0F);
						sprintf(tmp_char_3, "%d", (tmp_start_min>>4)&0x0F);
						sprintf(tmp_char_4, "%d", (tmp_start_min)&0x0F);
						
						// HOUR UPDATE 2				
						LCD_Ram_Data_Row[4] = (sevenSegCode(tmp_char_4[0])<<4)|(sevenSegCode(tmp_char_4[0])>>4);
						LCD_Ram_Data_Row[3] = (sevenSegCode(tmp_char_3[0])<<4)|(sevenSegCode(tmp_char_3[0])>>4);
						LCD_Ram_Data_Row[2] = (sevenSegCode(tmp_char_2[0])<<4)|(sevenSegCode(tmp_char_2[0])>>4);
						LCD_Ram_Data_Row[1] = (sevenSegCode(tmp_char_1[0])<<4)|(sevenSegCode(tmp_char_1[0])>>4);								
						LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] | 0x10;   // saat arasindaki ":"	
					}
				
					// HOUR SELECT DIGITS 1. 2. 3. 4.
					//LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] | 0x10;   // saatten önceki 1. 2. ":"
					//LCD_Ram_Data_Row[8] = LCD_Ram_Data_Row[8] | 0x01;   // saatten önceki 1. 2. ":"
					LCD_Ram_Data_Row[0] = LCD_Ram_Data_Row[0] | 0x0C;   // saatten önceki 3. 4. ":"
						
					//
					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0F; // sand watch full on;
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |  0xF0; // sand watch full on;
					//
					if(fSelStartTime_3_HourAdjust)   // ayar modunda flashlasin
					{
						++flashingCnt;
						if(flashingCnt >= 30)
						{
							LCD_Ram_Data_Row[7] = LCD_Ram_Data_Row[7] & 0b00000001;   // abcdfgeX
							LCD_Ram_Data_Row[8] = LCD_Ram_Data_Row[8] & 0b00000001;		// abcdfgeX							
						}
						if (flashingCnt == 50)
						{
							flashingCnt = 0;
						}
					}		
					else if(fSelStartTime_3_MinuteAdjust)
					{
						++flashingCnt;
						if(flashingCnt >= 30)
						{
							LCD_Ram_Data_Row[5] = LCD_Ram_Data_Row[5] & 0b00000001;   // abcdfgeX
							LCD_Ram_Data_Row[6] = LCD_Ram_Data_Row[6] & 0b00000001;		// abcdfgeX							
						}
						if (flashingCnt == 50)
						{
							flashingCnt = 0;
						}
					}		
					else if(fSelStartTime_4_HourAdjust)   // ayar modunda flashlasin
					{
						++flashingCnt;
						if(flashingCnt >= 30)
						{
							LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] & 0b00010000;   // abcdfgeX
							LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] & 0b00000000;		// abcdfgeX							
						}
						if (flashingCnt == 50)
						{
							flashingCnt = 0;
						}
					}		
					else if(fSelStartTime_4_MinuteAdjust)
					{
						++flashingCnt;
						if(flashingCnt >= 30)
						{
							LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] & 0b00010000;   // abcdfgeX
							LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] & 0b00000000;		// abcdfgeX							
						}
						if (flashingCnt == 50)
						{
							flashingCnt = 0;
						}
					}	
				}				
			}
			/*************************************
			SPECIAL PROGRAMMING SCREEN  - DONGU PROGRAMLAMA
			**************************************/		
			//**********************
			// "HERGUN" SCREEN
			//**********************	
			if(fHergunSelected)
			{	
				LCD_Ram_Data_Row[15] = sevenSegCode('H');	
				LCD_Ram_Data_Row[14] = sevenSegCode('E');
				LCD_Ram_Data_Row[13] = sevenSegCode('A');
				LCD_Ram_Data_Row[12] = sevenSegCode('G');	// EKRANDA HERGUN YAZAR
				LCD_Ram_Data_Row[11] = sevenSegCode('U');
				LCD_Ram_Data_Row[10] = sevenSegCode('N');
				
				// WEEK OF DAYS hepsi yanar
				LCD_Ram_Data_Row[0] = LCD_Ram_Data_Row[0] | 0xF3;   // Pt,Sa,Ca,Pe ....	Water Drop
				LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9] | 0x88;   // Pt,Sa,Ca,Pe ....	Water Drop
			}
			else if(fDelayedIrrigationSelected)
			{			
				uint8_t tmp_delay_val = 0;	
				char tmp_char_delay_1[1];	
				char tmp_char_delay_2[1];					
				tmp_delay_val = bcd2cnv(sel_delayed_irr_val[selectedSelenoid-1]);
				sprintf(tmp_char_delay_1, "%d", (tmp_delay_val>>4)&0x0F);
				sprintf(tmp_char_delay_2, "%d", (tmp_delay_val)&0x0F);
					
				// DELAYED IRRIGATION VALUE UPDATE						
				LCD_Ram_Data_Row[15] = (sevenSegCode(tmp_char_delay_1[0]));
				LCD_Ram_Data_Row[14] = (sevenSegCode(tmp_char_delay_2[0]));
				LCD_Ram_Data_Row[13] = sevenSegCode('X');
				LCD_Ram_Data_Row[12] = sevenSegCode('G');	// XX GUN 
				LCD_Ram_Data_Row[11] = sevenSegCode('U');
				LCD_Ram_Data_Row[10] = sevenSegCode('N');
				
				if(fDelayValueAdjust)   // ayar modunda flashlasin
				{
					++flashingCnt;
					if(flashingCnt >= 30)
					{
						LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] & 0b00000001;   // abcdfgeX
						LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] & 0b00000001;		// abcdfgeX							
					}
					if (flashingCnt == 50)
					{
						flashingCnt = 0;
					}
				}
			}
			else if(fSpecialSelected) // ozel programlama  // ozel yazisi kaysin isteniyor
			{
				uint8_t tmp_day_PT = 0;
				uint8_t tmp_day_SA = 0;
				uint8_t tmp_day_CA = 0;
				uint8_t tmp_day_PE = 0;
				uint8_t tmp_day_CU = 0;
				uint8_t tmp_day_CT = 0;
				uint8_t tmp_day_PZ = 0;		
				
				tmp_day_PT = (sel_special_irr_days[selectedSelenoid-1]&0x01)<<7;
				tmp_day_SA = (sel_special_irr_days[selectedSelenoid-1]&0x02)<<5;
				tmp_day_CA = (sel_special_irr_days[selectedSelenoid-1]&0x04)<<3;
				tmp_day_PE = (sel_special_irr_days[selectedSelenoid-1]&0x08)>>3;
				tmp_day_CU = (sel_special_irr_days[selectedSelenoid-1]&0x10)>>3;
				tmp_day_CT = (sel_special_irr_days[selectedSelenoid-1]&0x20)<<2;
				tmp_day_PZ = (sel_special_irr_days[selectedSelenoid-1]&0x40)>>3;
				
				if(fSpecialDaySelect_PT)	{tmp_day_PT = 0x80;}
				else if(fSpecialDaySelect_SA)	{tmp_day_SA = 0x40;}
				else if(fSpecialDaySelect_CA)	{tmp_day_CA = 0x20;}
				else if(fSpecialDaySelect_PE)	{tmp_day_PE = 0x01;}	 
				else if(fSpecialDaySelect_CU)	{tmp_day_CU = 0x02;}
				else if(fSpecialDaySelect_CT)	{tmp_day_CT = 0x80;}	
				else if(fSpecialDaySelect_PZ)	{tmp_day_PZ = 0x08;}
				
				++flashingCnt2;
				if (flashingCnt2 == 60)
				{
					flashingCnt2 = 0;	
				}	
				if(flashingCnt2 >= 30)
				{
					if(fSpecialDaySelect_PT)	{tmp_day_PT = 0;}
					else if(fSpecialDaySelect_SA)	{tmp_day_SA = 0;}
					else if(fSpecialDaySelect_CA)	{tmp_day_CA = 0;}
					else if(fSpecialDaySelect_PE)	{tmp_day_PE = 0;}	 // EKLLL
					else if(fSpecialDaySelect_CU)	{tmp_day_CU = 0;}
					else if(fSpecialDaySelect_CT)	{tmp_day_CT = 0;}	
					else if(fSpecialDaySelect_PZ)	{tmp_day_PZ = 0;}				
				}								
					

				// WEEK OF DAYS hepsi yanar
				LCD_Ram_Data_Row[0] = LCD_Ram_Data_Row[0] | 0x10;   // Pt,Sa,Ca,Pe .. Always On
				LCD_Ram_Data_Row[0] = (LCD_Ram_Data_Row[0]&0b00011100) | tmp_day_PT| tmp_day_SA| tmp_day_CA| tmp_day_CU| tmp_day_PE;   // Pt,Sa,Ca,X,X,X,Cu,Pe .... Water Drop
				LCD_Ram_Data_Row[9] = (LCD_Ram_Data_Row[9]&0b01110111) | tmp_day_CT| tmp_day_PZ;   // Ct,X,X,X,PZ,X,X,X  .... Water Drop				

				
				
				++flashingCnt;
				if(flashingCnt == 10)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('0');	
					LCD_Ram_Data_Row[14] = sevenSegCode('2');
					LCD_Ram_Data_Row[13] = sevenSegCode('E');
					LCD_Ram_Data_Row[12] = sevenSegCode('L');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('X');
					LCD_Ram_Data_Row[10] = sevenSegCode('G');
					
				}
				else if(flashingCnt == 40)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('2');	
					LCD_Ram_Data_Row[14] = sevenSegCode('E');
					LCD_Ram_Data_Row[13] = sevenSegCode('L');
					LCD_Ram_Data_Row[12] = sevenSegCode('X');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('G');
					LCD_Ram_Data_Row[10] = sevenSegCode('U');
				
				}
				else if(flashingCnt == 70)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('E');	
					LCD_Ram_Data_Row[14] = sevenSegCode('L');
					LCD_Ram_Data_Row[13] = sevenSegCode('X');
					LCD_Ram_Data_Row[12] = sevenSegCode('G');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('U');
					LCD_Ram_Data_Row[10] = sevenSegCode('N');
					
				}
				else if(flashingCnt == 100)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('L');	
					LCD_Ram_Data_Row[14] = sevenSegCode('X');
					LCD_Ram_Data_Row[13] = sevenSegCode('G');
					LCD_Ram_Data_Row[12] = sevenSegCode('U');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('N');
					LCD_Ram_Data_Row[10] = sevenSegCode('L');
					
				}	
				else if(flashingCnt == 130)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('X');	
					LCD_Ram_Data_Row[14] = sevenSegCode('G');
					LCD_Ram_Data_Row[13] = sevenSegCode('U');
					LCD_Ram_Data_Row[12] = sevenSegCode('N');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('L');
					LCD_Ram_Data_Row[10] = sevenSegCode('E');					
				}	
				else if(flashingCnt == 160)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('G');	
					LCD_Ram_Data_Row[14] = sevenSegCode('U');
					LCD_Ram_Data_Row[13] = sevenSegCode('N');
					LCD_Ram_Data_Row[12] = sevenSegCode('L');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('E');
					LCD_Ram_Data_Row[10] = sevenSegCode('R');					
				}
				else if(flashingCnt == 190)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('U');	
					LCD_Ram_Data_Row[14] = sevenSegCode('N');
					LCD_Ram_Data_Row[13] = sevenSegCode('L');
					LCD_Ram_Data_Row[12] = sevenSegCode('E');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('R');
					LCD_Ram_Data_Row[10] = sevenSegCode('X');					
				}		
				else if(flashingCnt == 220)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('N');	
					LCD_Ram_Data_Row[14] = sevenSegCode('L');
					LCD_Ram_Data_Row[13] = sevenSegCode('E');
					LCD_Ram_Data_Row[12] = sevenSegCode('R');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('X');
					LCD_Ram_Data_Row[10] = sevenSegCode('0');					
				}	
				else if(flashingCnt == 250)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('L');	
					LCD_Ram_Data_Row[14] = sevenSegCode('E');
					LCD_Ram_Data_Row[13] = sevenSegCode('R');
					LCD_Ram_Data_Row[12] = sevenSegCode('X');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('0');
					LCD_Ram_Data_Row[10] = sevenSegCode('2');					
				}
				else if(flashingCnt == 280)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('E');	
					LCD_Ram_Data_Row[14] = sevenSegCode('R');
					LCD_Ram_Data_Row[13] = sevenSegCode('X');
					LCD_Ram_Data_Row[12] = sevenSegCode('0');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('2');
					LCD_Ram_Data_Row[10] = sevenSegCode('E');					
				}			
				else if(flashingCnt == 310)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('R');	
					LCD_Ram_Data_Row[14] = sevenSegCode('X');
					LCD_Ram_Data_Row[13] = sevenSegCode('0');
					LCD_Ram_Data_Row[12] = sevenSegCode('2');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('E');
					LCD_Ram_Data_Row[10] = sevenSegCode('L');					
				}	
				else if(flashingCnt == 340)
				{
					LCD_Ram_Data_Row[15] = sevenSegCode('X');	
					LCD_Ram_Data_Row[14] = sevenSegCode('0');
					LCD_Ram_Data_Row[13] = sevenSegCode('2');
					LCD_Ram_Data_Row[12] = sevenSegCode('E');	// EKRANDA OZEL YAZISI KAYAR
					LCD_Ram_Data_Row[11] = sevenSegCode('L');
					LCD_Ram_Data_Row[10] = sevenSegCode('X');					
				}			
				else if(flashingCnt == 360)
				{
					flashingCnt = 0;
				}				
			}	
		}	
	/*************************************
		......
		....
		..
		.
		MANUEL IRRIGATION SCREEN
		.
		..
		....
		......
	**************************************/		
		else if(fmanuelIrrigationAdjust)
		{
			//
			LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | 0x80; // supiri logo on;
			//
			//
			updateBatLevelDigit(); // show battery level;
			//
			//
			LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0x11; // hand ( manuel irrigation icon) on;
			//
			//
			if(fActivateManuelMode)
			{
				++flashingCnt4;
				if(flashingCnt4 >= 30)
				{
					LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] & 0b11101110;
				}
				if (flashingCnt4 == 50)
				{
					flashingCnt4 = 0;
				}
			}
		
			if((fManuelRelayAdjust)||(fManuelWorkDurationAdjust)||(fManuelAllRelayWorkDurAdj)||(fStartManuelWork)||(fStartAllRelayManuelWork))
			{
			//***********************
			// SELENOID NUMBER UPDATE
			//***********************
				if(fAll_Her_Selected == 1)
				{

					LCD_Ram_Data_Row[8] = sevenSegCode('H');
					LCD_Ram_Data_Row[7] = sevenSegCode('E');
					LCD_Ram_Data_Row[6] = sevenSegCode('R');
					LCD_Ram_Data_Row[5] = sevenSegCode('X');								
	
					// HOUR UPDATE 2
					LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] |  (sevenSegCode('A')<<4)|(sevenSegCode('A')>>4);
					LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] |  (sevenSegCode('L')<<4)|(sevenSegCode('L')>>4);
					LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] |  (sevenSegCode('L')<<4)|(sevenSegCode('L')>>4);
					LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] |  (sevenSegCode('X')<<4)|(sevenSegCode('X')>>4);
			
				}
				else if((selectedSelenoid == 1)&&(!fStartManuelWork)&&(!fStartAllRelayManuelWork))
				{
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00000110;   // XXXXabcd   // 1
					LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b00000000;		// fgeXXXXX		
				}
				else if((selectedSelenoid == 2)&&(!fStartManuelWork)&&(!fStartAllRelayManuelWork))
				{
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00001101;   // XXXXabcd   // 2
					LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b01100000;		// fgeXXXXX		
				}			
				else if((selectedSelenoid == 3)&&(!fStartManuelWork)&&(!fStartAllRelayManuelWork))
				{
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00001111;   // XXXXabcd   // 3
					LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b01000000;		// fgeXXXXX			
				}			
				else if((selectedSelenoid == 4)&&(!fStartManuelWork)&&(!fStartAllRelayManuelWork))
				{
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00000110;   // XXXXabcd   // 4
					LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b11000000;		// fgeXXXXX		
				}			

				if(fManuelRelayAdjust)
				{
					if(fAll_Her_Selected == 1)
					{
						++flashingCnt6;
						if(flashingCnt6 >= 30)
						{
							LCD_Ram_Data_Row[1] = 0;
							LCD_Ram_Data_Row[2] = 0;
							LCD_Ram_Data_Row[3] = 0;
							LCD_Ram_Data_Row[4] = 0;
							LCD_Ram_Data_Row[5] = 0;
							LCD_Ram_Data_Row[6] = 0;
							LCD_Ram_Data_Row[7] = 0;
							LCD_Ram_Data_Row[8] = 0;	
						}
						if (flashingCnt6 == 50)
						{
							flashingCnt6 = 0;
						}	
					}
					else
					{
						++flashingCnt5;
						if(flashingCnt5 >= 30)
						{
							LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] & 0b11110000;   // XXXXabcd
							LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] & 0b00011111;		// fgeXXXXX		
						}
						if (flashingCnt5 == 50)
						{
							flashingCnt5 = 0;
						}	
					}			
				}		
				//******************************
				// SELENOID WORK DURATION UPDATE
				//******************************	
				if((fManuelWorkDurationAdjust) || (fManuelAllRelayWorkDurAdj))
				{	
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0x30; // sand watch icon on;
					++flashingCnt8;
					if(flashingCnt8 == 10)
					{
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0x40; // sand watch full on;
					}
					else if(flashingCnt8 == 20)
					{
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt8 == 30)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x08; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt8 == 40)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0C; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt8 == 50)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0E; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt8 == 60)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0F; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt8 == 70)
					{
						flashingCnt8 = 0;
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] &  0xF0; // sand watch animation full off;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] &   0x3F; // sand watch animation full off;
					}	
				}
				if(fManuelWorkDurationAdjust)
				{				
					uint32_t tmp_work_duration0 = 0; 
					char tmpDurTens[1];
					char tmpDurUnits[1];
					
					tmp_work_duration0 = sel_manuel_work_duration[selectedSelenoid-1];
		
					tmp_work_duration0 = bcd2cnv(tmp_work_duration0);
					
					sprintf(tmpDurTens, "%d", (tmp_work_duration0>>4)&0x0F);
					sprintf(tmpDurUnits, "%d", (tmp_work_duration0)&0x0F);
						
					// DURATION MIN UNITES UPDATE
					LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] |  ((sevenSegCode(tmpDurTens[0])>>4) & 0x0F);    // XXXXabcd
					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  ((sevenSegCode(tmpDurTens[0])<<4) & 0xE0);	 // fgeXXXXX	
					// DURATION MIN TENS UPDATE
					LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | ((sevenSegCode(tmpDurUnits[0])>>4) & 0x0F);   // XXXXabcd 
					LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] | ((sevenSegCode(tmpDurUnits[0])<<4) & 0xE0);		// fgeXXXXX
					
					++flashingCnt2;
					if(flashingCnt2 >= 30)
					{
						LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] & 0b11110000;   // XXXXabcd
						LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] & 0b00010000;		// fgeXXXXX	
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] & 0b00011111;		// fgeXXXXX							
					}
					if (flashingCnt2 == 50)
					{
						flashingCnt2 = 0;
					}			
				}		
				if(fManuelAllRelayWorkDurAdj)
				{				
					uint32_t tmp_work_duration1 = 0; 
					char tmpDurTens[1];
					char tmpDurUnits[1];
					
					tmp_work_duration1 = all_manuel_work_duration;
		
					tmp_work_duration1 = bcd2cnv(tmp_work_duration1);
					
					sprintf(tmpDurTens, "%d", (tmp_work_duration1>>4)&0x0F);
					sprintf(tmpDurUnits, "%d", (tmp_work_duration1)&0x0F);
						
					// DURATION MIN UNITES UPDATE
					LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] |  ((sevenSegCode(tmpDurTens[0])>>4) & 0x0F);    // XXXXabcd
					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  ((sevenSegCode(tmpDurTens[0])<<4) & 0xE0);	 // fgeXXXXX	
					// DURATION MIN TENS UPDATE
					LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | ((sevenSegCode(tmpDurUnits[0])>>4) & 0x0F);   // XXXXabcd 
					LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] | ((sevenSegCode(tmpDurUnits[0])<<4) & 0xE0);		// fgeXXXXX
					
					++flashingCnt2;
					if(flashingCnt2 >= 30)
					{
						LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] & 0b11110000;   // XXXXabcd
						LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] & 0b00010000;		// fgeXXXXX	
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] & 0b00011111;		// fgeXXXXX							
					}
					if (flashingCnt2 == 50)
					{
						flashingCnt2 = 0;
					}			
				}						
				//
				// manuel Mode kurulmus
				//
				else if((fStartManuelWork)||(fStartAllRelayManuelWork))
				{		
					uint32_t tmp_work_duration = 0; 
					char tmpDurTens[1];
					char tmpDurUnits[1];
					uint32_t manuelWorkEndTime = 0;
					uint32_t currentTime = 0;
					int endTimeHourDelay = 0;
					
					manuelWorkEndTime = RTC->ALRMAR;
					currentTime =  RTC->TR;
					manuelWorkEndTime = manuelWorkEndTime&0x7FFFFFFF;
					
					if((currentTime & (RTC_TR_HT|RTC_TR_HU)) == ((manuelWorkEndTime) & (RTC_TR_HT|RTC_TR_HU)))
					{
						tmp_work_duration = (((manuelWorkEndTime&(RTC_TR_MNT))>>12)*10 + ((manuelWorkEndTime&(RTC_TR_MNU))>>8)) - (((currentTime&RTC_TR_MNT)>>12)*10 + ((currentTime&RTC_TR_MNU)>>8));
					}
					else
					{
						endTimeHourDelay = ((manuelWorkEndTime & RTC_TR_HT)>>20)*10 + ((manuelWorkEndTime & RTC_TR_HU)>>16) - (((currentTime & RTC_TR_HT)>>20)*10 + ((currentTime & RTC_TR_HU)>>16)) ;
						if( endTimeHourDelay == 1) // saat farki 1 : örnek: 1:50 de 65 dakika sulama kurduk. bitis saati 2: 55
						{
							tmp_work_duration = 60  + ((manuelWorkEndTime&(RTC_TR_MNT))>>12)*10 + ((manuelWorkEndTime&(RTC_TR_MNU))>>8) - (((currentTime&RTC_TR_MNT)>>12)*10 + ((currentTime&RTC_TR_MNU)>>8));
						}
						else if (endTimeHourDelay == 2) // saat farki 2 : örnek: 1:50 de 90 dakika sulama kurduk. bitis saati 3: 20
						{
							tmp_work_duration = 120  + (((manuelWorkEndTime&(RTC_TR_MNT))>>12)*10 + ((manuelWorkEndTime&(RTC_TR_MNU))>>8)) - (((currentTime&RTC_TR_MNT)>>12)*10 + ((currentTime&RTC_TR_MNU)>>8));
						}
						else if (endTimeHourDelay == -23) // saat farki -23 : örnek: 23:50 de 30 dakika sulama kurduk. bitis saati 0: 20
						{
							tmp_work_duration = 60  + (((manuelWorkEndTime&(RTC_TR_MNT))>>12)*10 + ((manuelWorkEndTime&(RTC_TR_MNU))>>8)) - (((currentTime&RTC_TR_MNT)>>12)*10 + ((currentTime&RTC_TR_MNU)>>8));
						}
						else // saat farki -22 : örnek: 23:50 de 90 dakika sulama kurduk. bitis saati 1: 20
						{
							tmp_work_duration = 120  + (((manuelWorkEndTime&(RTC_TR_MNT))>>12)*10 + ((manuelWorkEndTime&(RTC_TR_MNU))>>8)) - (((currentTime&RTC_TR_MNT)>>12)*10 + ((currentTime&RTC_TR_MNU)>>8));
						}				
					}
					
					if(startManuelWorkDelay)
					{
						if(fStartManuelWork)
						{
							tmp_work_duration = sel_manuel_work_duration[selectedSelenoid-1];  // biraz gecikme ekliyoruz ilk basta kurulu olan alarmdan kalan süre hesaplanmasin diye
						}
						else //fStartAllRelayManuelWork
						{
							tmp_work_duration = all_manuel_work_duration;
						}
					}
										
					tmp_work_duration = bcd2cnv(tmp_work_duration);
					
					sprintf(tmpDurTens, "%d", (tmp_work_duration>>4)&0x0F);
					sprintf(tmpDurUnits, "%d", (tmp_work_duration)&0x0F);
					
					LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] & 0b11110000;   // XXXXabcd
					LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] & 0b00010000;		// fgeXXXXX	
					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] & 0b00011111;		// fgeXXXXX		
						
					// DURATION MIN UNITES UPDATE
					LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] |  ((sevenSegCode(tmpDurTens[0])>>4) & 0x0F);    // XXXXabcd
					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  ((sevenSegCode(tmpDurTens[0])<<4) & 0xE0);	 // fgeXXXXX	
					// DURATION MIN TENS UPDATE
					LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | ((sevenSegCode(tmpDurUnits[0])>>4) & 0x0F);   // XXXXabcd 
					LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] | ((sevenSegCode(tmpDurUnits[0])<<4) & 0xE0);		// fgeXXXXX
				
					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0x30; // sand watch icon on;
					++flashingCnt7;
					if(flashingCnt7 == 10)
					{
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0x40; // sand watch full on;
					}
					else if(flashingCnt7 == 20)
					{
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt7 == 30)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x08; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt7 == 40)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0C; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt7 == 50)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0E; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt7 == 60)
					{
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0F; // sand watch full on;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
					}
					else if(flashingCnt7 == 70)
					{
						flashingCnt7 = 0;
						LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] &  0xF0; // sand watch animation full off;
						LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] &   0x3F; // sand watch animation full off;
					}				
				}				
			}
		}
/*************************************
	......
	....
	..
	.
	MANUEL IRRIGATION PLAN SCREEN
	.
	..
	....
	......
**************************************/		
//		else if(fmanuelIrrigationPlanAdjust)
//		{	
//			//
//			LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | 0x80; // supiri logo on;
//			//
//			//
//			updateBatLevelDigit(); // show battery level;
//			//
//			//
//			//LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0x11; // hand ( manuel irrigation icon) on;
//			//
//			//
//			LCD_Ram_Data_Row[8] = sevenSegCode('P');
//			LCD_Ram_Data_Row[7] = sevenSegCode('L');
//			LCD_Ram_Data_Row[6] = sevenSegCode('A');
//			LCD_Ram_Data_Row[5] = sevenSegCode('N');			
//			if(fActivateManuelPlanMode)
//			{
//				++flashingCnt;
//				if(flashingCnt >= 30)
//				{
////					LCD_Ram_Data_Row[8] = sevenSegCode('P');
////					LCD_Ram_Data_Row[7] = sevenSegCode('L');
////					LCD_Ram_Data_Row[6] = sevenSegCode('A');
////					LCD_Ram_Data_Row[5] = sevenSegCode('N');
//					LCD_Ram_Data_Row[8] = sevenSegCode('X');
//					LCD_Ram_Data_Row[7] = sevenSegCode('X');
//					LCD_Ram_Data_Row[6] = sevenSegCode('X');
//					LCD_Ram_Data_Row[5] = sevenSegCode('X');					
//				}
//				if (flashingCnt == 50)
//				{
//					flashingCnt = 0;
////					LCD_Ram_Data_Row[8] = sevenSegCode('X');
////					LCD_Ram_Data_Row[7] = sevenSegCode('X');
////					LCD_Ram_Data_Row[6] = sevenSegCode('X');
////					LCD_Ram_Data_Row[5] = sevenSegCode('X');
//				}				
//			}
//			else
//			{	
////				LCD_Ram_Data_Row[8] = sevenSegCode('P');
////				LCD_Ram_Data_Row[7] = sevenSegCode('L');
////				LCD_Ram_Data_Row[6] = sevenSegCode('A');
////				LCD_Ram_Data_Row[5] = sevenSegCode('N');
//				//
//				LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0x11; // hand ( manuel irrigation icon) on;
//				//		
//				uint32_t tmp_work_duration = 0;
//				char tmpDurTens[1];
//				char tmpDurUnits[1];
//				tmp_work_duration = sel_manuel_work_duration[selectedSelenoid-1];
//				
//					if(tmp_work_duration > 99)
//					{
//						LCD_Ram_Data_Row[10] = sevenSegCode('2');  //pilin yaninda 2 yazisi çiksin 2. 99 daki ayarlandigi belli olur
////						LCD_Ram_Data_Row[11] = sevenSegCode('0');
////						LCD_Ram_Data_Row[12] = sevenSegCode('0');		//
////						LCD_Ram_Data_Row[13] = sevenSegCode('1');
////						LCD_Ram_Data_Row[14] = sevenSegCode('X');
////						LCD_Ram_Data_Row[15] = sevenSegCode('+');	
//						tmp_work_duration = tmp_work_duration - 100;
//					}
//				
//				tmp_work_duration = bcd2cnv(tmp_work_duration);	
//				sprintf(tmpDurTens, "%d", (tmp_work_duration>>4)&0x0F);
//				sprintf(tmpDurUnits, "%d", (tmp_work_duration)&0x0F);		
//				// DURATION MIN UNITES UPDATE
//				LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] |  ((sevenSegCode(tmpDurTens[0])>>4) & 0x0F);    // XXXXabcd
//				LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  ((sevenSegCode(tmpDurTens[0])<<4) & 0xE0);	 // fgeXXXXX	
//				// DURATION MIN TENS UPDATE
//				LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | ((sevenSegCode(tmpDurUnits[0])>>4) & 0x0F);   // XXXXabcd 
//				LCD_Ram_Data_Row[18] = LCD_Ram_Data_Row[18] | ((sevenSegCode(tmpDurUnits[0])<<4) & 0xE0);		// fgeXXXXX			
//				//sand watch icon flashing
//				LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0x30; // sand watch icon on;
//				++flashingCnt;
//				if(flashingCnt == 10)
//				{
//					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0x40; // sand watch full on;
//				}
//				else if(flashingCnt == 20)
//				{
//					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
//				}
//				else if(flashingCnt == 30)
//				{
//					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x08; // sand watch full on;
//					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
//				}
//				else if(flashingCnt == 40)
//				{
//					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0C; // sand watch full on;
//					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
//				}
//				else if(flashingCnt == 50)
//				{
//					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0E; // sand watch full on;
//					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
//				}
//				else if(flashingCnt == 60)
//				{
//					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] |  0x0F; // sand watch full on;
//					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] |   0xC0; // sand watch full on;
//				}
//				else if(flashingCnt == 70)
//				{
//					flashingCnt = 0;
//					LCD_Ram_Data_Row[19] = LCD_Ram_Data_Row[19] &  0xF0; // sand watch animation full off;
//					LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] &   0x3F; // sand watch animation full off;
//				}						
//			}				
//		}			
/*************************************
	......
	....
	..
	.
	PASSWORD ADJUST
	.
	..
	....
	......
**************************************/		
		else if(fPasswordAdjust)
		{	
			LCD_Ram_Data_Row[17] = LCD_Ram_Data_Row[17] | 0x80; // supiri logo on;
			//
			//
			updateBatLevelDigit(); // show battery level;
			//	
			LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9]  | 0x40; // Kilit
			//
			if(fPasswordActivateMode)
			{
				++flashingCnt;
				if(flashingCnt >= 30)
				{
					//LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9]  | 0x40;
					LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9]  & 0xBF;
				}
				if (flashingCnt == 50)
				{
					flashingCnt = 0;
					//LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9]  & 0xBF;
				}				
			}
			else if(fsetNewPasswordMode)
			{
				//LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9]  | 0x40;  // kilit açik
	
				
				if((tmpPassword[0] == 0) & (tmpPassword[1] == 0) & (tmpPassword[2] == 0) & (tmpPassword[3] == 0)) 
				{
					// HOUR UPDATE 2 -- PASSWORD				
						LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] |  0b00000001;
						LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] |  0b00000001;
						LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] |  0b00000001;
						LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] |  0b00000001;	
				}
				else
				{		
					char tmp_char_1[1];
					char tmp_char_2[1];
					char tmp_char_3[1];
					char tmp_char_4[1];
					
					sprintf(tmp_char_1, "%d", (tmpPassword[0]));
					sprintf(tmp_char_2, "%d", (tmpPassword[1]));
					sprintf(tmp_char_3, "%d", (tmpPassword[2]));
					sprintf(tmp_char_4, "%d", (tmpPassword[3]));
					// HOUR UPDATE 2 -- PASSWORD				
					LCD_Ram_Data_Row[4] = (sevenSegCode(tmp_char_4[0])<<4)|(sevenSegCode(tmp_char_4[0])>>4);
					LCD_Ram_Data_Row[3] = (sevenSegCode(tmp_char_3[0])<<4)|(sevenSegCode(tmp_char_3[0])>>4);
					LCD_Ram_Data_Row[2] = (sevenSegCode(tmp_char_2[0])<<4)|(sevenSegCode(tmp_char_2[0])>>4);
					LCD_Ram_Data_Row[1] = (sevenSegCode(tmp_char_1[0])<<4)|(sevenSegCode(tmp_char_1[0])>>4);
				}	
				if(fPasswordDigit1Adjust)  // password 1 ayarlaniyor
				{
					++flashingCnt;
					if(flashingCnt > 20)
					{
						LCD_Ram_Data_Row[1] = LCD_Ram_Data_Row[1] & 0b00010000;   // abcdfgeX
					}
					if (flashingCnt == 50)
					{
						flashingCnt = 0;	
					}				
				}	
				if(fPasswordDigit2Adjust)  // password 1 ayarlaniyor
				{
					++flashingCnt;
					if(flashingCnt > 20)
					{
						LCD_Ram_Data_Row[2] = LCD_Ram_Data_Row[2] & 0b00000000;   // abcdfgeX
					}
					if (flashingCnt == 50)
					{
						flashingCnt = 0;	
					}			
				}
				if(fPasswordDigit3Adjust)  // password 1 ayarlaniyor
				{
					++flashingCnt;
					if(flashingCnt > 20)
					{
						LCD_Ram_Data_Row[3] = LCD_Ram_Data_Row[3] & 0b00010000;   // abcdfgeX
					}
					if (flashingCnt == 50)
					{
						flashingCnt = 0;	
					}				
				}	
				if(fPasswordDigit4Adjust)  // password 1 ayarlaniyor
				{
					++flashingCnt;
					if(flashingCnt > 20)
					{
						LCD_Ram_Data_Row[4] = LCD_Ram_Data_Row[4] & 0b00000000;   // abcdfgeX
					}
					if (flashingCnt == 50)
					{
						flashingCnt = 0;	
					}				
				}					
			}			
		}
	/*************************************
		......
		....
		..
		.
		SPRINK ON WHEN ANY RELAY ON
		.
		..
		....
		......
	**************************************/	

		if((fAnyRelayON)&(!fShowBatteryLevel))
		{
			//
			// FISKIYE
			//
			LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0b00100000;		// FISKIYE SUREKLI ACIK
			
			++flashingCnt3;
			if(flashingCnt3 == 10)
			{
				LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] | 0b00000001;   // SAG DAMLA 1
				LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0b01000000;		// SOL DAMLA 1
			}
			else if(flashingCnt3 == 20)
			{
				LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] | 0b00000001;   // SAG DAMLA 2
				LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0b10000000;		// SOL DAMLA 2
			}
			else if(flashingCnt3 == 30)
			{
				LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] | 0b00000001;   // SAG DAMLA 3
				LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0b00001000;		// SOL DAMLA 3
			}
			else if(flashingCnt3 == 40)
			{
				LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] | 0b00000001;   // SAG DAMLA 4
				LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] | 0b00000100;		// SOL DAMLA 4
			}
			else if(flashingCnt3 == 50)
			{					
				LCD_Ram_Data_Row[12] = LCD_Ram_Data_Row[12] & ~0b00000001;   // SAG DAMLA 1
				LCD_Ram_Data_Row[13] = LCD_Ram_Data_Row[13] & ~0b00000001;   // SAG DAMLA 2
				LCD_Ram_Data_Row[14] = LCD_Ram_Data_Row[14] & ~0b00000001;   // SAG DAMLA 3
				LCD_Ram_Data_Row[15] = LCD_Ram_Data_Row[15] & ~0b00000001;   // SAG DAMLA 4						
				LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] & ~0b01000000;		// SOL DAMLA 1
				LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] & ~0b10000000;		// SOL DAMLA 2					
				LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] & ~0b00001000;		// SOL DAMLA 3	
				LCD_Ram_Data_Row[16] = LCD_Ram_Data_Row[16] & ~0b00000100;		// SOL DAMLA 4
			}
			else if(flashingCnt3 == 60)
			{					
				flashingCnt3 = 0;
			}	
		}
		if((fAnyRelayON)&(!fShowBatteryLevel)&(!fselenoidProgramming)&((fhomePage)|(fHomePageWithPassword)))
		{
			//
			// AKTIF OLAN RÖLE NUMARASI
			//		
			if(fRelay1Status)
			{
				LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00000110;   // XXXXabcd   // 1
				LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b00000000;		// fgeXXXXX		
			}
			else if(fRelay2Status)
			{
				LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00001101;   // XXXXabcd   // 2
				LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b01100000;		// fgeXXXXX		
			}			
			else if(fRelay3Status)
			{
				LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00001111;   // XXXXabcd   // 3
				LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b01000000;		// fgeXXXXX			
			}			
			else if(fRelay4Status)
			{
				LCD_Ram_Data_Row[20] = LCD_Ram_Data_Row[20] | 0b00000110;   // XXXXabcd   // 4
				LCD_Ram_Data_Row[21] = LCD_Ram_Data_Row[21] | 0b11000000;		// fgeXXXXX		
			}				
		}
	}
}


/*******************************************************************************
 *  function :    UPDATE BATTERY LEVEL DIGIT
 ******************************************************************************/
 void updateBatLevelDigit(void)
 {
		if((!powerON) && (fBatteryReady))
		{
			//LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9] | 0x37; // battery full on;  //X,X,BAT3,BAT4,X,BAT0,BAT1,BAT2
		 
			LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9] | 0x04;  //BAT0 always on
		 
		 if(batteryVoltage > 8.4)
		 {
			 LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9] | 0x02;  //BAT1 on
		 }
		 if(batteryVoltage > 7.8)
		 {
			 LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9] | 0x01;  //BAT2 on
		 }	
		 if(batteryVoltage > 7.2)
		 {
			 LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9] | 0x20;  //BAT3 on
		 }
		 if(batteryVoltage > 6.6)
		 {
			 LCD_Ram_Data_Row[9] = LCD_Ram_Data_Row[9] | 0x10;  //BAT4 on
		 }	
		} 	 
 }


/*******************************************************************************
 *  function :    LCD BUFFER RESET
 ******************************************************************************/
 void LCDBufferReset(void)
 {
	 for(int i = 0; i < 30; i++)
	 {
		 LCD_Ram_Data_Row[i] = 0x00;
	 }
 }


/* 
7-segment codes.
Dig   gfedcba abcdefg	abcdfge
---   ------- ------- -------	
 0     0x3F    0x7E			0
 1     0x06    0x30
 2     0x5B    0x6D
 3     0x4F    0x79
 4     0x66    0x33
 5     0x6D    0x5B
 6     0x7D    0x5F
 7     0x07    0x70
 8     0x7F    0x7F
 9     0x6F    0x7B
 A     0x77    0x77
 b     0x7C    0x1F
 C     0x39    0x4E
 d     0x5E    0x3D
 E     0x79    0x4F
 F     0x71    0x47

 G     0x6f    0x7B;
 H     0x76    0x37;
 I     0x06    0x30;
 J     0x0e    0x38;
 K     ----    ----
 L     0x38    0x0e;
 M     ----    ----
 N     0x54    ----;  (poor char)
 O     0x3f    0x7e;
 P     0x73    0x67;
 Q     ----    ----
 R     ----    ----
 S     0x6d    0x5B;
 T     0x78    ----
 U     0x3e    0x3E;
 V     ----    ----
 W     ----    ----
 X     ----    ----
 Y     0x6e    ----
 Z     ----    ----
' '    ----    0x00;
'-'    0x40    0x01;

*/

// sevenSegCode
//
// Given a hexadecimal digit, return the segment code that will
// display that digit on our LCD.  Returns 0xff if digit is not valid.
// our digit order abcdfge


/*

-----a------
-						-
f						b
-						-
-----g-------
-						-
e						c
-						-
------d------

*/
uint8_t sevenSegCode(char c)
{
    uint8_t code = 0;

    switch(toupper(c))
    {										//abcdfge---LSB=0
        case 'X': code = 0b00000000; break;
        case '-': code = 0b00000100; break;
				case '_': code = 0b00010000; break;
        case '0': code = 0b11111010; break;
        case '1': code = 0b01100000; break;
        case '2': code = 0b11010110; break;
        case '3': code = 0b11110100; break;
        case '4': code = 0b01101100; break;
        case '5': code = 0b10111100; break;
        case '6': code = 0b10111110; break;
        case '7': code = 0b11100000; break;
        case '8': code = 0b11111110; break;
        case '9': code = 0b11111100; break;
        case 'A': code = 0b11101110; break;
        case 'B': code = 0b00111110; break;
        case 'C': code = 0b00010110; break;
        case 'D': code = 0b01111010; break;
        case 'E': code = 0b10011110; break;
        case 'F': code = 0b10001110; break;
				case 'G': code = 0b10111110; break;
        case 'H': code = 0b01101110; break;
        case 'I': code = 0b00001010; break;
				case 'i': code = 0b00100000; break;
        case 'L': code = 0b00011010; break;
				case 'N': code = 0b11101010; break;
				case 'n': code = 0b00100110; break;
        case 'O': code = 0b00110110; break;
        case 'P': code = 0b11001110; break;
				case 'R': code = 0b11101110; break;
				case 'r': code = 0b11001010; break;
				case 'T': code = 0b01100100; break;
				case 'U': code = 0b01111010; break;

        default:
            code = 0x00; break;      // Invalid 
    }
		
    return code;
}
