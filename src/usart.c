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
#define USART2_TX_Port		GPIOA
#define USART2_RX_Port		GPIOA
#define USART2_TX_Pin		GPIO_Pin_2
#define USART2_RX_Pin		GPIO_Pin_3

USARTRECIVETYPE CoreBoardUsartType;
USARTRECIVETYPE	LeftBoardUsartType;

void DS_LeftBoardUsartReceive_IDLE()
{
	if(RESET != USART_GetITStatus(USART2,USART_IT_IDLE))
	{
		DMA_Cmd(DMA1_Channel6,DISABLE);
		DMA_ClearFlag(DMA1_FLAG_GL3);
		LeftBoardUsartType.RX_Size = USART_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Channel6);
		LeftBoardUsartType.RX_Flag = 1;
		DMA_SetCurrDataCounter(DMA1_Channel6,USART_RX_BUF_LEN);
		DMA_Cmd(DMA1_Channel6,ENABLE);
		USART_ReceiveData(USART2);
	}
}

void DS_CoreBoardUsartReceive_IDLE()
{
	if(RESET != USART_GetITStatus(USART1,USART_IT_IDLE))
	{
		DMA_Cmd(DMA1_Channel5,DISABLE);
		DMA_ClearFlag(DMA1_FLAG_GL3);
		CoreBoardUsartType.RX_Size = USART_RX_BUF_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);
		CoreBoardUsartType.RX_Flag = 1;
		DMA_SetCurrDataCounter(DMA1_Channel5,USART_RX_BUF_LEN);
		DMA_Cmd(DMA1_Channel5,ENABLE);
		//USART_ClearITPendingBit(USART1,USART_IT_IDLE);
		USART_ReceiveData(USART1);
	}
}


void MX_USART1_Init()
{
	USART_InitTypeDef USART_InitTypeDef_Struct;
	GPIO_InitTypeDef GPIO_InitTypeDef_Struct;
	NVIC_InitTypeDef NVIC_InitTypeDef_Struct;
	DMA_InitTypeDef DMA_InitTypeDef_Struct;

	/* 使能时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

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
	DMA_DeInit(DMA1_Channel5);
	DMA_InitTypeDef_Struct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitTypeDef_Struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitTypeDef_Struct.DMA_MemoryInc= DMA_MemoryInc_Enable;
	DMA_InitTypeDef_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitTypeDef_Struct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitTypeDef_Struct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitTypeDef_Struct.DMA_Priority = DMA_Priority_Low;
	DMA_InitTypeDef_Struct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitTypeDef_Struct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
	DMA_InitTypeDef_Struct.DMA_BufferSize = USART_RX_BUF_LEN;
	DMA_InitTypeDef_Struct.DMA_MemoryBaseAddr =  (uint32_t)&(CoreBoardUsartType.RX_pData);
	DMA_Init(DMA1_Channel5,&DMA_InitTypeDef_Struct);
	DMA_Cmd(DMA1_Channel5,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

	/* USART1 配置 */
	USART_InitTypeDef_Struct.USART_BaudRate = USARTBAUDRATE;
	USART_InitTypeDef_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeDef_Struct.USART_Parity = USART_Parity_No;
	USART_InitTypeDef_Struct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeDef_Struct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeDef_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitTypeDef_Struct);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART1,ENABLE);

}
void MX_USART2_Init(void)
{
	USART_InitTypeDef USART_InitTypeDef_Struct;
	GPIO_InitTypeDef GPIO_InitTypeDef_Struct;
	NVIC_InitTypeDef NVIC_InitTypeDef_Struct;
	DMA_InitTypeDef DMA_InitTypeDef_Struct;

	/* 使能时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* GPIOA.02 TX */
	GPIO_InitTypeDef_Struct.GPIO_Pin = USART2_TX_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_Port,&GPIO_InitTypeDef_Struct);

	/* GPIOA.03 RX */
	GPIO_InitTypeDef_Struct.GPIO_Pin = USART2_RX_Pin;
	GPIO_InitTypeDef_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitTypeDef_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_RX_Port,&GPIO_InitTypeDef_Struct);

	/* USART2 NVIC */
	NVIC_InitTypeDef_Struct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitTypeDef_Struct.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitTypeDef_Struct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitTypeDef_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitTypeDef_Struct);

	/* USART2 DMA */
	DMA_DeInit(DMA1_Channel6);
	DMA_InitTypeDef_Struct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitTypeDef_Struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitTypeDef_Struct.DMA_MemoryInc= DMA_MemoryInc_Enable;
	DMA_InitTypeDef_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitTypeDef_Struct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitTypeDef_Struct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitTypeDef_Struct.DMA_Priority = DMA_Priority_Low;
	DMA_InitTypeDef_Struct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitTypeDef_Struct.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
	DMA_InitTypeDef_Struct.DMA_BufferSize = USART_RX_BUF_LEN;
	DMA_InitTypeDef_Struct.DMA_MemoryBaseAddr =  (uint32_t)&(LeftBoardUsartType.RX_pData);
	DMA_Init(DMA1_Channel6,&DMA_InitTypeDef_Struct);
	DMA_Cmd(DMA1_Channel6,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);

	/* USART2 */
	USART_InitTypeDef_Struct.USART_BaudRate = USARTBAUDRATE;
	USART_InitTypeDef_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitTypeDef_Struct.USART_Parity = USART_Parity_No;
	USART_InitTypeDef_Struct.USART_StopBits = USART_StopBits_1;
	USART_InitTypeDef_Struct.USART_WordLength = USART_WordLength_8b;
	USART_InitTypeDef_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2,&USART_InitTypeDef_Struct);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART2,ENABLE);

}
void MX_USART3_Init(void)
{

}

