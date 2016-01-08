/* @file    main.c
  * @author  Jasper van den Akker
  * @version V0.1.0
  * @date    15-November-2015
  * @brief   Coop door control
  ******************************************************************************
  * @attention
  */


/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct {
	int SunSetUTC;
	int SunSetLT;
	unsigned char Hour;
	unsigned char Minutes;
	unsigned char HoursLT;
}SunSetTypeDef;

typedef struct {
	int SunRiseUTC;
	int SunRiseLT;
	unsigned char Hour;
	unsigned char Minutes;
	unsigned char HoursLT;
}SunRiseTypeDef;
/* Private define ------------------------------------------------------------*/
#define CLOSEDOOR 1
#define OPENDOOR 0
#define STEPS 48000*6 // TBD Later
#define OPENHOUR 9
#define OPENMIN 0
#define LIGHTOFFHR 1
#define LIGHTOFFMIN 0
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t StepperDelay = 300;
bool SunSetCalculationFlag = true;   /*Set this bit True when sunset calculation needs to be done*/
bool DoorOpen = true;
bool LightsON = false;
volatile unsigned TickHigh;           /*Count duration of logic high level*/
volatile unsigned TickLow;            /*Count duration of logic low level*/
static unsigned char DCF_Array[59];   /*The DCF time array, 60 bits*/
static int DCF_Index;
uint8_t CEST;                         /*Central European Summer Time BIT*/
RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef ts;
RTC_DateTypeDef ds;
RTC_AlarmTypeDef as;
/* Private function prototypes -----------------------------------------------*/
void StepperMotor(uint8_t Direction, uint32_t Steps);
uint8_t bcd2dec(uint8_t i);
void HAL_SYSTICK_Callback(void);
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc);
void SunSetCalculation(SunSetTypeDef *sunset_p, SunRiseTypeDef *sunrise_p);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){
	uint8_t Hours = 0;
	uint8_t Minutes = 0;
	uint8_t Seconds = 0;

	uint16_t rtc_year;
	uint8_t rtc_month;
	uint8_t rtc_day;

	/* The folowing integers are for testing purpose,
	 * Here we can set the y/m/d to write into RTC
	 */
	int year = 16;
	int month = 1;
	int day = 6;
	int hour = 16;
	int minutes = 44;


	SunSetTypeDef sunset = {0};
	SunSetTypeDef *sunset_p = &sunset;
	SunRiseTypeDef sunrise = {0};
	SunRiseTypeDef *sunrise_p = &sunrise;

  /* STM32L1xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 32 MHz */
  RCC_Configuration();
  RTC_Configuration();
  GPIO_Configuration();
  Update_RTC();

  //MX_RTC_Init();

  /*
   * Switch RELAYS OFF
   *
   * */
  RELAY_1_LOW();
  RELAY_2_LOW();

  SetDate_Configuration(year, month, day);
  HAL_Delay(200);
  SetTime_Configuration(hour, minutes);

  while (1)
  {

	 // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	  HAL_RTC_GetTime(&hrtc, &ts, FORMAT_BCD);
	  /*Convert the received HMS in BCD format to DEC format*/
	  Hours   = bcd2dec(ts.Hours);
	  Minutes = bcd2dec(ts.Minutes);
	  Seconds = bcd2dec(ts.Seconds);

	  HAL_RTC_GetDate(&hrtc, &ds, FORMAT_BCD);
	  /*convert received date from RTC into decimal values*/
	  rtc_day 	= bcd2dec(ds.Date);
	  rtc_month = bcd2dec(ds.Month);
	  rtc_year 	= bcd2dec(ds.Year) + 2000;


	  /*Calculate the SunSet time
	   * When user button is pressed, or when RTC Alarm goes on
	   * Button Pin is RESET when pushed
	   */
	  if ((SunSetCalculationFlag == false) || (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN) == GPIO_PIN_RESET)){
		  SunSetCalculation(&sunset, &sunrise);
	  }


	  /*
	   * This function will Close the ChickenCoopDoor
	   */
	  if ((Hours == sunset_p->HoursLT) && (Minutes == sunset_p->Minutes) && (DoorOpen == true)){
		  StepperMotor(CLOSEDOOR, STEPS);
		  DoorOpen = false;
	  }
	  else if ((Hours == OPENHOUR) && (Minutes == OPENMIN) && (DoorOpen == false)){
		  StepperMotor(OPENDOOR, STEPS);
		  DoorOpen = true;
	  }


	  /*
	   * This Function switch the lights on and off
	   * Switch ON: @ SunSet and SunRise - 2hrs
	   * Switch OFF: @ SunRise and 01.00hrs
	   */

	  if((LightsON == false) && (((Hours == sunset_p->HoursLT) && (Minutes == sunset_p->Minutes)) || ((Hours == (sunrise_p->HoursLT - 2)) && (Minutes == sunrise_p->Minutes)))){
		  RELAY_1_HIGH();
		  RELAY_2_HIGH();
		  LightsON = true;
	  }
	  else if((LightsON == true) && (((Hours == LIGHTOFFHR) && (Minutes == LIGHTOFFMIN)) || ((Hours == sunrise_p->HoursLT) && (Minutes == sunrise_p->Minutes)))){
		  RELAY_1_LOW();
		  RELAY_2_LOW();
		  LightsON = false;
	  }
  }
}



/*Function to decode BCD code to Decimal*/
uint8_t bcd2dec(uint8_t BCD){
	uint8_t result = (BCD & 0x0F) + 10*((BCD >> 4) & 0x0F);
	return result;
}

/*Stepper motor function*/
/*Called when time is sunset +2 hrs*/
/*0 is close, 1 is open*/
void StepperMotor(uint8_t Direction, uint32_t Steps){
	int i;
	/*ENABLE THE STEPPER MOTOR MODULE BY WRITING LOGIC HIGH TO STEPPER MOTOR DRIVER*/
	MOTOR_SLP_HIGH();
	if(Direction == 0x01){
		/*Set Direction pin to 0*/
		MOTOR_DIR_LOW();
		for(i = 0; i < Steps; i++){
			MOTOR_STEP_TOGGLE();
			HAL_Delay(StepperDelay);
		}
	}
  	else if (Direction == 0x00){
  		/*Set Direction pin to 1*/
  		MOTOR_DIR_HIGH();
  		for(i = 0; i < Steps; i++){
  			MOTOR_STEP_TOGGLE();
  			HAL_Delay(StepperDelay);
  		}
  	}
}
/*
 * @brief, This function handles the system interrupt.
 * every 10ms, this function is called and check for DCF_INPUT.
 * When +/- 100ms high, write a 0 into array. at 200ms write a 1.
 * In this way, we collect 60bits in 1 minute.
 * In these 60 bit, the time and date are coded in BCD.
 */


void HAL_SYSTICK_Callback(void){
	HAL_IncTick();
	if(HAL_GPIO_ReadPin(DCF_INPUT_GPIO_PORT, DCF_INPUT) == GPIO_PIN_SET){
		LED2_HIGH();
		/*Count duration logic high level*/
		TickHigh++;
		/*When pulse is for more than 1.1s LOW*/
		if (TickLow > 110){
			/*Clear whole array*/
			for (DCF_Index = 0; DCF_Index < 60; DCF_Index++){
				DCF_Array[DCF_Index] = 0;
			}
			/*Set index to 0*/
			DCF_Index = 0;
		}
		TickLow = 0;
	}

	if(HAL_GPIO_ReadPin(DCF_INPUT_GPIO_PORT, DCF_INPUT) == GPIO_PIN_RESET){
		//LED2_LOW();
		/*When signal is high between 30 and 130 ms, it's a 0*/
		if((TickHigh > 3) && (TickHigh < 13)){
			/*Store it as a 0 and increment index bit*/
			DCF_Array[DCF_Index] = 0x00;
			DCF_Index++;
		}
		if((TickHigh > 13) && (TickHigh <25)){
			/*Store it as a 1 and increment index bit*/
			DCF_Array[DCF_Index] = 0x01;
			DCF_Index++;
		}
		TickHigh = 0;
	}

	/*
	 * When Array is completely received, start decoding array
	 */
	if(DCF_Index == 59){
		Decode(DCF_Array, &CEST);
	}
}

/*
 * @brief, this is the callback function from RTC alarm.
 * handles the RTC Alarm interrupt A and B.
 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	Update_RTC();
}

void  HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	SunSetCalculationFlag = false;
	Update_RTC();
}

void SunSetCalculation(SunSetTypeDef *sunset_p, SunRiseTypeDef *sunrise_p){
	float JD; // This is the Julian Date

	/* GPS Location @home LAT/LON */
	double latitude = 51.50;
	double longitude = -5.46;

	LED1_LOW();
	HAL_RTC_GetDate(&hrtc, &ds, FORMAT_BCD);
	/*Calculate the Julian Date with the received y, m d*/
	JD = calcJD((bcd2dec(ds.Year) + 2000), (bcd2dec(ds.Month)), (bcd2dec(ds.Date)));
	/*Calculate the SunSet and SunRise times*/
	sunset_p->SunSetUTC = calcSunsetUTC(JD, latitude, longitude);
	sunrise_p->SunRiseUTC = calcSunriseUTC(JD, latitude, longitude);
	/*
	* Since the Sunset/rise is calculated in UTC, we have to add an extra hour,
	* and during daylight saving time 2 hours.
	* add the extra minutes to close the door later
	*/
	if (CEST == 1){
		sunset_p->SunSetLT = sunset_p->SunSetUTC + 120;
		sunrise_p->SunRiseLT = sunrise_p->SunRiseUTC + 120;
	}
	else{
		sunset_p->SunSetLT = sunset_p->SunSetUTC + 60;
		sunrise_p->SunRiseLT = sunrise_p->SunRiseUTC + 60;
	}
	sunset_p->HoursLT = sunset_p->SunSetLT/60;
	sunset_p->Minutes = sunset_p->SunSetLT%60;
	sunrise_p->HoursLT = sunrise_p->SunRiseLT/60;
	sunrise_p->Minutes = sunrise_p->SunRiseLT%60;
	SunSetCalculationFlag = true;
}




