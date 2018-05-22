/*
 * DS_ProtocolLayer.c
 *
 *  Created on: 2018年5月22日
 *      Author: bertz
 */
#include "ds_protocollayer.h"
#include "usart.h"
#include "main.h"

AckedStruct    CoreBoardAckedData;
AckedStruct    LeftDoorBoardAckedData;

RevDataStruct   CoreBoardRevDataStruct;
RevDataStruct   LeftDoorBoardRevDataStruct;
NeedToAckStruct sNeedToAckStruct;

tTable Table = {{0},0};

/*存放接收到数据*/
static uint8_t CoreRevDataBuf[USART_RX_BUF_LEN];
static uint8_t LeftDoorRevDataBuf[USART_RX_BUF_LEN];

static uint8_t GetAvailableTableID(void)
{
  uint8_t reCode = 0xFF;
  uint8_t i = 0;

  for(i = 0; i < 16; i++)
  {
    if(0 == Table.tab[i])
    {
      return i;
    }
  }
  return reCode;
}

static uint8_t WriteStatusToTable(uint8_t tableID,uint8_t statusCode)//statusCode : 0-表示空，1-表示正在处理，2-表示处理完成可以回复
{
    uint8_t reCode = 0xFF;
    if(tableID > 16)
    {
      return reCode;
    }
    if(0 != statusCode && 0 != Table.tab[tableID])
    {
      return reCode;
    }
    Table.tab[tableID] = statusCode;
    reCode = tableID;
    return reCode;
}


static uint8_t getXORCode(uint8_t* pData,uint16_t len)
{
  uint8_t ret;
  uint16_t i;
  ret = pData[0];
  for(i = 1; i < len; i++)
  {
    ret ^= pData[i];
  }
  return ret;
}

static DS_StatusTypeDef DS_HandingUartData(pRevDataStruct pRevData,pAckedStruct pAckedData,pUSARTRECIVETYPE pUsartType ,uint8_t* pRevDataBuf)
{
	DS_StatusTypeDef state = DS_OK;
	uint8_t xorTemp;
	uint16_t i;

	/* 判断串口数据是否接收完成 */
	if(!(pUsartType->RX_Flag))
	{
		return state;
	}
	pUsartType->RX_Flag = 0;
	/* Handing the ACK Cmd */
	if(0x0A == (*(pUsartType->RX_pData + 1) & 0xF0))
	{
		if(pAckedData->AckCnt > 5)
		{
	       return state;
	    }
	    if(0x5B != *(pUsartType->RX_pData) || 0x5D != *(pUsartType->RX_pData + ACKFIXEDCOMMANDLEN -1))
	    {
	        return state;
	    }
	    xorTemp = getXORCode(pUsartType->RX_pData + 1,3);
	    if(xorTemp != *(pUsartType->RX_pData + 4))
	    {
	      return state;
	    }
	}
	/* Handling Request Cmd Data */
	if(pRevData->RevOKFlag)
	{
		return state;
	}

	if((pRevData->NumberOfBytesReceived) < (pRevData->DataLength) && 0 != (pRevData->NumberOfBytesReceived))
	{
	  for(i = 0; i < pUsartType->RX_Size; i++)
	  {
	    pRevDataBuf[5 + pRevData->NumberOfBytesReceived] = *(pUsartType->RX_pData + i);
	    pRevData->NumberOfBytesReceived ++;
	    if(pRevData->DataLength == pRevData->NumberOfBytesReceived)
	    {
	      pRevData->XOR8BIT = *(pUsartType->RX_pData + i + 1);
	      if(0x5D != *(pUsartType->RX_pData + i + 2))
	      {
	        pRevData->NumberOfBytesReceived = 0;
	        pRevData->DataLength = 0;
	        pRevData->TotalLength = 0;
	        return state;
	      }
	      pRevData->TotalLength = pRevData->DataLength + REQUESTFIXEDCOMMANDLEN;
	      /* here to check XOR code */
	      xorTemp = getXORCode(pRevDataBuf + 1, pRevData->TotalLength - 3);
	      if(pRevData->XOR8BIT != xorTemp)
	      {
	        pRevData->NumberOfBytesReceived = 0;
	        pRevData->DataLength = 0;
	        pRevData->TotalLength = 0;
	        return state;
	      }
	      pRevDataBuf[5 + pRevData->NumberOfBytesReceived] = xorTemp;
	      pRevDataBuf[5 + pRevData->NumberOfBytesReceived + 1] = 0x5D;
	      pRevData->pRevDataBuf = pRevDataBuf;
	      pRevData->RevOKFlag = 1;
	    }
	  }
	  return state;
	}

	if(0 == pRevData->TotalLength)
	{
	  if(0x5B != *(pUsartType->RX_pData))
	  {
		  return state;
	  }
	  pRevData->CmdType      =*(pUsartType->RX_pData + 1);
	  pRevData->CmdParam     =*(pUsartType->RX_pData + 2);
	  pRevData->DataLength   =(*(pUsartType->RX_pData + 3)<< 8) + *(pUsartType->RX_pData + 4);
	  if(0 == pRevData->DataLength)
	  {
	    if(0x5D != *(pUsartType->RX_pData + REQUESTFIXEDCOMMANDLEN - 1))
	    {
	      return state;
	    }
	    pRevData->XOR8BIT         =*(pUsartType->RX_pData + 5);
	    pRevData->TotalLength     = REQUESTFIXEDCOMMANDLEN;
		pRevDataBuf[0]           = 0x5B;
		pRevDataBuf[1]           = pRevData->CmdType;
		pRevDataBuf[2]           = pRevData->CmdParam;
		pRevDataBuf[3]           = *(pUsartType->RX_pData + 3);
		pRevDataBuf[4]           = *(pUsartType->RX_pData + 4);
		xorTemp = getXORCode(pRevDataBuf + 1, REQUESTFIXEDCOMMANDLEN - 3);
		if((pRevData->XOR8BIT) != xorTemp)
		{
		  pRevData->TotalLength = 0;
		  return state;
		}
	    pRevDataBuf[5]           = pRevData->XOR8BIT;
	    pRevDataBuf[6]           = 0x5D;
	    pRevData->RevOKFlag = 1;
	    return state;
	  }
	  for(i = 5; i < pUsartType->RX_Size; i++)
	  {
	    pRevDataBuf[i] = *(pUsartType->RX_pData + i);
	    pRevData->NumberOfBytesReceived ++;
	    if(pRevData->DataLength == pRevData->NumberOfBytesReceived)
	    {
	      if(0x5D != *(pUsartType->RX_pData + REQUESTFIXEDCOMMANDLEN + pRevData->NumberOfBytesReceived - 1))
	      {
	        pRevData->DataLength = 0;
	        pRevData->NumberOfBytesReceived = 0;
	        pRevData->TotalLength = 0;
	      }
	      pRevData->XOR8BIT = *(pUsartType->RX_pData + i + 1 );
	      pRevData->TotalLength = pRevData->DataLength + REQUESTFIXEDCOMMANDLEN;
	      /* here to XOR check */
	      xorTemp = getXORCode(pUsartType->RX_pData + 1, pRevData->TotalLength - 3);
	      /* 校验，如果校验不正确，退出 */
	      if(pRevData->XOR8BIT != xorTemp)
	      {
	        pRevData->TotalLength = 0;
	        return state;
	      }
	      pRevDataBuf[0]           = 0x5B;
	      pRevDataBuf[1]           = pRevData->CmdType;
	      pRevDataBuf[2]           = pRevData->CmdParam;
	      pRevDataBuf[3]           = *(pUsartType->RX_pData + 3);
	      pRevDataBuf[4]           = *(pUsartType->RX_pData + 4);
	      pRevDataBuf[i + 1]           = pRevData->XOR8BIT;
	      pRevDataBuf[i + 2]           = 0x5D;
	      pRevData->RevOKFlag = 1;
	      return state;
	    }
	  }
	}
	return state;
}

/*******************************************************************************
*
*       Function        :DS_HandingUartDataFromCoreBoard()
*
*       Input           :void
*
*       Return          :DS_StatusTypeDef
*
*       Description     :--
*
*
*       Data            :2018/1/31
*       Author          :bertz
*******************************************************************************/
DS_StatusTypeDef DS_HandingUartDataFromCoreBoard()
{
	DS_StatusTypeDef state = DS_OK;
	state = DS_HandingUartData(&CoreBoardRevDataStruct, &CoreBoardAckedData, &CoreBoardUsartType, CoreRevDataBuf);
	return state;
}

DS_StatusTypeDef DS_HandingCoreBoardRequest()
{
	  DS_StatusTypeDef state = DS_OK;
	  uint8_t tempTableID;

	  if(CoreBoardRevDataStruct.RevOKFlag)
	  {
		  tempTableID = GetAvailableTableID();
		  if(0xFF == tempTableID)
		  {
			  return state;
		  }

		  switch((CoreBoardRevDataStruct.CmdType) & 0xF0)
		  {
		  	  case 0xB0:break;
		  	  case 0xC0:break;
		  	  case 0xD0:break;
		  	  case 0xE0:break;
		  	  case 0xF0:break;
		  	  default:state = DS_ERR;break;
		  }

		  CoreBoardRevDataStruct.NumberOfBytesReceived = 0;
		  CoreBoardRevDataStruct.DataLength = 0;
		  CoreBoardRevDataStruct.TotalLength = 0;
		  CoreBoardRevDataStruct.RevOKFlag = 0;

	  }
	  return state;
}



