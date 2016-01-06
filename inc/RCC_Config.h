/*
 * RCC_Config.h
 *
 *  Created on: 13 nov. 2015
 *      Author: aj81ein
 */


#ifndef RCC_CONFIG_H_
#define RCC_CONFIG_H_

#include "stm32l1xx.h"
#include "stm32l1xx_nucleo.h"
#include "stm32l1xx_hal_tim.h"
#include "stm32l1xx_hal_rcc.h"
#include "stm32l1xx_hal_pwr.h"
#include "main.h"

#define GPIOA_CLK_ENABLE()           			__HAL_RCC_GPIOA_CLK_ENABLE()
#define GPIOB_CLK_ENABLE()           			__HAL_RCC_GPIOB_CLK_ENABLE()
#define GPIOC_CLK_ENABLE()           			__HAL_RCC_GPIOC_CLK_ENABLE()
#define GPIOD_CLK_ENABLE()           			__HAL_RCC_GPIOD_CLK_ENABLE()
#define GPIOE_CLK_ENABLE()           			__HAL_RCC_GPIOE_CLK_ENABLE()
#define GPIOH_CLK_ENABLE()           			__HAL_RCC_GPIOH_CLK_ENABLE()

#define RCC_PWR_CLK_ENABLE()    	 			__HAL_RCC_PWR_CLK_ENABLE();
#define RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE) 	__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
#define RCC_RTC_ENABLE()						__HAL_RCC_RTC_ENABLE();

void RCC_Configuration(void);





#endif /* RCC_CONFIG_H_ */
