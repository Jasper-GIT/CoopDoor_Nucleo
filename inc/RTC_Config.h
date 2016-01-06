/*
 * RTC_Config.h
 *
 *  Created on: 13 nov. 2015
 *      Author: aj81ein
 */

#ifndef RTC_CONFIG_H_
#define RTC_CONFIG_H_

#include "stm32l1xx.h"
#include "stm32l1xx_nucleo.h"
#include "stm32l1xx_hal_tim.h"
#include "stm32l1xx_hal_rtc.h"
#include "main.h"

#define HAL_RTC_ALARMA_DISABLE()  		__HAL_RTC_ALARMA_DISABLE(__HANDLE__);
#define HAL_RTC_ALARMB_DISABLE()		__HAL_RTC_ALARMB_DISABLE(__HANDLE__);

#define HAL_RTC_ALARMA_ENABLE()			__HAL_RTC_ALARMA_ENABLE(__HANDLE__);
#define HAL_RTC_ALARMB_ENABLE() 		__HAL_RTC_ALARMB_ENABLE(__HANDLE__);

void RTC_Configuration(void);
void SetTime_Configuration(unsigned char Hours, unsigned char  Minutes);
void SetDate_Configuration(unsigned char Year, unsigned char Month, unsigned char Date);
//void Update_RTC(unsigned char Hours, unsigned char Minutes, unsigned char Seconds, unsigned char Year, unsigned char Month, unsigned char Day);
void Update_RTC(void);
void MX_RTC_Init(void);

#endif /* RTC_CONFIG_H_ */


