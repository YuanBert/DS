/*
 * gpio.c
 *
 *  Created on: 2018Äê5ÔÂ22ÈÕ
 *      Author: Zgeny
 */
#include "gpio.h"

void DS_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitTypeDef_Struct;

	RCC_APB2PeriphClockCmd(CTR485A_EN_RCC | GentleSensor_RCC | RunningLED_RCC | CommunicationLED_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(MCU_FAN_OUT_RCC | MCU_LED_OUT_RCC | MCUAtosphereLEDR_RCC | MCUAtmosphereLEDG_RCC ,ENABLE);

	/* GPIOC.1 */
	GPIO_InitTypeDef_Struct.GPIO_Pin = CTR485A_EN_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CTR485A_EN_Port,&GPIO_InitTypeDef_Struct);

	/* GPIOC.4 */
	GPIO_InitTypeDef_Struct.GPIO_Pin = GentleSensor_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GentleSensor_Port,&GPIO_InitTypeDef_Struct);

	/* GPIOB.9 */
	GPIO_InitTypeDef_Struct.GPIO_Pin = RunningLED_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(RunningLED_Port,&GPIO_InitTypeDef_Struct);

	/* GPIOC.13 */
	GPIO_InitTypeDef_Struct.GPIO_Pin = CommunicationLED_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(CommunicationLED_Port,&GPIO_InitTypeDef_Struct);

	/* GPIOB.5 */
	GPIO_InitTypeDef_Struct.GPIO_Pin = MCU_FAN_OUT_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MCU_FAN_OUT_Port,&GPIO_InitTypeDef_Struct);

	/* GPIOB.6 */
	GPIO_InitTypeDef_Struct.GPIO_Pin = MCU_LED_OUT_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MCU_LED_OUT_Port,&GPIO_InitTypeDef_Struct);

	/* GPIOB.12 */
	GPIO_InitTypeDef_Struct.GPIO_Pin = MCUAtosphereLEDR_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MCUAtosphereLEDR_Port,&GPIO_InitTypeDef_Struct);

	/* GPIOB.13 */
	GPIO_InitTypeDef_Struct.GPIO_Pin = MCUAtmosphereLEDG_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MCUAtmosphereLEDG_Port,&GPIO_InitTypeDef_Struct);
}

void DS_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	GPIOx->ODR ^= GPIO_Pin;
}

