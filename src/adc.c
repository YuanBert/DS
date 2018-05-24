/*
 * adc.c
 *
 *  Created on: 2018年5月22日
 *      Author: Zgeny
 */
#include "adc.h"

uint32_t adcBuffer[DS_ADC_BUF_LEN];

void DS_ADC_Init()
{
	GPIO_InitTypeDef GPIO_InitTypeDef_Struct;
	ADC_InitTypeDef	ADC_InitTypeDef_Struct;
	//NVIC_InitTypeDef NVIC_InitTypeDef_Struct;
	//DMA_InitTypeDef DMA_InitTypeDef_Struct;

	/* 使能 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	/* 初始化ADC - GPIO */
	GPIO_InitTypeDef_Struct.GPIO_Pin = DS_ADC_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(DS_ADC_Port,&GPIO_InitTypeDef_Struct);


	/* ADC NVIC 配置 */
	/*
	{
		NVIC_InitTypeDef_Struct.NVIC_IRQChannel = DMA1_Channel1_IRQn;//ADC1_2_IRQn;
		NVIC_InitTypeDef_Struct.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitTypeDef_Struct.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitTypeDef_Struct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitTypeDef_Struct);
	}
	*/

	/* ADC DMA配置 */
	/*
	DMA_DeInit(DMA1_Channel1);
	DMA_InitTypeDef_Struct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitTypeDef_Struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitTypeDef_Struct.DMA_MemoryInc= DMA_MemoryInc_Enable;
	DMA_InitTypeDef_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitTypeDef_Struct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitTypeDef_Struct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitTypeDef_Struct.DMA_Priority = DMA_Priority_Low;
	DMA_InitTypeDef_Struct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitTypeDef_Struct.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	DMA_InitTypeDef_Struct.DMA_BufferSize = DS_ADC_BUF_LEN;
	DMA_InitTypeDef_Struct.DMA_MemoryBaseAddr =  (uint32_t)&(adcBuffer);
	DMA_Init(DMA1_Channel1,&DMA_InitTypeDef_Struct);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	*/
	/* ADC 配置 */
	ADC_DeInit(ADC1);
	ADC_InitTypeDef_Struct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitTypeDef_Struct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitTypeDef_Struct.ADC_ScanConvMode = DISABLE;
	ADC_InitTypeDef_Struct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitTypeDef_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitTypeDef_Struct.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1,&ADC_InitTypeDef_Struct);

	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,1,ADC_SampleTime_239Cycles5);
	ADC_Cmd(ADC1,ENABLE);
	//ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}
uint16_t DS_GetADC_Value()
{
	uint16_t adcValue;
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	adcValue = ADC_GetConversionValue(ADC1);
	return adcValue;
}
