/*
 * usart.h
 *
 *  Created on: 2018Äê5ÔÂ21ÈÕ
 *      Author: Zgeny
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f10x.h"
#include "stdint.h"

#define USART_RX_BUF_LEN	512

struct sUsartReciveType
{
    uint8_t     RX_Flag:1;
    uint16_t    RX_Size;
    uint8_t     RX_pData[USART_RX_BUF_LEN];
};
typedef struct sUsartReciveType   USARTRECIVETYPE,    *pUSARTRECIVETYPE;

extern USARTRECIVETYPE CoreBoardUsartType;
extern USARTRECIVETYPE LeftBoardUsartType;

void MX_USART1_Init(void);
void MX_USART2_Init(void);
void MX_USART3_Init(void);


#endif /* USART_H_ */
