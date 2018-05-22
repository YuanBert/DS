/*
 * adc.c
 *
 *  Created on: 2018��5��22��
 *      Author: Zgeny
 */
#include "adc.h"


void DS_ADC_Init()
{
	GPIO_InitTypeDef GPIO_InitTypeDef_Struct;
	ADC_InitTypeDef	ADC_InitTypeDef_Struct;

	/* ʹ�� ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	/* ��ʼ��ADC - GPIO */
	GPIO_InitTypeDef_Struct.GPIO_Pin = DS_ADC_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(DS_ADC_Port,&GPIO_InitTypeDef_Struct);



}
