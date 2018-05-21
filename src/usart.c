/*
 * usart.c
 *
 *  Created on: 2018年5月21日
 *      Author: Zgeny
 */
#include "usart.h"
#include "stm32f10x.h"

#define USARTBAUDRATE		115200
#define USART1_TX_Port		GPIOA
#define USART1_TX_Pin		GPIO_Pin_9
#define USART1_RX_Port		GPIOA
#define USART1_RX_Pin		GPIO_Pin_10

void MX_USART1_Init()
{
	USART_InitTypeDef USART_InitTypeDef_Struct;
	GPIO_InitTypeDef GPIO_InitTypeDef_Struct;
	NVIC_InitTypeDef NVIC_InitTypeDef_Struct;

	/* 使能时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,ENABLE);

	/* TX GPIOA.9 */
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeDef_Struct.GPIO_Pin = USART1_TX_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_TX_Port,&GPIO_InitTypeDef_Struct);

	/* RX GPIOA.10 */
	GPIO_InitTypeDef_Struct.GPIO_Pin = USART1_RX_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_RX_Port,&GPIO_InitTypeDef_Struct);

	/* USART1 NVIC 配置 */
	NVIC_InitTypeDef_Struct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitTypeDef_Struct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitTypeDef_Struct.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitTypeDef_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitTypeDef_Struct);

	/* USART1 DMA 配置 */


	USART_InitTypeDef_Struct.USART_BaudRate = USARTBAUDRATE;
	USART_InitTypeDef_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeDef_Struct.USART_Parity = USART_Parity_No;
	USART_InitTypeDef_Struct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeDef_Struct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeDef_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitTypeDef_Struct);


}

