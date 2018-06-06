/*
 * tim.c
 *
 *  Created on: 2018年5月24日
 *      Author: bertz
 */
#include "tim.h"

void DS_TimerInit(void)
{
	GPIO_InitTypeDef		GPIO_InitTypeDef_Struct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeDef_Struct;
	TIM_OCInitTypeDef		TIM_OCInitTypeDef_Struct;
	NVIC_InitTypeDef		NVIC_InitTypeDef_Struct;

	/* 使能定时器时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/* GPIO配置  PB3*/
	GPIO_InitTypeDef_Struct.GPIO_Pin = GPIO_Pin_3;	//TIM2_CH2
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitTypeDef_Struct);

	/* TIM2-PWM */
	TIM_TimeBaseInitTypeDef_Struct.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitTypeDef_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitTypeDef_Struct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitTypeDef_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitTypeDef_Struct);
	TIM_OCInitTypeDef_Struct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitTypeDef_Struct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitTypeDef_Struct.TIM_Pulse = 0;
	TIM_OCInitTypeDef_Struct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM2,&TIM_OCInitTypeDef_Struct);
	TIM_SetCompare2(TIM2,500);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);


	/* 时钟配置 */
	TIM_TimeBaseInitTypeDef_Struct.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitTypeDef_Struct.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitTypeDef_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitTypeDef_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM_TimeBaseInitTypeDef_Struct.TIM_RepetitionCounter = 1000 - 1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitTypeDef_Struct);
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);

	/* NVIC 中断优先级设置 */
	NVIC_InitTypeDef_Struct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitTypeDef_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitTypeDef_Struct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitTypeDef_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitTypeDef_Struct);

	TIM_Cmd(TIM3,ENABLE);
}

