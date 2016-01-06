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
	RTC_TimeStructure.Hours = 0x0A; //Hours;
	RTC_TimeStructure.Minutes = 0x02; //Minutes;
	RTC_TimeStructure.Seconds = 0x00;
	RTC_TimeStructure.TimeFormat = RTC_HOURFORMAT_24;
	RTC_TimeStructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	RTC_TimeStructure.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&hrtc, &RTC_TimeStructure, RTC_FORMAT_BIN);
}

void SetDate_Configuration(unsigned char Year, unsigned char Month, unsigned char Date){

	RTC_DateTypeDef RTC_DateStructure;
	RTC_DateStructure.Year = 0x0F; //Year;
	RTC_DateStructure.Month = 0x0B; //Month;
	RTC_DateStructure.Date = 0x12; //Date;
	HAL_RTC_SetDate(&hrtc, &RTC_DateStructure, RTC_FORMAT_BIN);
}



//void Update_RTC(unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Year, unsigned char Month, unsigned char Day){

void Update_RTC(){

	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_B);

	RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_AlarmStructure.AlarmTime.Hours   = 0x0A;
	RTC_AlarmStructure.AlarmTime.Minutes = 0x03;
 	RTC_AlarmStructure.AlarmTime.Seconds = 0x00;
 	RTC_AlarmStructure.AlarmDateWeekDay = 0x12;
 	RTC_AlarmStructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
 	RTC_AlarmStructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
 	RTC_AlarmStructure.Alarm = RTC_ALARM_A;
 	HAL_RTC_SetAlarm_IT(&hrtc, &RTC_AlarmStructure, RTC_FORMAT_BIN);

 	RTC_AlarmStructure.AlarmTime.Hours   = 0x0A;
 	RTC_AlarmStructure.AlarmTime.Minutes = 0x04;
 	RTC_AlarmStructure.AlarmTime.Seconds = 0x00;
 	RTC_AlarmStructure.AlarmDateWeekDay = 0x12;
 	RTC_AlarmStructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
 	RTC_AlarmStructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
 	RTC_AlarmStructure.Alarm = RTC_ALARM_B;
 	HAL_RTC_SetAlarm_IT(&hrtc, &RTC_AlarmStructure, RTC_FORMAT_BIN);

    __HAL_RTC_ALARMA_ENABLE(&hrtc);
    /* Configure the Alarm interrupt */
    __HAL_RTC_ALARM_ENABLE_IT(&hrtc,RTC_IT_ALRA);

    __HAL_RTC_ALARMB_ENABLE(&hrtc);
        /* Configure the Alarm interrupt */
        __HAL_RTC_ALARM_ENABLE_IT(&hrtc,RTC_IT_ALRB);
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

