/*
 * adc.c
 *
 *  Created on: 2018年5月22日
 *      Author: Zgeny
 */
#include "adc.h"


void DS_ADC_Init()
{
	GPIO_InitTypeDef GPIO_InitTypeDef_Struct;
	ADC_InitTypeDef	ADC_InitTypeDef_Struct;

	/* 使能 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	/* 初始化ADC - GPIO */
	GPIO_InitTypeDef_Struct.GPIO_Pin = DS_ADC_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(DS_ADC_Port,&GPIO_InitTypeDef_Struct);



}
