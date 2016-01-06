/*
 * GPIO_Config.c
 *
 *  Created on: 13 nov. 2015
 *      Author: aj81ein
 */


#include "GPIO_Config.h"

void GPIO_Configuration(void){

  /* GPIO, EXTI and NVIC Init structure declaration */
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOH_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

/*  MOTOR_GPIO_CLK_ENABLE();
  DCF_INPUT_GPIO_CLK_ENABLE();
  LED2_GPIO_CLK_ENABLE();
  RELAY_GPIO_CLK_ENABLE();
*/


  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 2);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);


  /**
   * @brief configure motor control pins
   * MOTOR_SLP 	PIN_7
   * MOTOR_STEP PIN_8
   * MOTOR_DIR 	PIN_9
   */
  GPIO_InitStruct.Pin = MOTOR_SLP | MOTOR_STEP | MOTOR_DIR;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(MOTOR_GPIO_PORT, &GPIO_InitStruct);

  /* Configure PC12 pin as input floating, DCF INPUT PIN */
  /**
   * @brief Configure DCF input pin
   * DCF_INPUT = PIN 6
   */
  GPIO_InitStruct.Pin = DCF_INPUT;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT; //_FLOATING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DCF_INPUT_GPIO_PORT, &GPIO_InitStruct);

  /* Configure the LED_pin as output push-pull for LD3 & LD4 usage*/

  /**
   * @brief Configure 2 LED pins
   */
  GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LEDs_GPIO_PORT, &GPIO_InitStruct);
}


