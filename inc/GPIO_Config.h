/*
 * GPIO_Config.h
 *
 *  Created on: 13 nov. 2015
 *      Author: aj81ein
 */

#ifndef GPIO_CONFIG_H_
#define GPIO_CONFIG_H_

#include "stm32l1xx.h"
#include "stm32l1xx_nucleo.h"
#include "stm32l1xx_hal_gpio.h"


#define GPIOA_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()
#define GPIOB_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()
#define GPIOC_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE()
#define GPIOD_CLK_ENABLE()		__HAL_RCC_GPIOD_CLK_ENABLE()
#define GPIOE_CLK_ENABLE()		__HAL_RCC_GPIOE_CLK_ENABLE()
#define GPIOF_CLK_ENABLE()		__HAL_RCC_GPIOF_CLK_ENABLE()
#define GPIOH_CLK_ENABLE()		__HAL_RCC_GPIOH_CLK_ENABLE()


void GPIO_Configuration(void);



#endif /* GPIO_CONFIG_H_ */
