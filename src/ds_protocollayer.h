/*
 * ds_protocollayer.h
 *
 *  Created on: 2018Äê5ÔÂ22ÈÕ
 *      Author: bertz
 */

#ifndef DS_PROTOCOLLAYER_H_
#define DS_PROTOCOLLAYER_H_
#include "stm32f10x.h"
#include "main.h"

#define REQUESTFIXEDCOMMANDLEN        7         //Header + CmdType + CmdParam + DataLength + XOR8Bits + End
#define ACKFIXEDCOMMANDLEN            6         //Header + AckCmdCode + AckCodeH + XOR8Bits  + End

struct t_HandingFlag{
	uint8_t		Flag;
	uint8_t		position;
};

struct t_RevDataStruct{

    uint8_t     CmdType;
    uint8_t     CmdParam;
    uint8_t     XOR8BIT;
    uint16_t    DataLength;
    uint16_t    TotalLength;
    uint8_t     NumberOfBytesReceived;
    uint8_t     RevOKFlag;
    uint8_t     *pRevDataBuf;
};
struct t_RevACkStruct{
    uint8_t     AckCmdCode[5];
    uint8_t     AckCodeH[5];
    uint8_t     AckCodeL[5];
    uint8_t     CheckedAckFlag[5];
    uint8_t     AckCnt;
};
struct t_NeedToAckStruct{
  uint8_t     AckCmdCode[16];
  uint8_t     AckCodeH[16];
  uint8_t     AckCodeL[16];
  uint8_t     CmdType[16];
  uint8_t     CmdParam[16];
  uint8_t     DeviceType[16];// 0-None or Error 1-CoreBoard   2-LeftDoorBoard 3-RightDoorBoard
  uint8_t     TableID[16];
};

typedef struct {
  uint8_t tab[16];
  uint8_t tabCnt;
}tTable;

typedef struct t_HandingFlag	  HaningFlag,			*pHandingFlag;
typedef struct t_RevACkStruct     AckedStruct,          *pAckedStruct;
typedef struct t_RevDataStruct    RevDataStruct,        *pRevDataStruct;
typedef struct t_NeedToAckStruct  NeedToAckStruct,      *pNeedToAckStruct;


DS_StatusTypeDef DS_HandingUartDataFromCoreBoard(void);

DS_StatusTypeDef DS_HandingCoreBoardRequest(void);

DS_StatusTypeDef DS_HandingUartDataFromLeftBoard(void);

DS_StatusTypeDef DS_HandingLeftBoardRequest(void);

DS_StatusTypeDef DS_SendDataToCoreBoard(uint8_t *dataBuffer,uint16_t dataLength);

DS_StatusTypeDef DS_SendDataToLeftDoorBoard(uint8_t *dataBuffer,uint16_t dataLength);

DS_StatusTypeDef DS_CheckHandingFlag(void);

DS_StatusTypeDef DS_SendAckData(void);

#endif /* DS_PROTOCOLLAYER_H_ */
