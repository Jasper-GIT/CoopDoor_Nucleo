#include "RTC_Config.h"
#include "stm32l1xx_hal_rtc.h"

RTC_HandleTypeDef hrtc;


void RTC_Configuration(void){

	  hrtc.Instance = RTC;
	  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	  hrtc.Init.AsynchPrediv = 127;
	  hrtc.Init.SynchPrediv = 255;
	  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	  HAL_RTC_Init(&hrtc);
}


void SetTime_Configuration(unsigned char Hours, unsigned char Minutes){

	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_TimeStructure.Hours = Hours;
	RTC_TimeStructure.Minutes = Minutes;
	RTC_TimeStructure.Seconds = 0x00;
	RTC_TimeStructure.TimeFormat = RTC_HOURFORMAT_24;
	RTC_TimeStructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	RTC_TimeStructure.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
}

void SetDate_Configuration(unsigned char Year, unsigned char Month, unsigned char Date){

	RTC_DateTypeDef RTC_DateStructure;
	RTC_DateStructure.Year = Year;
	RTC_DateStructure.Month = Month;
	RTC_DateStructure.Date = Date;
	HAL_RTC_SetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
}



//void Update_RTC(unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Year, unsigned char Month, unsigned char Day){

void Update_RTC(){

  RTC_AlarmTypeDef sAlarm;
    /**Enable the Alarm A
    */
  sAlarm.AlarmTime.Hours = 16;
  sAlarm.AlarmTime.Minutes = 44;
  sAlarm.AlarmTime.Seconds = 10;
  sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 6;
  sAlarm.Alarm = RTC_ALARM_A;
  HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, FORMAT_BIN);

    /**Enable the Alarm B
    */
  sAlarm.AlarmTime.Hours = 16;
  sAlarm.AlarmTime.Minutes = 43;
  sAlarm.AlarmTime.Seconds = 55;
  sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 6;
  sAlarm.Alarm = RTC_ALARM_B;
  HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, FORMAT_BIN);


}


/*
void HAL_RTC_Alarm_IRQHandler(void){
	 HAL_RTC_AlarmIRQHandler(&hrtc);
	if (__HAL_RTC_ALARM_GET_IT(&hrtc, RTC_IT_ALRA) != RESET){
		 HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);

	}
}
*/


void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  RTC_AlarmTypeDef sAlarm;

    /**Initialize RTC and set the Time and Date
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  HAL_RTC_Init(&hrtc);

  sTime.Hours = 0x15;
  sTime.Minutes = 0x10;
  sTime.Seconds = 0x00;
  sTime.TimeFormat = RTC_HOURFORMAT_24;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BCD);

  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  sDate.Month = RTC_MONTH_NOVEMBER;
  sDate.Date = 0x18;
  sDate.Year = 0x15;

  HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BCD);

    /**Enable the Alarm A
    */
  sAlarm.AlarmTime.Hours = 0x15;
  sAlarm.AlarmTime.Minutes = 0x10;
  sAlarm.AlarmTime.Seconds = 0x11;
  sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 18;
  sAlarm.Alarm = RTC_ALARM_A;
  HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, FORMAT_BCD);

    /**Enable the Alarm B
    */
  sAlarm.AlarmTime.Hours = 0x16;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_B;
  HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, FORMAT_BCD);



}

