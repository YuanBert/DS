/*
 * gpio.h
 *
 *  Created on: 2018Äê5ÔÂ22ÈÕ
 *      Author: Zgeny
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x.h"
#include "main.h"

#define CTR485A_EN_Port				GPIOC
#define CTR485A_EN_Pin				GPIO_Pin_1
#define CTR485A_EN_RCC				RCC_APB2Periph_GPIOC

#define GentleSensor_Port			GPIOC
#define GentleSensor_Pin			GPIO_Pin_4
#define GentleSensor_RCC			RCC_APB2Periph_GPIOC

#define CommunicationLED_Port		GPIOC
#define CommunicationLED_Pin		GPIO_Pin_13
#define CommunicationLED_RCC		RCC_APB2Periph_GPIOC

#define RunningLED_Port				GPIOB
#define RunningLED_Pin				GPIO_Pin_9
#define RunningLED_RCC				RCC_APB2Periph_GPIOB

#define MCUAtmosphereLEDG_Port		GPIOB
#define MCUAtmosphereLEDG_Pin		GPIO_Pin_13
#define MCUAtmosphereLEDG_RCC		RCC_APB2Periph_GPIOB

#define MCUAtosphereLEDR_Port	    GPIOB
#define MCUAtosphereLEDR_Pin		GPIO_Pin_12
#define MCUAtosphereLEDR_RCC		RCC_APB2Periph_GPIOB

#define MCU_LED_OUT_Port			GPIOB
#define MCU_LED_OUT_Pin				GPIO_Pin_6
#define MCU_LED_OUT_RCC				RCC_APB2Periph_GPIOB

#define MCU_FAN_OUT_Port			GPIOB
#define MCU_FAN_OUT_Pin				GPIO_Pin_5
#define MCU_FAN_OUT_RCC				RCC_APB2Periph_GPIOB




void DS_GPIO_Init(void);
void DS_MCU_LED_OUT_ON(void);
void DS_MCU_LED_OUT_OFF(void);
void DS_MCU_FAN_OUT_ON(void);
void DS_MCU_FAN_OUT_OFF(void);
void DS_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


#endif /* GPIO_H_ */
