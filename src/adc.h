/*
 * adc.h
 *
 *  Created on: 2018��5��22��
 *      Author: Zgeny
 */

#ifndef ADC_H_
#define ADC_H_
#include "stm32f10x.h"

#define DS_ADC_BUF_LEN	20
#define DS_ADC_Pin		GPIO_Pin_2
#define DS_ADC_Port		GPIOC


void DS_ADC_Init(void);

#endif /* ADC_H_ */
