/*
 * RCC_Config.c
 *
 *  Created on: 13 nov. 2015
 *      Author: aj81ein
 */

#include "RCC_Config.h"
#include "stm32l1xx_hal.h"

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLMUL                         = 6
  *            PLLDIV                         = 3
  *            Flash Latency(WS)              = 1
  * @retval None
  */

void RCC_Configuration(void){
	  RCC_ClkInitTypeDef 		RCC_ClkInitStruct = {0};
	  RCC_OscInitTypeDef 		RCC_OscInitStruct = {0};
	  RCC_PeriphCLKInitTypeDef 	RCC_PeriphClkInit = {0};

	  /* Set Voltage scale1 as MCU will run at 32MHz */
	  __PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


	  /* Enable HSE Oscillator and Activate PLL with HSE as source */
	  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
	  //RCC_OscInitStruct.LSEState			= RCC_LSE_ON;
	  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL6;
	  RCC_OscInitStruct.PLL.PLLDIV          = RCC_PLL_DIV3;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    //Error_Handler();
	  }


	  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
	  while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {};

	  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	  clocks dividers */
	  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	  {
	    //Error_Handler();
	  }

	  __HAL_RCC_PWR_CLK_ENABLE();
	  HAL_PWR_EnableBkUpAccess();


	  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    //Error_Handler();
	  }

	  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	  RCC_PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
	  {
	    //Error_Handler();
	  }

	  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/100);

	  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	  /* SysTick_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 2);

	  /*##-2- Enable RTC peripheral Clocks #######################################*/
	  /* Enable RTC Clock */
	  __HAL_RCC_RTC_ENABLE();

	  /*##-4- Configure the NVIC for RTC Alarm ###################################*/
	  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 3);
	  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

	}


